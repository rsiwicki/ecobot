#include <iostream>
#include "BPQ.h"

int main() {

	std::cout << "in main\n";

	int i;
	int k = 5;


	BPQ* bpq = new BPQ(k);

	for(i=100;i<(100+k);i++) {
		BPQPoint* p1 = new BPQPoint();
		p1->setX(i);
		p1->setY(0);
		p1->setP(i);

		bpq->enqueue(p1);
	}

	BPQPoint* pout = bpq->max_priority_elem(); 

	std::cout << "highest priority elem should be nearest (100) " << pout->getP() << "\n";

	for(i=7;i<20;i++) {
		BPQPoint* p1 = new BPQPoint();
		p1->setX(i);
		p1->setY(0);
		p1->setP(i);

		bpq->enqueue(p1);
	}

	BPQPoint* pout2 = bpq->max_priority_elem(); 

	std::cout << "highest priority elem after overflow should be nearest (7) " << pout2->getP() << "\n";

	for(i=2;i<720;i++) {
		BPQPoint* p1 = new BPQPoint();
		p1->setX(i);
		p1->setY(0);
		p1->setP(i);

		bpq->enqueue(p1);
	}

	BPQPoint* pout7 = bpq->max_priority_elem(); 

	std::cout << "highest priority elem after overflow should be nearest (2) " << pout7->getP() << "\n";


}
