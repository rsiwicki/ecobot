#include <iostream>
#include "StrategyFactory.h"

using namespace std;

int main() {

	cout << "in main " << "\n";

	StrategyFactory* sf = new StrategyFactory();
	
	IStrategy* s = sf->makeStrategy(1);

	s->getHighestYieldingAction();

	return 0;
}
