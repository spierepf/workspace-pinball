//============================================================================
// Name        : RunLengthEncoder.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>

#include <rle.h>


int main() {
	#include <tmp.h>

	for(uint i = 0; i < length; i++) {
		if(i % 16 == 0) printf("\n\t");
		printf("0x%02x", buf[i]);
		if(i < length - 1) printf(", ");
	}
	printf("\n};\n");

	return 0;
}
