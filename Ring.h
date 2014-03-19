#ifndef RING_H
#define RING_H

#include <iostream>

template<typename T> class Ring {
	private:
		int _k;
		int _head;
		T* _queue[];

	public:
		Ring(int k) { _k = k; _head=0;}
		
		~Ring() {
			delete [] _queue;
		}
		
		void Enqueue(T* item) {
			std::cout << "Enqueing \n";
			if(_head >= _k) {
				std::cout << "Overflowing\n";
				delete _queue[_head % _k];
			}
			_queue[_head % _k] = item;
			_head++;
		}


};
/*
template<class T> Ring<T>::Ring(int k) {
	_k = k;
	_head = 0;
	T* _queue = new T[k]; 
}

template<class T> Ring<T>::~Ring() {
	delete [] _queue;
}

template<class T> void Ring<T>::Enqueue(const T* item) {
	if(_head >= _k) {
		delete _queue[_head % _k];
	}
	_queue[_head % _k] = item;
	_head++;
}*/

#endif
