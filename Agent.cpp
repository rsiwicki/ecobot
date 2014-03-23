#include <iostream>
#include "thresholds.h"
#include "Utils.h"
#include "LocF.h"
#include "Hn.h"
#include "StrategyFactory.h"
#include "sensors.h"
#include "StrategyTypes.h"
#include "WorkingMemory.h"

using namespace std;

// draw a i 50*50 grid centred on current location and plot the readings
// DO NOT WANT THIS IN THE REAL CODE - JUST FOR DEBUGGING
void print_belief_map(BMap* map) {

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

	for(i=0;i<map->_size;i++) {
		Hn* point = map->GetPoint(i);

		int rebasedPointX = 25 + point->x;
		int rebasedPointY = 25 + point->y;
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
			if(x == (map->_curx +25) && y == (map->_cury+25)) {
				printf(" ^");
			}
			else if(x == 25 && y == 25) {
				printf(" 0");
			} else if(grid[x][y]==0) {
				printf(" -");
			} else {
				printf(" %d", grid[x][y]);	
			}
		}
	}

	printf("\nFinished printing beleif map\n");
}

// just orchestrates the working memory
void agent_take_reading(funcSensor* sensors, WorkingMemory* wm, ActionTypes::Enum act) {

    struct LocF* fact = new LocF();
    
    fact->s0 = ((*sensors[S0])(0));
    fact->s90 = ((*sensors[S90])(0));
    fact->s180 = ((*sensors[S180])(0));
    fact->s270 = ((*sensors[S270])(0));
    fact->ori = ((*sensors[OR0])(0));
    fact->precact = act;

    printf("sensor reading %d\n", fact->s0);

    // DEBUG - encapsulate
    wm->AssertFact(fact);
}


void RunDiagnostic() {
	cout << "Starting Agent\n";

	std::cout << "---------------------------------------------\n";
        std::cout << "Running Diagnostic\n";
	std::cout << "---------------------------------------------\n";
	std::cout << "charbits are " << BITS << "\n";
	std::cout << "pos should be positive " << Utils::pos(-10) <<  "\n";
	std::cout << "size of LocF " <<  sizeof(LocF) << "\n";
	std::cout << "size of Hn " <<  sizeof(Hn) << "\n";
	std::cout << "---------------------------------------------\n";
	std::cout << "testing vectors\n";
	std::cout << "---------------------------------------------\n";
	std::cout << "vec test x is 1 " <<  Utils::lvfp(90,1)->x << "\n";
	std::cout << "vec test y is 0 " <<  Utils::lvfp(90,1)->y << "\n";
	std::cout << "vec test x is 2 " <<  Utils::lvfp(90,2)->x << "\n";
	std::cout << "vec test y is 0 " <<  Utils::lvfp(90,2)->y << "\n";
	std::cout << "vec test x is 0 " <<  Utils::lvfp(0,1)->x << "\n";
	std::cout << "vec test y is 1 " <<  Utils::lvfp(0,1)->y << "\n";
	std::cout << "vec test x is 0 " <<  Utils::lvfp(180,1)->x << "\n";
	std::cout << "vec test y is -1 " <<  Utils::lvfp(180,1)->y << "\n";
	std::cout << "vec test x is -1 " <<  Utils::lvfp(270,1)->x << "\n";
	std::cout << "vec test y is 0 " <<  Utils::lvfp(270,1)->y << "\n";

	Vct* dVct = Utils::lvfp(0,10);
	std::cout << "this should be 0" << dVct->x << "\n";
	std::cout << "this should be 10" << dVct->y << "\n";

	BMap* beliefs = new BMap();

	std::cout << "---------------------------------------------\n";
	std::cout << "Test Add Beliefs\n";
	std::cout << "---------------------------------------------\n";

	beliefs->AddBelief(0,0,1.0,1);
	std::cout << "Size of beliefs is (1) " << beliefs->_size  << "\n";

	beliefs->AddBelief(5,5,1.0,1);
	std::cout << "Size of beliefs is (2) " << beliefs->_size  << "\n";

	beliefs->AddBelief(10,10,1.0,1);
	std::cout << "Size of beliefs is (3) " << beliefs->_size  << "\n";
	
	beliefs->AddBelief(15,15,1.0,1);
	std::cout << "Size of beliefs is (4) " << beliefs->_size  << "\n";

	// should display a diagonal
	print_belief_map(beliefs);


	delete beliefs;


	beliefs = new BMap();

	funcSensor sensors[NUMSNSRS];

	std::cout << "Loading Sensor Array\n";

	std::cout << "------------------------------------\n";
	std::cout << "Test all in range orientation 0\n";
	std::cout << "------------------------------------\n";

	sensors[S0] = &read_within_range;
	sensors[S90] = &read_within_range;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_within_range;
	sensors[T0] = &read_within_range;
	sensors[OR0] = &read_orientation_is_0;

	StrategyFactory* sf = new StrategyFactory();
	IStrategy* s = sf->makeStrategy(StrategyTypes::COLLISION_AVOIDANCE);

	Action* action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 0);
	std::cout << "Agent should MOVE 1 m 10 " << action->c << " " << action->m  << "\n";

	delete action;

	std::cout << "------------------------------------\n";
	std::cout << "Test all will collide but 180 orientation 0\n";
	std::cout << "------------------------------------\n";

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_will_collide;
	
	action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 100);
	std::cout << "Agent should TURN 0 m 180 " << action->c << " " << action->m  << "\n";

        delete action;

	std::cout << "------------------------------------\n";
	std::cout << "Test all will collide but 270 orientation 0\n";
	std::cout << "------------------------------------\n";

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_within_range;
	
	action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 100);
	std::cout << "Agent should TURN 0 m 270 " << action->c << " " << action->m  << "\n";

        delete action;

	std::cout << "------------------------------------\n";
	std::cout << "Test all will collide orientation 0\n";
	std::cout << "------------------------------------\n";

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_will_collide;
	
	action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 0);
	std::cout << "Agent should NOP 2 m 0 " << action->c << " " << action->m  << "\n";

	std::cout << "------------------------------------\n";
	std::cout << "Test all will collide but 270 orientation 270 SHOULD MOVE \n";
	std::cout << "------------------------------------\n";

	sensors[S0] = &read_will_collide;
	sensors[S90] = &read_will_collide;
	sensors[S180] = &read_will_collide;
	sensors[S270] = &read_within_range;
	sensors[OR0] = &read_orientation_is_270;

	action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 0);
	std::cout << "Agent should MOVE 1 m 10 " << action->c << " " << action->m  << "\n";

        delete action;

	std::cout << "---------------------------------------------\n";
	std::cout << "Test Working Memory\n";
	std::cout << "---------------------------------------------\n";
	std::cout << "Simulating moving toward obstacle bearing 0 degrees\n";
	
	WorkingMemory* wm = new WorkingMemory();

	sensors[S0] = &read_at_range_100;
	sensors[S90] = &read_within_range;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_within_range;
	sensors[T0] = &read_within_range;
	sensors[OR0] = &read_orientation_is_0;

	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs

	sensors[S0] = &read_at_range_90;
	agent_take_reading(sensors, wm, ActionTypes::MOVE); // move 

	sensors[S0] = &read_at_range_80;
	agent_take_reading(sensors, wm, ActionTypes::MOVE); // move

	sensors[S0] = &read_at_range_70;
	agent_take_reading(sensors, wm, ActionTypes::MOVE); // move
	
	sensors[S0] = &read_at_range_70;
	agent_take_reading(sensors, wm, ActionTypes::NOP); // stopped 
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	
	std::cout << "generating belief map from working memory\n";

	beliefs = wm->MaterializeWorld();	
	
	std::cout << "print should show a single point\n";
	print_belief_map(beliefs);
	std::cout << "print should show a single point\n";

	// DEBUG - fix the leak
	//delete wm;
	delete beliefs;

	std::cout << "Simulating moving toward obstacle bearing 0 degrees with objects on all sides\n";
	
	wm = new WorkingMemory();

	sensors[S0] = &read_at_range_40;
	sensors[S90] = &read_at_range_15;
	sensors[S180] = &read_beyond_range;
	sensors[S270] = &read_at_range_15;
	sensors[T0] = &read_at_range_15;
	sensors[OR0] = &read_orientation_is_0;

	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm, ActionTypes::NOP); // start with NOPs

	sensors[S0] = &read_at_range_30;
	agent_take_reading(sensors, wm, ActionTypes::MOVE); // move 

	sensors[S0] = &read_at_range_20;
	agent_take_reading(sensors, wm, ActionTypes::MOVE); // move

	sensors[S0] = &read_at_range_15;
	agent_take_reading(sensors, wm, ActionTypes::MOVE); // move
	
	std::cout << "generating belief map from working memory\n";

	beliefs = wm->MaterializeWorld();	
	
	std::cout << "print should show a single point at bearing 0 with a tunnel on all sides\n";
	print_belief_map(beliefs);
	std::cout << "print should show a single point at bearing 0 with a tunnel on all sides\n";

	//delete wm;
	delete beliefs;

	std::cout << "print should show a tunnel\n";

	delete s;


	std::cout << "---------------------------------------------\n";
	std::cout << "Test Highest Yielding Strategy\n";
	std::cout << "---------------------------------------------\n";
	
	
	s = sf->makeStrategy(StrategyTypes::HIGHEST_YIELD);

	sensors[S0] = &read_within_range;
	sensors[S90] = &read_within_range;
	sensors[S180] = &read_within_range;
	sensors[S270] = &read_within_range;
	
	action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 0);
	std::cout << "Agent should MOVE 1 m 10 " << action->c << " " << action->m  << "\n";

	delete action;

	WorkingMemory* wm2;

	wm2 = new WorkingMemory();

	sensors[S0] = &read_at_range_40;
	sensors[S90] = &read_at_range_15;
	sensors[S180] = &read_beyond_range;
	sensors[S270] = &read_at_range_15;
	sensors[T0] = &read_at_range_15;
	sensors[OR0] = &read_orientation_is_0;

	agent_take_reading(sensors, wm2, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm2, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm2, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm2, ActionTypes::NOP); // start with NOPs
	agent_take_reading(sensors, wm2, ActionTypes::NOP); // start with NOPs

	sensors[S0] = &read_at_range_30;
	agent_take_reading(sensors, wm2, ActionTypes::MOVE); // move 

	sensors[S0] = &read_at_range_20;
	agent_take_reading(sensors, wm2, ActionTypes::MOVE); // move

	sensors[S0] = &read_at_range_15;
	agent_take_reading(sensors, wm2, ActionTypes::MOVE); // move
	
	std::cout << "generating belief map from working memory\n";

	beliefs = wm2->MaterializeWorld();	
	
	std::cout << "print should show a single point at bearing 0 with a tunnel on all sides\n";
	print_belief_map(beliefs);
	std::cout << "print should show a single point at bearing 0 with a tunnel on all sides\n";

	action = s->getHighestYieldingAction(sensors, beliefs, 0, 0, 0, 0);
	std::cout << "Agent should MOVE 1 m 10 " << action->c << " " << action->m  << "\n";

	delete s;
	delete sf;

}

int main() {
	RunDiagnostic();
}
