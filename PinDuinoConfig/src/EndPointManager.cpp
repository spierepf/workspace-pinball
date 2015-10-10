/*
 * EndPointManager.cpp
 *
 *  Created on: Mar 20, 2015
 *      Author: peter
 */

#include "EndPointManager.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <gtkmm.h>
#include <easylogging++.h>

#include <SerialPort.h>

extern Gtk::Notebook* notebook;

#define FIFO_NAME "/tmp/american_maid"

EndPointManager::EndPointManager() : fifo(NULL) {
	// TODO Auto-generated constructor stub

}

EndPointManager::~EndPointManager() {
	// TODO Auto-generated destructor stub
}

void EndPointManager::setup() {
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
	fifo = fopen(FIFO_NAME, "w+");
	int fd = fileno(fifo);
	int flags = fcntl(fd, F_GETFL, 0) | O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
}

bool EndPointManager::loop() {
	char s[300];

	for(list<EndPointWrapper*>::iterator i=endPoints.begin(); i != endPoints.end(); ++i)
		(*i) -> schedule();

	if(fgets(s, sizeof(s), fifo) != NULL) {
		char *end = s + strlen(s) - 1;
		while(end > s && isspace(*end)) end--;
		*(end+1) = 0;

		try {
			endPoints.push_back(new EndPointWrapper(notebook, s));
			LOG(INFO) << "Pending connection to " << s;
		} catch(SerialPort::OpenFailed& e) {
			LOG(ERROR) << "While opening " << s << " : " << e.what();
		}
	}

	return true;
}
