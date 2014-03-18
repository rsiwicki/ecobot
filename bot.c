#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "thresholds.h"
#include "bot.h"
#include "sensors.h"


// added these libs using the IDE import library
#if ARDUINO == 1
#include <EcoSensors.h>
#include <EcoSPI.h>
#include <SPI.h>
#include "pins_arduino.h"
#endif

/****************************
 *
 * Very basic C program to control the bot.
 * Keeping everything simple using simple C constructs
 * as we have to make this run on a microcontroller.
 *
 * Rob Siwicki @ ComparetheMarket.com
 *
 * *******************************/

#if ARDUINO == 1

EcoSPI utrasonic_espi(5, DEVICE_ULTRASONIC);

EcoSensors es;

#endif

funcSensor sensors[NUMSNSRS];

unsigned int pos(int n) {
	int const mask = n >> sizeof(int) * BITS -1;
	unsigned int r = (n + mask) ^ mask;
	return (unsigned) r;
}

struct Map* new_map() {
    struct Map* m = (struct Map*) malloc(sizeof(struct Map));
    m->size=0;
    m->maxdimx=0; // more efficient on insert
    m->maxdimy=0;
    return m;
}

struct Wm* new_wm() {
    struct Wm* wm = (struct Wm*) malloc(sizeof(struct Wm));
    wm->size=0;
    return wm;
}

void map_add_belief(struct Map* m, int x, int y, float temp, int obs) {

    if(m->size < MAX_POINTS) {	
   	 struct Hn* point = (struct Hn*) malloc(sizeof(struct Hn));

    	point->x=x;
    	point->y=y;
	#if ARDUINO == 1
    	point->t=101;
	#else    
    	point->t= time(NULL);
	#endif
    	point->temp_1=temp;
    	point->is_obstacle=obs;

    	int s=m->size;

    	m->points[s] = point; 
    	m->maxdimx = x > m->maxdimx ? x : m->maxdimx;
    	m->maxdimy = y > m->maxdimy ? y : m->maxdimy;
    	m->size++;
    } else {
	// we cannot add anymore points due to memory pressure (perhaps!)
	// need to either offload distant points to wifi
	// or 
	// reduce using KNN
	printf("WARNING - need to reduce the map using KNN");	
    }
}

void agnt_take_reading(struct Wm* wm, enum acts act) {

    struct LocF* fact = (struct LocF*) malloc(sizeof(struct LocF));
    
#if ARDUINO == 1
    fact->t0 = 101;
#else    
    fact->t0 = time(NULL);
#endif

    fact->s0 = ((*sensors[S0])(0));
    fact->s90 = ((*sensors[S90])(0));
    fact->s180 = ((*sensors[S180])(0));
    fact->s270 = ((*sensors[S270])(0));
    fact->ori = ((*sensors[OR0])(0));
    fact->precact = act;

    printf("sensor reading %d\n", fact->s0);

    wm->facts[wm->size++] = fact;
}

struct LocF* wm_get_latest_fact(struct Wm* wm) {
    return wm->facts[wm->size-1];
}

struct vct lvfp(int d, int m) {

	d = d % 360;

	struct vct v;
	v.x = 0;
	v.y = 0;

	if(d>=0&&d<=90) {
            double r = (90 - d) / (180/PI);
	    v.x = m * cos(r);
	    v.y = m * sin(r);
	}
	else if(d>=90&&d<=180) {
            double r = (d - 90) / (180/PI);
	    v.x = m * cos(r);
	    v.y = m * sin(r) * -1;
	}
	else if(d>=180&&d<=270) {
            double r = (270 - d) / (180/PI);
	    v.x = m * cos(r) * -1;
	    v.y = m * sin(r) * -1;
	}
	else if(d>=270&&d<=360) {
            double r = (d - 270) / (180/PI);
	    v.x = m * cos(r) * -1;
	    v.y = m * sin(r);
	}

	return v;
}

struct Actn collision_avoidance_str(struct Map* beliefMap, int curOr, int curXBel, int curYBel, int tProb) {


    int* vds =(int*) malloc(sizeof(int) * 4); 
    int svds = 0;

    int test = 10;

    struct Actn next;
    next.c = NOP;
    next.m = 0;

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
printf("svds:%d\n",svds);
#if ARDUINO == 1
    if((random(1,101)) < tProb) {
        int index = random(0,svds);
#else
    if((rand() % 100) < tProb) {
    	int index = rand() % svds;
#endif	    
	next.c = TURN;
   	next.m = *(vds+index);
    } else {

	int i;

	// this is redundant
	for(i=0; i<svds;i++) {
		if(*(vds+i) == curOr) {
                    next.c = MOVE;
		    next.m = STRIDE;
		    break;
		}
	}
    }

    free(vds);
    
    return next;
}

struct Map* forward_chain_belief_map(struct Wm* wm) {

// need to plot all the obstacles as coordinates in the a' relative system
// at the moment they are curr v relative.

    struct LocF* earliestFact = wm->facts[0]; // get the first fact and chain forwards
    
    // DEBUG -- need to test previous fact here
    struct LocF* previousFact = wm->facts[wm->size-2];

    struct Map* beliefs = new_map();

    struct vct curVct;
    curVct.x = 0;
    curVct.y = 0;

    int i;

    for(i=0;i<wm->size; i++) {
	struct LocF* currentFact = wm->facts[i];
	
	int delta0 = previousFact->s0 - currentFact->s0;
	int delta90 = previousFact->s90 - currentFact->s90;
	int delta180 = previousFact->s180 - currentFact->s180;
	int delta270 = previousFact->s270 - currentFact->s270;

	int dOr = previousFact->ori - currentFact->ori;

	printf("precact %d\n", currentFact->precact);

        switch(currentFact->precact) {
		case TURN:
		    if(currentFact->s0 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr,currentFact->s0);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };
		   
		    if(currentFact->s90 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+90, currentFact->s90);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };

		    if(currentFact->s180 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+180, currentFact->s180);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };

		    if(currentFact->s270 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+270, currentFact->s270);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };
		break;
		case MOVE:
		    // could add error correction here
		    if(pos(delta0) < 1 && pos(delta90) < 1 && pos(delta180) < 1 && pos(delta270) < 1) {
		        printf("no movement detected post move command.\n");	
			break;
		    }
		
		    struct vct dVct; 

		    // object should have moved closer on 0	    
		    // this means an increase in the delta
		    // e.g. if previous s0 100 and current s0 95 then we have moved forward 5
		    // at current bearing
		    // if delta0 is unavailable then we have to use delta 180 in which case
		    // the delta should be negative i.e. we have moved away from object
		    // by the amount of delta. Therefore new location vector is abs(delta)
		    // but need to add 180 degrees
		    printf("delta0 %d\n", delta0);
		    if(delta0 > MEASURE_TOLERANCE) {
			  dVct  = lvfp(currentFact->ori, delta0); 
			  printf("Movement detected in forward sensor %d\n", delta0);
		    }		    
		    else if(delta180 < MEASURE_TOLERANCE) {
			  dVct  = lvfp(currentFact->ori + 180, pos(delta180)); 
			  printf("Movement detected in aft sensor %d\n", delta180);
		    }		    
		    else {
			printf("PANIC! In dev null. You shouldn't let agent turn 90 if there is no contact on 90 or 270. You really should add that to the collision strategy.\n");
		    }		    

		    printf("dVct.x is %d\n", dVct.x);	
		    printf("dVct.y is %d\n", dVct.y);	

		    curVct.x += dVct.x;
		    curVct.y += dVct.y;
		    
		    if(currentFact->s0 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr,currentFact->s0);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };
		   
		    if(currentFact->s90 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+90, currentFact->s90);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };

		    if(currentFact->s180 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+180, currentFact->s180);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };

		    if(currentFact->s270 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+270, currentFact->s270);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };
		break;
		case NOP:
		    if(currentFact->s0 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr,currentFact->s0);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };
		   
		    if(currentFact->s90 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+90, currentFact->s90);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };

		    if(currentFact->s180 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+180, currentFact->s180);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };

		    if(currentFact->s270 < MAX_RANGE) {
			    struct vct relVct = lvfp(dOr+270, currentFact->s270);
			    map_add_belief(beliefs, curVct.x+relVct.x, curVct.y+relVct.y, 1.0, OBS);
		    };
		break;
     
  		printf("no operation.\n");
		default:
		   printf("Unknown command.\n");
		break;
	}	

        //printf("data %d @\n", m->points[i]->x);
        printf("sensor reading in build map previous fact %d \n", previousFact->s0);
        printf("sensor reading in build map current fact %d \n", currentFact->s0);
    	printf("current position x %d\n", curVct.x);
    	printf("current position y %d\n", curVct.y);
    
    } 

    return beliefs;

}

#if ARDUINO == 1
int freeRam () {
	  extern int __heap_start, *__brkval; 
	  int v; 
	  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif

// draw a i 50*50 grid centred on current location and plot the readings
//
void print_belief_map(struct Map* map) {

	int grid[51][51];

	// sprintf
	printf("Printing belief map\n");

	int x, y;

	for(y=0;y<51;y++) {
		for(x=0;x<51;x++) {
			grid[x][y]=0;	
		}
	}
	
	int i;

	for(i=0;i<map->size;i++) {
		struct Hn* point = map->points[i];

		int rebasedPointX = 21 + point->x;
		int rebasedPointY = 21 + point->y;
		int isObstacle = point->is_obstacle;

		if (rebasedPointX > 0 && rebasedPointX < 51 && rebasedPointY > 0 && rebasedPointY < 51) {
			grid[rebasedPointX] [rebasedPointY] = isObstacle;
		}

		printf("%d,%d,%d\n",point->x, point->y, point->is_obstacle);
		printf("%d,%d,%d\n",rebasedPointX, rebasedPointY, isObstacle);
	}

	for(y=51;y>=0;y--) {
		printf("\n");
		for(x=0;x<51;x++) {
			if(grid[x][y]==0) {
				printf(" -");
			} else {
				printf(" %d", grid[x][y]);	
			}
		}
	}

	printf("\nFinished printing beleif map\n");
}

// dont want to use a fixed array as it limits the number of points
// currently memory is the constraint so use a sparse matrix.
int rundiagnostic() {

	printf("---------------------------------------------\n");
        printf("Running Diagnostic\n");
	printf("---------------------------------------------\n");
	printf("charbits are %d\n", BITS);
	printf("pos should be positive %d\n", pos(-10));
	printf("size of LocF %d\n", sizeof(struct LocF));
	printf("size of Hn %d\n", sizeof(struct Hn));
	printf("---------------------------------------------\n");
	printf("testing vectors\n");
	printf("---------------------------------------------\n");
	printf("vec test x is 1 %d\n", lvfp(90,1).x);
	printf("vec test y is 0 %d\n", lvfp(90,1).y);
	printf("vec test x is 2 %d\n", lvfp(90,2).x);
	printf("vec test y is 0 %d\n", lvfp(90,2).y);
	printf("vec test x is 0 %d\n", lvfp(0,1).x);
	printf("vec test y is 1 %d\n", lvfp(0,1).y);
	printf("vec test x is 0 %d\n", lvfp(180,1).x);
	printf("vec test y is -1 %d\n", lvfp(180,1).y);
	printf("vec test x is -1 %d\n", lvfp(270,1).x);
	printf("vec test y is 0 %d\n", lvfp(270,1).y);
	
	
        struct vct dVct  = lvfp(0, 10); 
	printf("This should be  0 %d\n", dVct.x);
	printf("This should be 10 %d\n", dVct.y);

	printf("---------------------------------------------\n");
	printf("Loading sensor array with mocks\n");
	printf("---------------------------------------------\n");
	
	sensors[S0] = &read_within_range;
	sensors[S90] = &read_within_range;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_within_range;
	sensors[T0] = &read_within_range;
	sensors[OR0] = &read_orientation_is_0;


	printf("---------------------------------------------\n");
	printf("Testing map\n");
	printf("---------------------------------------------\n");
	
	struct Map* beliefs;
	
	beliefs = new_map();

	map_add_belief(beliefs, 10,20, 1.0, 0);
	map_add_belief(beliefs, 100, 20, 1.0, 1);
	map_add_belief(beliefs, 90, 20, 1.0, 1);
	map_add_belief(beliefs, 100, 20, 1.0, 1);
	
	printf("belief size 4 %d\n",beliefs->size);

	free(beliefs);
	
	printf("---------------------------------------------\n");
	printf("Testing collision avoidance strategy\n");
	printf("---------------------------------------------\n");
	printf("Mocking sensors for collision at 0 90 180 100 pct probability of turn\n");
	
	beliefs = new_map();

	map_add_belief(beliefs, 10,20, 1.0, 0);
	map_add_belief(beliefs, 100, 20, 1.0, 1);
	map_add_belief(beliefs, 90, 20, 1.0, 1);
	map_add_belief(beliefs, 100, 20, 1.0, 1);
	
	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_within_range;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_will_collide;
	sensors[OR0] = &read_orientation_is_0;
	
	struct Actn action = collision_avoidance_str(beliefs, 0, 0, 0, 100); // 100% prob of turn

	printf("Action is 0 %d with magnitude 90 %d\n", action.c, action.m);

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_within_range;
	sensors[OR0] = &read_orientation_is_0;

	action = collision_avoidance_str(beliefs, 0, 0, 0, 100); // 100% prob of turn

	printf("Action is 0 %d with magnitude 270 %d\n", action.c, action.m);

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_within_range;
	sensors[OR0] = &read_orientation_is_270;

	action = collision_avoidance_str(beliefs, 0, 0, 0, 0); // 0% prob of turn

	printf("Action is 1 %d with magnitude 10 %d\n", action.c, action.m);

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_beyond_range;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_beyond_range;
	sensors[OR0] = &read_orientation_is_0;

	action = collision_avoidance_str(beliefs, 0, 0, 0, 100); // 0% prob of turn

	printf("Action is 0  %d with magnitude 180 %d\n", action.c, action.m);

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_within_range;
	sensors[OR0] = &read_orientation_is_270;

	action = collision_avoidance_str(beliefs, 0, 0, 0, 0); // 0% prob of turn

	printf("Action is 1 %d with magnitude 10 %d\n", action.c, STRIDE, action.m);

	free(beliefs);
	
	printf("---------------------------------------------\n");
	printf("Testing working memory\n");
	printf("---------------------------------------------\n");

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_beyond_range;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_beyond_range;
	sensors[OR0] = &read_orientation_is_0;
	
	struct Wm* wm = new_wm();

	agnt_take_reading(wm, TURN);

	struct LocF* lf = wm_get_latest_fact(wm);
	printf("Wm size %d\n", wm->size);
	printf("latest clock reading %d\n", lf->t0);
	printf("latest S0 reading %d\n", lf->s0);
	
	free(wm);

	beliefs = new_map();

	sensors[S0] = &read_at_range_100;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_will_collide;
	sensors[OR0] = &read_orientation_is_0;

	agnt_take_reading(wm,NOP);

	print_belief_map(beliefs);
	//struct BeliefMap* m1 = forward_chain_belief_map(wm);
	// seg fault
        	
	// force it to move forwards
	action = collision_avoidance_str(beliefs, 0, 0, 0, 0); // 0% prob of turn
	printf("Action is 1  %d with magnitude 10 %d\n", action.c, action.m);

	sensors[S0] = &read_at_range_90;

	agnt_take_reading(wm,action.c);

	struct Map* m2 = forward_chain_belief_map(wm);

	print_belief_map(m2);


	action = collision_avoidance_str(beliefs, 0, 0, 0, 0); // 0% prob of turn
	printf("Action is 1  %d with magnitude 10 %d\n", action.c, action.m);

	sensors[S0] = &read_at_range_80;

	agnt_take_reading(wm,action.c);

	struct Map* m3 = forward_chain_belief_map(wm);

	print_belief_map(m3);

	action = collision_avoidance_str(beliefs, 0, 0, 0, 0); // 0% prob of turn
	printf("Action is 1  %d with magnitude 10 %d\n", action.c, action.m);

	sensors[S0] = &read_at_range_70;

	agnt_take_reading(wm,action.c);

	struct Map* m4 = forward_chain_belief_map(wm);

	print_belief_map(m4);

	free(wm);

	printf("----------------------------------------------------\n");
	printf("testing movement and beleif map\n");
	printf("----------------------------------------------------\n");
	
	struct Wm* wm2 = new_wm();
	beliefs = new_map();

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_will_collide;
	sensors[OR0] = &read_orientation_is_0;

	agnt_take_reading(wm2,NOP);
	agnt_take_reading(wm2,NOP);
	agnt_take_reading(wm2,NOP);
	
	struct Map* m5 = forward_chain_belief_map(wm2);
		
	print_belief_map(m5);


#if ARDUINO == 1
	printf("Compiled for Arduino\n");
#else
	printf("gcc compiled\n");
#endif

	return 0;
}


#if ARDUINO != 1
int main() {

	return rundiagnostic();
}
#endif

#if ARDUINO == 1


int ReadLine(char str[]) {
	   char c;
	      
	   int index =0;
	         
	   while(true) {
	   	if(Serial.available() > 0){
		c = Serial.read();
  		if(c!='\n') {
			str[index++] = c;
		} else
	           str[index] = '\0';
		   break;
		}
	   }
   	   return index;
}
   



int my_putc( char c, FILE *t) {
	  Serial.write( c );
}

void setup() {
	     // put your setup code here, to run once:
	     fdevopen( &my_putc, 0);

           
	
             sensors[S0] = &read_0_sonic_real;
             sensors[S90] = &read_90_sonic_real;
	     sensors[S180] = &read_180_sonic_real;
	     sensors[S270] = &read_270_sonic_real;
	     sensors[OR0] = &read_orientation_is_0;



             Serial.begin(115200);
             randomSeed(analogRead(0));	       
         
             digitalWrite(SS, HIGH);  // ensure SS stays high for now

             // Put SCK, MOSI, SS pins into output mode
             // also put SCK, MOSI into LOW state, and SS into HIGH state.
             // Then put SPI hardware into Master mode and turn SPI on
             SPI.begin();
    
}

void loop() {
	  //put your main code here, to run repeatedly: 
	delay(1000);
        
        //rundiagnostic();
	
        //rundiagnostic();
	utrasonic_espi.getReadings(es);
	//Serial.println(es.ultrasonic0); 
        
	printf("%d %d %d %d\n", es.ultrasonic0, es.ultrasonic90, es.ultrasonic180, es.ultrasonic270);
	
        struct Map* beliefs;
	
	beliefs = new_map();

         // debug - need to add defensive code that means function poitners are all valid prior to entry into function
	 struct Actn action = collision_avoidance_str(beliefs, 0, 0, 0, TURN_PROBABILITY);// 0% prob of turn
	 //printf("Action is 1  %d with magnitude 10 %d\n", action.c, action.m);
         if(action.c == MOVE) {
	   printf("forward %d cm\n", action.m);
         } else if(action.c == TURN) {
           printf("turn %d deg\n",action.m);
         } else if(action.c == NOP) {
           printf("NOP\n");
         } else {
           printf("dunno\n");
         }

         char inputdata[20];
         Serial.println("done\n");

         ReadLine(inputdata); 

         Serial.println("you clicked\n");

	 Serial.flush();
}



#endif
