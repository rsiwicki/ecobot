#include <iostream>
#include "WorkingMemory.h"

WorkingMemory::WorkingMemory() {
    _size = 0;
    _facts = new LocF[MAX_POINTS]; // DEBUG -- dynamic
}

LocF* WorkingMemory::GetLatestFact() {
    return &(_facts[_size-1]);
}

void WorkingMemory::AssertFact(LocF* fact) {
    
	_size++;

	_facts[_size] = (*fact);
}


BMap* WorkingMemory::MaterializeWorld() {

// need to plot all the obstacles as coordinates in the a' relative system
// at the moment they are curr v relative.

    LocF* earliestFact = &(_facts[0]); // get the first fact and chain forwards
    
    // DEBUG -- need to test previous fact here
    LocF* previousFact = &(_facts[_size-2]);

    BMap* beliefs = new BMap();

    Vct* curVct = new Vct();
    curVct->x = 0;
    curVct->y = 0;

    int i;

    for(i=0;i<_size; i++) {
	LocF* currentFact = &(_facts[i]);
	
	int delta0 = previousFact->s0 - currentFact->s0;
	int delta90 = previousFact->s90 - currentFact->s90;
	int delta180 = previousFact->s180 - currentFact->s180;
	int delta270 = previousFact->s270 - currentFact->s270;

	int dOr = previousFact->ori - currentFact->ori;

        switch(currentFact->precact) {
		case ActionTypes::TURN:
		    if(currentFact->s0 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr,currentFact->s0);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };
		   
		    if(currentFact->s90 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+90, currentFact->s90);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };

		    if(currentFact->s180 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+180, currentFact->s180);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };

		    if(currentFact->s270 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+270, currentFact->s270);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };
		break;
		case ActionTypes::MOVE:
		    // could add error correction here
		    if(Utils::pos(delta0) < 1 && Utils::pos(delta90) < 1 && Utils::pos(delta180) < 1 && Utils::pos(delta270) < 1) {
			std::cout << "no movement detected post move command.\n";	
			break;
		    }
		
		    Vct* dVct; 

		    // object should have moved closer on 0	    
		    // this means an increase in the delta
		    // e.g. if previous s0 100 and current s0 95 then we have moved forward 5
		    // at current bearing
		    // if delta0 is unavailable then we have to use delta 180 in which case
		    // the delta should be negative i.e. we have moved away from object
		    // by the amount of delta. Therefore new location vector is abs(delta)
		    // but need to add 180 degrees
		    std::cout << "delta0" <<  delta0 << "\n";

		    if(delta0 > MEASURE_TOLERANCE) {
			  dVct  = Utils::lvfp(currentFact->ori, delta0); 
			  std::cout << "Movement detected in forward sensor" << delta0 << "\n";
		    }		    
		    else if(delta180 < MEASURE_TOLERANCE) {
			  dVct  = Utils::lvfp(currentFact->ori + 180, Utils::pos(delta180)); 
			  std::cout << "Movement detected in aft sensor" << delta180 << "\n";
		    }		    
		    else {
			    std::cout << "PANIC! In dev null. You shouldn't let agent turn 90 if there is no contact on 90 or 270. You really should add that to the collision strategy.\n";
		    }		    

		    std::cout << "dVct.x is " << dVct->x << "\n";	
		    std::cout << "dVct.y is " << dVct->y << "\n";	

		    curVct->x += dVct->x;
		    curVct->y += dVct->y;
		    
		    if(currentFact->s0 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr,currentFact->s0);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };
		   
		    if(currentFact->s90 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+90, currentFact->s90);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };

		    if(currentFact->s180 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+180, currentFact->s180);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };

		    if(currentFact->s270 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+270, currentFact->s270);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };
		break;
		case ActionTypes::NOP:
		    if(currentFact->s0 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr,currentFact->s0);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };
		   
		    if(currentFact->s90 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+90, currentFact->s90);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };

		    if(currentFact->s180 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+180, currentFact->s180);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };

		    if(currentFact->s270 < MAX_RANGE) {
			    Vct* relVct = Utils::lvfp(dOr+270, currentFact->s270);
			    beliefs->AddBelief(curVct->x+relVct->x, curVct->y+relVct->y, 1.0, OBS);
		    };
		break;
     
  			std::cout << "no operation.\n";
		default:
		   	std::cout << "Unknown command.\n";
		break;
	}	

    } 

    return beliefs;

}
