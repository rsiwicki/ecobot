#include "BMap.h"
#include <iostream>

void BMap::AddBelief(short int x, short int y, short int temp, short int obs) {

    if(_size < MAX_POINTS) {	
    	Hn* point = new Hn();

    	point->x=x;
    	point->y=y;
    	point->temp_1=temp;
    	point->is_obstacle=obs;

    	int s=_size;

    	_points[s] = point; 
    	_maxdimx = x > _maxdimx ? x : _maxdimx;
    	_maxdimy = y > _maxdimy ? y : _maxdimy;
    	_size++;
    } else {
	// we cannot add anymore points due to memory pressure (perhaps!)
	// need to either offload distant points to wifi
	// or 
	// reduce using KNN
	std::cout << "WARNING - need to reduce the map using KNN\n";	
    }
}

