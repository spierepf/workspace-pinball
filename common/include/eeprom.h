/*
 * eeprom.h
 *
 *  Created on: Mar 17, 2015
 *      Author: peter
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/eeprom.h>

#include <IncomingDataLink.h>
#include <OutgoingDataLink.h>

uint32_t EEMEM eeprom_actions[12][2];
uint8_t EEMEM eeprom_id;

#endif /* EEPROM_H_ */
