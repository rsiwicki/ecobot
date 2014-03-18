#ifndef BPQ_H
#define BPQ_H

#include "BPQPoint.h"

class BPQ {
	private:
		int _size;
		int _k;
		BPQPoint** _values;
	public:
		BPQ(int k);
		~BPQ();

		int getSize() {return _size;}

		BPQPoint* getValue(int index);
		BPQPoint* max_priority_elem();
		BPQPoint* min_priority_elem();

		void enqueue(BPQPoint* point);		
		
		int is_full() {return _size == _k;}
};

#endif
