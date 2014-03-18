#ifndef ISTRATEGY_H
#define ISTRATEGY_H

#include "BMap.h"
#include "sensors.h"
#include "Action.h"

class IStrategy {
	public:
//Actn collision_avoidance_str(struct Map* beliefMap, int curOr, int curXBel, int curYBel, int tProb) 

		virtual Action* getHighestYieldingAction(const funcSensor* sensors,BMap* map, int curOr, int curXBel, int curYBel, int tProb) =0;

};

#endif // ISTRATEGY_H
