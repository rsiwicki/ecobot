#include "Hn.h"
#include "thresholds.h"

class BMap {
	public:	
		Hn* _points[MAX_POINTS];
		int _size;
		int _maxdimx;
		int _maxdimy;
	
		void AddBelief(int x, int y, float temp, int obs);

};
