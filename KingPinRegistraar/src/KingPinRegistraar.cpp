//============================================================================
// Name        : KingPinRegistraar.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/american_maid"

int main(int argc, char** argv) {
    int fd;

    mknod(FIFO_NAME, S_IFIFO | 0666, 0);

    fd = open(FIFO_NAME, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]));
    close(fd);
    unlink(FIFO_NAME);
}
