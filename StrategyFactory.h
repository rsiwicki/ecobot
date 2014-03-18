#include "IStrategy.h"
#include "StrategyTypes.h"

class StrategyFactory {
	private:
	public:
		//StrategyFactory();
		//~StrategyFactory();
		
		IStrategy* makeStrategy(StrategyTypes::Enum selector);
};
