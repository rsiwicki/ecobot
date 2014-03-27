#include <iostream>
#include "IStrategy.h"
#include "sensors.h"
#include "BMap.h"
#include "Action.h"
#include "Utils.h"

class YieldStrategy : public IStrategy {

	private:
		int YieldFunction1(BMap* beliefMap, int* viableDirections, int viableDirectionsSize);

	public:
		Action* getHighestYieldingAction(const funcSensor* sensors,BMap* map, int curOr, int curXBel, int curYBel, int tProb);

};
