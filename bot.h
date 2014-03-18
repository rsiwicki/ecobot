#include "thresholds.h"
#include <time.h>
/****************************
 *
 * Very basic C program to control the bot.
 * Keeping everything simple using simple C constructs
 * as we have to make this run on a microcontroller.
 *
 * Rob Siwicki @ ComparetheMarket.com
 *
 * *******************************/



enum acts {TURN,MOVE,NOP,NUMACTS};

#if ARDUINO == 1

typedef int time_t;

#endif

int currentOrientation = 0;
int currentX = 0;
int currentY = 0;


typedef int (*funcSensor) (int);
typedef enum acts (*funcStrategy) (struct Map*, int, int, int);




struct Actn {
    enum acts c;
    int m;
};

struct Wm {
    struct LocF* facts[MAX_POINTS];
    int size;
};

struct vct {
    int x;
    int y;
};

struct LocF {
    time_t t0;
    int s0; // MAX range is 300
    int s90;
    int s180;
    int s270;
    int temp_1;
    int ori;
    enum acts precact;
};	

struct Hn {
    int x;
    int y;
    time_t t;
    float temp_1;
    int is_obstacle; // consider uchar bit
};

struct Map {
    struct Hn* points[MAX_POINTS];
    int size;
    int maxdimx;
    int maxdimy;
};


