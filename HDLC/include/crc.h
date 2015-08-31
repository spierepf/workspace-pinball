/*
 * crc.h
 *
 *  Created on: Aug 30, 2015
 *      Author: peter
 */

#ifndef INCLUDE_CRC_H_
#define INCLUDE_CRC_H_

#include <stdint.h>

void crc_ccitt_update (uint16_t& crc, uint8_t data);

#endif /* INCLUDE_CRC_H_ */
