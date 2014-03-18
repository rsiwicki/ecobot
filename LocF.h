#ifndef LOCF_H
#define LOCF_H

#include <time.h>
#include "ActionTypes.h"

#if ARDUINO == 1

typedef int time_t;

#endif


class LocF {
	public:
		time_t t0;
		short int s0;
		short int s90;
		short int s180;		
		short int s270;
		short int temp_1;
		short int ori;
		enum ActionTypes::Enum precact;
};

#endif
