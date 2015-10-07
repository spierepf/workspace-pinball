/*
 * main.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: peter
 */

#include <Arduino.h>

#include <eeprom.h>

const uint8_t id = 5;

void setup() {
	pinMode(13, OUTPUT);
	eeprom_busy_wait();
	eeprom_update_byte(&eeprom_id, id);
	eeprom_busy_wait();
}

void loop() {
	digitalWrite(13, eeprom_read_byte(&eeprom_id) == id);
	delay(1000);
	digitalWrite(13, LOW);
	delay(1000);
}
