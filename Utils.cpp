#include "Utils.h"



Vct* Utils::lvfp(int d, int m) {

	d = d % 360;

	Vct* v = new Vct();
	v->x = 0;
	v->y = 0;

	if(d>=0&&d<=90) {
            double r = (90 - d) / (180/PI);
	    v->x = m * cos(r);
	    v->y = m * sin(r);
	}
	else if(d>=90&&d<=180) {
            double r = (d - 90) / (180/PI);
	    v->x = m * cos(r);
	    v->y = m * sin(r) * -1;
	}
	else if(d>=180&&d<=270) {
            double r = (270 - d) / (180/PI);
	    v->x = m * cos(r) * -1;
	    v->y = m * sin(r) * -1;
	}
	else if(d>=270&&d<=360) {
            double r = (d - 270) / (180/PI);
	    v->x = m * cos(r) * -1;
	    v->y = m * sin(r);
	}

	return v;

}
unsigned int Utils::pos(int n) {
	int const mask = n >> sizeof(int) * BITS -1;
	unsigned int r = (n + mask) ^ mask;
	return (unsigned) r;
}
