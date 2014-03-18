#ifndef BPQPOINT_H
#define BPQPOINT_H


class BPQPoint {
	private:
		int _x;
		int _y;
		int _p;
	public:
		int getX() {return _x;}
		int getY() {return _y;}
		int getP() {return _p;}
		
		int setX(int x) {_x =x;} 
		int setY(int y) {_y =y;} 
		int setP(int p) {_p =p;} 

};

#endif
