#include "StrategyFactory.h"
#include "YieldStrategy.h"
#include "CollisionAvoidanceStrategy.h"

IStrategy* StrategyFactory::makeStrategy(StrategyTypes::Enum selector) {

	IStrategy* s = NULL;
	
	switch (selector) {
		case StrategyTypes::HIGHEST_YIELD:
			std::cout << "highest yeild\n";
			s = new YieldStrategy();
			break;
		default:
			s = new CollisionAvoidanceStrategy();
			break;
	}

	return s;
};
