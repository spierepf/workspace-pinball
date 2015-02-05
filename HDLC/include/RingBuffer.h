/*
 * RingBuffer.h
 *
 *  Created on: Jan 29, 2015
 *      Author: peter
 */

#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_

#include <stdint.h>

template<uint8_t SIZE> // SIZE must be a power of 2
class RingBuffer {
	uint8_t buf[SIZE];
	uint8_t count;
	uint8_t head;
	uint8_t tail;

	uint8_t modulo_add(uint8_t a, uint8_t b) {
		return (a + b) & (SIZE - 1);
	}

public:
	RingBuffer() {
		count = head = tail = 0;
	}

	bool empty() {
		return 0 == count;
	}

	bool full() {
		return SIZE == count;
	}

	void put(uint8_t b) {
		if(!full()) {
			buf[head] = b;
			head = modulo_add(head, 1);
			count++;
		}
	}

	uint8_t get() {
		uint8_t b = buf[tail];
		if(!empty()) {
			tail = modulo_add(tail, 1);
			count--;
		}
		return b;
	}

	uint8_t peek(uint8_t position) {
		return buf[modulo_add(tail, position)];
	}

	void consume(uint8_t amount) {
		if(count >= amount) {
			tail = modulo_add(tail, amount);
			count -= amount;
		}
	}
};

#endif /* RINGBUFFER_H_ */
