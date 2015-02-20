/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Application.h>

Application application1("/dev/ttyACM0");

void setup() {
}

void loop() {
	application1.schedule();
}

int main(int argc, char **argv) {
	setup();
	for(;;) loop();
}
