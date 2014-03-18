#ifndef UTILS_H
#define UTILS_H

#include "thresholds.h"
#include "Vct.h"
#include <math.h>

class Utils {
	public:
		static unsigned int pos(int n);
		static Vct* lvfp(int d, int m);
};
#endif
