/*
 * Timer.h
 *
 *  Created on: Feb 2, 2015
 *      Author: peter
 */

#ifndef TIMER_H_
#define TIMER_H_

template<class T>
class Timer {
	T t0;

public:
	void set(T t) {
		t0 = t;
	}

	T elapsed(T t) {
		return t - t0;
	}
};



#endif /* TIMER_H_ */
