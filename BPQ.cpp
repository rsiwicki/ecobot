#include "stdlib.h"
#include "BPQ.h"
#include "BPQPoint.h"
#include <iostream>

BPQ::BPQ(int k) {
	// alloc array
	_k = k;
	_values = new BPQPoint*[k];
	_size = 0;
}

BPQ::~BPQ() {

	int i;
	
	delete [] _values;

	_size = 0;

}

BPQPoint* BPQ::getValue(int index) {

	// DEBUG -- if not OOB or throws
	return _values[index];

}


void BPQ::enqueue(BPQPoint* point) {

	std::cout << "queueing \n";

	if(getSize() < _k) {
		std::cout << "adding new bp\n";
		_size++;
		std::cout << "size " << _size << "\n";
		_values[_size-1]=point;
	} else {

		std::cout << "overflowing queue - adjust priority\n";
		//remove highest value n as it represents the lowest priority n
		int max = 0;
		int pos = -1;
		int i;
		
		for(i=0; i<_size;i++){
			int cp = _values[i]->getP();
			if(cp > max) {
				max = cp;
				pos = i;	
			}	
		}

		// now pos is the minimum
		//printf("max is %d at pos %d\n", max, pos);

		if(point->getP() < max && pos > -1) {
			delete _values[pos];
			_values[pos] = point;
		}
	}
}

BPQPoint* BPQ::max_priority_elem() {
	
		int min = 9999999;
		int pos = 0;

		int i;
		
		for(i=0; i<_size;i++ ){
		
			int cp = _values[i]->getP();
			if(cp < min) {
				min = cp;
				pos = i;	
			}	
		}

		return _values[pos];

}

BPQPoint* BPQ::min_priority_elem() {
	
		int min = -99999;
		int pos = 0;

		int i;
		
		for(i=0; i<_size;i++ ){
			int cp = _values[i]->getP();
			if(cp > min) {
				min = cp;
				pos = i;	
			}	
		}


		return _values[pos];

}



