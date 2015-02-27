//============================================================================
// Name        : KingPinRegistraar.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/american_maid"

int main(int argc, char** argv) {
	FILE* fifo;

    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    fifo = fopen(FIFO_NAME, "a");
    fprintf(fifo, "%s\n", argv[1]);
    fclose(fifo);
}
