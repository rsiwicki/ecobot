#ifndef BMAP_H
#define BMAP_H

#include "Hn.h"
#include "thresholds.h"

class BMap {
	private:
		Hn* _points[MAX_POINTS];
	public:	
		short int _size;
		short int _maxdimx;
		short int _maxdimy;
		short int _curx;
		short int _cury;

		void AddBelief(short int x, short int y, short int temp, short int obs);

		Hn* GetPoint(int i);	

		BMap() {;}
		~BMap() { 
		
			for(int i=0; i< _size; i++) {
				delete _points[i];
			}
		}
};

#endif
