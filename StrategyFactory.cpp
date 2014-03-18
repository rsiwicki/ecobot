#include "StrategyFactory.h"
#include "YieldStrategy.h"
#include "CollisionAvoidanceStrategy.h"

IStrategy* StrategyFactory::makeStrategy(StrategyTypes::Enum selector) {

	IStrategy* s = NULL;
	
	switch (selector) {
		case StrategyTypes::HIGHEST_YIELD:
			s = new YieldStrategy();
		default:
			s = new CollisionAvoidanceStrategy();
	}

	return s;
};
