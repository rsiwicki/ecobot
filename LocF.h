#ifndef LOCF_H
#define LOCF_H

#include "ActionTypes.h"

class LocF {
	public:
		short int s0;
		short int s90;
		short int s180;		
		short int s270;
		short int temp_1;
		short int ori;
		enum ActionTypes::Enum precact;
};

#endif
