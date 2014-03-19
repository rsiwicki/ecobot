#include <iostream>
#include "Hn.h"
#include "Ring.h"

int main() {

	std::cout << "In TestRing \n";

	Ring<Hn>* ring = new Ring<Hn>(5);

	for(int i =0; i < 10 ; i++) {

		Hn* hn1 = new Hn();
		hn1->x = 1;
		hn1->y = 1;

		ring->Enqueue(hn1);
	}
}
