/*
 * EndPointManager.cpp
 *
 *  Created on: Mar 20, 2015
 *      Author: peter
 */

#include "EndPointManager.h"

#include "IncomingDataLink.h"
#include "OutgoingDataLink.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <gtkmm.h>
#include <easylogging++.h>
extern Gtk::Notebook* notebook;

#define FIFO_NAME "/tmp/american_maid"

EndPointManager::EndPointManager() {
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

	for(list<EndPoint*>::iterator i=endPoints.begin(); i != endPoints.end(); ++i)
		(*i) -> schedule();

	if(fgets(s, sizeof(s), fifo) != NULL) {
		char *end = s + strlen(s) - 1;
		while(end > s && isspace(*end)) end--;
		*(end+1) = 0;

		try {
			Tty* hardware = new Tty(s);
			ByteSource* byteSource = hardware;
			ByteSink* byteSink = hardware;
			IncomingDataLink* incomingDatalink = new IncomingDataLink(*byteSource);
			OutgoingDataLink* outgoingDatalink = new OutgoingDataLink(*byteSink);
			endPoints.push_back(new EndPoint(notebook, s, byteSource, byteSink, incomingDatalink, outgoingDatalink));
			LOG(INFO) << "Pending connection to " << s;
		} catch(SerialPort::OpenFailed& e) {
			LOG(ERROR) << "While opening " << s << " : " << e.what();
		}
	}

	return true;
}
