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
	}


	Hn* hnr = ring->GetLatest();

	std::cout << hnr->x << "\n"; 


	delete ring;

}
