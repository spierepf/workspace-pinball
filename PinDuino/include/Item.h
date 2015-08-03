/*
 * Item.h
 *
 *  Created on: Jun 20, 2015
 *      Author: peter
 */

#ifndef ITEM_H_
#define ITEM_H_

#include <stdint.h>

class Item {
	friend class ItemBank;

	uint8_t& dirtyList;

protected:
	const uint8_t mask;

	void dirty() { dirtyList |= mask; }
	void clean() { dirtyList &= ~mask; }
	bool isDirty() { return dirtyList & mask; }

public:
	Item(const uint8_t mask, uint8_t& dirtyList) : dirtyList(dirtyList), mask(mask) {}
	virtual ~Item() {};
};

#endif /* ITEM_H_ */