/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Tty.h>
#include <DataLink.h>
#include <Application.h>

Tty hardware1("/dev/ttyACM0");
//Tty hardware2("/dev/ttyUSB0");

DataLink datalink1(hardware1);
//DataLink datalink2(hardware2);

Application application1(datalink1);
//Application application2(datalink2);

void setup() {
}

void loop() {
	application1.schedule();
	//application2.schedule();
}

int main(int argc, char **argv) {
	setup();
	for(;;) loop();
}
