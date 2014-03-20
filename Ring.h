#ifndef RING_H
#define RING_H

#include <iostream>

template<typename T> class Ring {
	private:
		int _k;
		int _head;
		T** _queue;

	public:
		Ring(int k) { 
			_k = k; 
			_head=0;
			_queue = new T*[k];
		}
		
		~Ring() {
			std::cout << _head << "\n";
			delete [] _queue;
		}
		
		void Enqueue(T* item) {
			std::cout << "Enqueueing \n";
			if(_head >= _k) {
				std::cout << "Overflowing\n";
				delete _queue[_head % _k];
			}
			_queue[_head % _k] = item;
			_head++;
		}

		int GetSize() {
			if(_head < _k) {
				return _head;
			};
			
			return _k;	
		}

		T* GetLatest() {
			if(_head > 1) {
				return _queue[(_head-1) % _k];
			}
		}

		T* GetLatestMinus(int delta) {
			int offset = delta + 1;
			if(_head - offset >= 0) {
				return _queue[(_head-offset) % _k];
			}
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
