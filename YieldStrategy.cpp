#include <iostream>
#include "YieldStrategy.h"
#include "Action.h"
#include "kdtree.h"
#include "math.h"

using namespace std;

static double dist_sq( double *a1, double *a2, int dims ) {
	  double dist_sq = 0, diff;
	    while( --dims >= 0 ) {
		        diff = (a1[dims] - a2[dims]);
			    dist_sq += diff*diff;
			      }
	      return dist_sq;
}

Action* YieldStrategy::getHighestYieldingAction(const funcSensor* sensors,BMap* map, int curOr, int curXBel, int curYBel, int tProb){

    //cout << "getting highest yeilding action \n";

    int testReading0 = (*sensors[S0])(0);
    int testReading90 = (*sensors[S90])(0);
    int testReading180 = (*sensors[S180])(0);
    int testReading270 = (*sensors[S270])(0);

    //cout << "test read of sensor array 0 " << testReading0 << "\n";
    //cout << "test read of sensor array 90 " << testReading90 << "\n";
    //cout << "test read of sensor array 180 " << testReading180 << "\n";
    //cout << "test read of sensor array 270 " << testReading270 << "\n";

    int* vds =(int*) malloc(sizeof(int) * 4); 
    int svds = 0;
/********************************************************************************
	DO COLLISION AVOIDANCE FIRST
********************************************************************************/
    Action* next = new Action();
    next->c = ActionTypes::NOP;
    next->m = 0;

    curOr = (*sensors[OR0])(0);

    int COLLISION_INTERCEPT = COLLISION_THRESHOLD + STRIDE;

    if(((*sensors[S0])(0)) > COLLISION_INTERCEPT 
		    && ( ((*sensors[S0])(0)) < MAX_RANGE ||  ((*sensors[S180])(0)) < MAX_RANGE )) {
    	*(vds+svds) = 0;
    	svds++;
    }
   
    if(((*sensors[S90])(0)) > COLLISION_INTERCEPT 
		    && ( ((*sensors[S90])(0)) < MAX_RANGE ||  ((*sensors[S270])(0)) < MAX_RANGE )) {
	    *(vds+svds) = 90;
	    svds++;
    }

    if(((*sensors[S180])(0)) > COLLISION_INTERCEPT
		    && ( ((*sensors[S180])(0)) < MAX_RANGE ||  ((*sensors[S0])(0)) < MAX_RANGE )) {
	    *(vds+svds) = 180;
	    svds++;
    }
    
    if(((*sensors[S270])(0)) > COLLISION_INTERCEPT
		    && ( ((*sensors[S270])(0)) < MAX_RANGE ||  ((*sensors[S90])(0)) < MAX_RANGE )) {
	    *(vds+svds) = 270;
	    svds++;
    }


	int suggestedBearing = YieldFunction1(map, vds, svds);

	std::cout << "Suggested Bearing is " << suggestedBearing << "\n";


#if ARDUINO == 1
    if((random(1,101)) < tProb) {
        int index = random(0,svds);
#else
    if((rand() % 100) < tProb) {
    	int index = rand() % svds;
#endif	    
	next->c = ActionTypes::TURN;
   	next->m = *(vds+index);
    } else {

	int i;

	// this is redundant
	for(i=0; i<svds;i++) {
		if(*(vds+i) == curOr) {
                    next->c = ActionTypes::MOVE;
		    next->m = STRIDE;
		    break;
		}
	}
    }

    free(vds);
    
    return next;
}


int YieldStrategy::YieldFunction1(BMap* map, int* viableDirections, int sizeViableDirections) {
/***************************************************************************
	NOW WORK OUT YIELD
***************************************************************************/
// can only move in a direction that is in svds
// so test each available location for KNN density

    int currentX = map->_curx;
    int currentY = map->_cury;

    kdtree *kd;

// need to fix the radius via thresholds.h

    kd = kd_create(3);

    std::cout << "map size is : " << map->_size << "\n";
    std::cout << "current x : " << map->_curx << "\n";
    std::cout << "current y : " << map->_cury << "\n";

    for(int i=0;i<map->_size;i++) {
	
	Hn* point = map->GetPoint(i);

	//std::cout << "point x " << point->x << "\n";

	int x = 0; //point->x;
	int y = -10 + i; //point->y;
	int is_obstacle = 1; // fix the density = point->is_obstacle;

	kd_insert3(kd,x,y,0,is_obstacle);

    } 

/*
    kd_insert3(kd, 1, 2, 0, 1);
    kd_insert3(kd, 2, 2, 0, 0);
    kd_insert3(kd, 5, 2, 0, 0);
    kd_insert3(kd, 7, 7, 0, 0);
    kd_insert3(kd, 8, 7, 0, 0);
    kd_insert3(kd, 9, 2, 0, 1);
    kd_insert3(kd, 9, 2, 0, 1);
    kd_insert3(kd, 1, 7, 0, 1);
    kd_insert3(kd, 5, 2, 0, 1);
    kd_insert3(kd, 7, 2, 0, 1);
    kd_insert3(kd, 4, 7, 0, 1);
    kd_insert3(kd, 7, 7, 0, 1);
    kd_insert3(kd, 8, 7, 0, 1);
    kd_insert3(kd, 8, 2, 0, 1);
    kd_insert3(kd, 8, 2, 0, 1);
    kd_insert3(kd, 8, 7, 0, 1);
    kd_insert3(kd, 7, 2, 0, 1);
    kd_insert3(kd, 9, 2, 0, 1);
    kd_insert3(kd, 9, 2, 0, 1);
*/

    // points to check
    //

    float max;
    max = 0.0;

    int targetBearing;
    targetBearing = 0.0;


    for(int x=0;x<sizeViableDirections;x++) {

    int direction = *(viableDirections+x);
    int targetDistance = STRIDE;// * 2;

    std::cout << "checking bearing " << direction << "\n";

    // check density going outwards
    // if returning zero could extend - for now just do one check
    //
    // add bearing and targetDistance to current location 
    //
    
    Vct* offsetVct;

    offsetVct = Utils::lvfp(direction, targetDistance);
    
    int targetX = currentX + offsetVct->x;
    int targetY = currentY + offsetVct->y;

    std::cout << "targetX is : " << targetX << "\n";
    std::cout << "targetY is : " << targetY << "\n";


    kdres  *set;
    //char *data;
    int pch;
    double pos[3], dist;
    double pt[3] = { targetX, targetY, 0 };
    double radius = 10;
 
    /* find points closest to the origin and within distance radius */
    set = kd_nearest_range(kd, pt, radius );

    std:: cout << "size of set is: " << kd_res_size(set)<< "\n";

    int size = 0;
    size = kd_res_size(set);

    int sum;
    sum = 0;
    
    while(!kd_res_end(set)) {
	    /* get the data and position of the current result item */
	    std::cout << "iterator 1 " << "\n";
	    pch = kd_res_item( set, pos );

	    std::cout << "iterator 1 " << "\n";
 	    /* compute the distance of the current result from the pt */
	    dist = sqrt( dist_sq( pt, pos, 3 ) );
	    std::cout << "dist: " << dist << "\n"; 

	    /* print out the retrieved data */
            printf( "node at (%.3f, %.3f, %.3f) is %.3f val %d\n", 
				                  pos[0], pos[1], pos[2], dist, pch);

            sum+=pch;

// 	    /* go to the next entry */
	    kd_res_next( set);
    }

    	float avg = (float) sum / (float) size; 

    	std::cout << "sum is: " << sum << " size is " << size  << " average is: " << avg << "\n";


	if(avg > max) {
		max = avg;
		targetBearing = direction;
	}

  	//free(data);
 	kd_res_free(set);
  
    }
  
    kd_free(kd);

    return targetBearing;
}
