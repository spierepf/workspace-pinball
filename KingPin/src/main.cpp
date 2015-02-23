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

#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

list<Application*> applications;
FILE* fifo;

void setup() {
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
	fifo = fopen(FIFO_NAME, "r");
}

void loop() {
	char s[300];

	for(list<Application*>::iterator i=applications.begin(); i != applications.end(); ++i)
		(*i) -> schedule();

	if(fgets(s, sizeof(s), fifo) != NULL) {
		char *end = s + strlen(s) - 1;
		while(end > s && isspace(*end)) end--;
		*(end+1) = 0;

		try {
			Hardware* hardware = new Tty(s);
			applications.push_back(new Application(hardware));
			LOG(INFO) << "Pending connection to " << s;
		} catch(SerialPort::OpenFailed& e) {
			LOG(ERROR) << "While opening " << s << " : " << e.what();
		}
	}
}

int main(int argc, char **argv) {
	START_EASYLOGGINGPP(argc, argv);
	setup();
	for(;;) loop();
}
