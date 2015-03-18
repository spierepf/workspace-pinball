/*
 * main.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <eeprom.h>

#include <DataLink.h>
#include <SolenoidAction.h>

const uint8_t id = 5;

void setup() {
	uint32_t defaultAction = SolenoidAction();

	pinMode(13, OUTPUT);
	for(int i = 0; i < 12; i++) {
		eeprom_busy_wait();
		eeprom_update_dword(&eeprom_actions[i][0], defaultAction);
		eeprom_busy_wait();
		eeprom_update_dword(&eeprom_actions[i][1], defaultAction);
	}
	eeprom_busy_wait();
	eeprom_update_byte(&eeprom_id, id);
	eeprom_busy_wait();
	digitalWrite(13, eeprom_read_byte(&eeprom_id) == id);
}

void loop() {

}
