
#include "sensors.h"



int read_beyond_range(int opt) {
    return MAX_RANGE + 100;
}

int read_within_range(int opt) {
    return MAX_RANGE - 1;
}

int read_will_collide(int opt) {
    return COLLISION_THRESHOLD + STRIDE -1;
}

int read_orientation_is_0(int opt) {
    return 0;
}

int read_orientation_is_90(int opt) {
    return 90;
}

int read_orientation_is_180(int opt) {
    return 180;
}

int read_orientation_is_270(int opt) {
    return 270;
}

int read_at_range_100(int opt) {
    return 100;
}

int read_at_range_90(int opt) {
    return 90;
}

int read_at_range_80(int opt) {
    return 80;
}

int read_at_range_70(int opt) {
    return 70;
}

int read_at_range_60(int opt) {
    return 60;
}

int read_at_range_50(int opt) {
    return 50;
}

int read_at_range_40(int opt) {
    return 40;
}

int read_at_range_30(int opt) {
    return 30;
}

int read_at_range_20(int opt) {
    return 20;
}

int read_at_range_15(int opt) {
    return 15;
}

int read_at_range_dynamic(int opt) {
    return opt;
}

#if ARDUINO == 1

int read_0_sonic_real(int opt) {  
	utrasonic_espi.getReadings(es);
	int r = es.ultrasonic0; 
	return r;
}


int read_90_sonic_real(int opt) {  
	utrasonic_espi.getReadings(es);
	int r = es.ultrasonic90; 
	return r;
}


int read_180_sonic_real(int opt) {  
	utrasonic_espi.getReadings(es);
	int r = es.ultrasonic180; 
	return r;
}


int read_270_sonic_real(int opt) {  
	utrasonic_espi.getReadings(es);
	int r = es.ultrasonic270; 
	return r;
}



#endif
