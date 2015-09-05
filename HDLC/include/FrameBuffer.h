/*
 * FrameBuffer.h
 *
 *  Created on: Sep 2, 2015
 *      Author: peter
 */

#ifndef INCLUDE_FRAMEBUFFER_H_
#define INCLUDE_FRAMEBUFFER_H_

#include <stdlib.h>

#include <RingBuffer.h>

template<uint8_t SIZE, uint8_t FRAME_COUNT> // must be powers of 2
class FrameBuffer {
	uint8_t buf[SIZE];
	uint8_t head;
	RingBuffer<FRAME_COUNT> tails;

	static uint8_t modulo_add(uint8_t a, uint8_t b) {
		return (a + b) & (SIZE - 1);
	}

public:
	class Frame {
		FrameBuffer* owner;
		uint8_t tail;
		uint8_t length;

	public:
		Frame() : owner(NULL), tail(0), length(0) {
		}

		Frame(FrameBuffer* owner, uint8_t tail, uint8_t length) : owner(owner), tail(tail), length(length) {
		}

		uint8_t getLength() const {
			return length;
		}

		uint8_t operator[](uint8_t i) const {
			return owner->buf[modulo_add(tail, i)];
		}
	};

	FrameBuffer() : head(0) {
		tails.put(head);
	}

	void endFrame() {
		tails.put(head);
	}

	bool hasFrame() {
		return getFrameCount() > 0;
	}

	uint8_t getFrameCount() {
		return tails.getCount() - 1;
	}

	void removeFrame() {
		tails.get();
	}

	const Frame operator[](uint8_t i) {
		return Frame(this, tails.peek(i), modulo_add(tails.peek(i + 1), -tails.peek(i)));
	}

	void put(uint8_t b) {
		buf[head] = b;
		head = modulo_add(head, 1);
	}

	void revert(uint8_t count) {
		head = modulo_add(head, -count);
	}

	void revertFrame() {
		head = tails.peek(getFrameCount());
	}
};

#endif /* INCLUDE_FRAMEBUFFER_H_ */
