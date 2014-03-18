#ifndef ACTION_H
#define ACTION_H

#include "ActionTypes.h"

class Action {
	public:
		ActionTypes::Enum c;
		int m;
		Action() { m = 0; }
		~Action() {;}

};

#endif
