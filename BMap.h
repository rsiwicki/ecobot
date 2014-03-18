#ifndef BMAP_H
#define BMAP_H

#include "Hn.h"
#include "thresholds.h"

class BMap {
	public:	
		Hn* _points[MAX_POINTS];
		short int _size;
		short int _maxdimx;
		short int _maxdimy;
	
		void AddBelief(short int x, short int y, float temp, short int obs);

};

#endif
