template<class T> Ring<T>::Ring(int k) {
	_k = k;
	_head = 0;
	T* _queue = new T[k](); 
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
}
