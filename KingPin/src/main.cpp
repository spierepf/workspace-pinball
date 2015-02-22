/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Application.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <list>

using namespace std;

#define FIFO_NAME "/tmp/american_maid"

list<Application*> applications;

void setup() {
}

void loop() {
	char s[300];
	int num, fd;

	for(list<Application*>::iterator i=applications.begin(); i != applications.end(); ++i)
		(*i) -> schedule();

	if((num = access(FIFO_NAME, R_OK)) != -1) {
		fd = open(FIFO_NAME, O_RDONLY);
		if(fd != -1) {
			if ((num = read(fd, s, sizeof(s))) != -1) {
				if(num != 0) {
					s[num] = '\0';
					printf("%s\n", s);
					try {
						Hardware* hardware = new Tty(s);
						applications.push_back(new Application(hardware));
					} catch(SerialPort::OpenFailed e) {
						printf("message: %s\n", e.what());
					}
				}
			} else {
				perror("read");
			}
			close(fd);
		}
	}
}

int main(int argc, char **argv) {
	setup();
	for(;;) loop();
}
