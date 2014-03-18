#include <iostream>
#include "IStrategy.h"
#include "sensors.h"
#include "BMap.h"
#include "Action.h"

class CollisionAvoidanceStrategy : public IStrategy {

	public:
		Action* getHighestYieldingAction(const funcSensor* sensors,BMap* map, int curOr, int curXBel, int curYBel, int tProb);

};
