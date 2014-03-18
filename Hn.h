#ifndef HN_H
#define HN_H

#include <time.h>

#if ARDUINO == 1

typedef int time_t;

#endif

class Hn {
	public:	
		short int x;
		short int y;
		time_t t;
		float temp_1;
		short int is_obstacle;
};

#endif
