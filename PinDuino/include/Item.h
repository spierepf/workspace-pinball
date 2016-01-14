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
	uint8_t& dirtyList;

protected:
	void dirty() { dirtyList |= mask; }
	void clean() { dirtyList &= ~mask; }
	bool isDirty() { return dirtyList & mask; }

	// making this protected prevents deleting an Item* directly
	~Item() {};

public:
	const uint8_t mask;

	Item(const uint8_t mask, uint8_t& dirtyList) : dirtyList(dirtyList), mask(mask) {}
};

#endif /* ITEM_H_ */
