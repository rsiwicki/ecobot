#include <iostream>
#include "Hn.h"
#include "Ring.h"

int main() {

	std::cout << "In TestRing \n";

	Ring<Hn>* ring = new Ring<Hn>(5);

	for(int i =0; i < 10 ; i++) {

		Hn* hn1 = new Hn();
		hn1->x = i;
		hn1->y = i;

		ring->Enqueue(hn1);
		std::cout << "Size is " << ring->GetSize() << "\n";
	}


	Hn* hnr = ring->GetLatest();

	std::cout << "Latest " << hnr->x << "\n"; 

	Hn* hnr0 = ring->GetLatestMinus(0);
	std::cout << "Minus 0 " << hnr0->x << "\n";
	
	Hn* hnr1 = ring->GetLatestMinus(1);
	std::cout << "Minus 1 " << hnr1->x << "\n";

	Hn* hnr2 = ring->GetLatestMinus(2);
	std::cout << "Minus 2 " << hnr2->x << "\n";
	
	Hn* hnr3 = ring->GetLatestMinus(3);
	std::cout << "Minus 3 " << hnr3->x << "\n";
	
	Hn* hnr4 = ring->GetLatestMinus(4);
	std::cout << "Minus 4 " << hnr4->x << "\n";
	
	Hn* hnr5 = ring->GetLatestMinus(5);
	std::cout << "Minus 5 " << hnr5->x << "\n";

	Hn* hnr6 = ring->GetLatestMinus(6);
	std::cout << "Minus 6 " << hnr6->x << "\n";

	Hn* hnr7 = ring->GetLatestMinus(7);
	std::cout << "Minus 7 " << hnr7->x << "\n";
	
	delete ring;

}
