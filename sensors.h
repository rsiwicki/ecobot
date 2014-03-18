#ifndef SENSORS_H
#define SENSORS_H

#include "thresholds.h"
#include "ActionTypes.h"
#include "BMap.h"

typedef int (*funcSensor) (int);
typedef enum ActionTypes::Enum (*funcStrategy) (BMap*, int, int, int);

enum snsrs {S0,S90,S180,S270,OR0,T0,TEMP0,NUMSNSRS};

int read_beyond_range(int opt);

int read_within_range(int opt);

int read_will_collide(int opt);

int read_orientation_is_0(int opt);

int read_orientation_is_90(int opt);

int read_orientation_is_180(int opt);

int read_orientation_is_270(int opt);

int read_at_range_100(int opt);

int read_at_range_90(int opt);

int read_at_range_80(int opt);

int read_at_range_70(int opt);

int read_at_range_15(int opt);

#if ARDUINO == 1

int read_0_sonic_real(int opt); 

int read_90_sonic_real(int opt); 

int read_180_sonic_real(int opt); 

int read_270_sonic_real(int opt);  

#endif

#endif
