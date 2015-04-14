/*
 * rle.h
 *
 *  Created on: Apr 13, 2015
 *      Author: peter
 */

#ifndef INCLUDE_RLE_H_
#define INCLUDE_RLE_H_

#include <stdint.h>
#include <stdlib.h>

size_t rle_compress(uint8_t[], size_t, uint8_t[]);
size_t rle_decompress(const uint8_t[], size_t, uint8_t[]);

#endif /* INCLUDE_RLE_H_ */
