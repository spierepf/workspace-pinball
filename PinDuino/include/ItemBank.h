/*
 * ItemBank.h
 *
 *  Created on: Jun 20, 2015
 *      Author: peter
 */

#ifndef ITEMBANK_H_
#define ITEMBANK_H_

#include <Item.h>

#include <pt.h>
#include <stdlib.h>

template<class Derived>
class ItemBank {
	struct pt pt;

	Item** items;

	PT_THREAD(run()) {
		PT_BEGIN(&pt);
		for(;;) {
			updateSelf();
			if(dirtyList) {
				for(uint8_t i = 0; i < 8; i++) {
					if((items[i] != NULL) && ((dirtyList & items[i]->mask) != 0)) {
						updateItem(items[i]);
					}
				}
			}
			PT_YIELD(&pt);
		}
		PT_END(&pt);
	}

protected:
	uint8_t& dirtyList;

	void updateSelf() { static_cast<Derived*>(this)->updateSelf_impl(); }
	void updateItem(Item* item) { static_cast<Derived*>(this)->updateItem_impl(item); }

	// making this protected prevents deleting an ItemBank* directly
	~ItemBank() {}

public:
	ItemBank(Item** items, uint8_t& dirtyList) : items(items), dirtyList(dirtyList) { PT_INIT(&pt); }

	void schedule() { PT_SCHEDULE(run()); }
};

#endif /* ITEMBANK_H_ */
