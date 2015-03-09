/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <gtkmm.h>

#include <Application.h>

#include <stdio.h>
#include <sys/stat.h>

#include <list>

using namespace std;

#define FIFO_NAME "/tmp/american_maid"

#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

#include <fcntl.h>

list<Application*> applications;
FILE* fifo;
Gtk::Notebook* notebook;

void setup() {
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
	fifo = fopen(FIFO_NAME, "w+");
	int fd = fileno(fifo);
	int flags = fcntl(fd, F_GETFL, 0) | O_NONBLOCK;
	fcntl(fd, F_SETFL, flags);
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
			applications.push_back(new Application(notebook, s, hardware));
			LOG(INFO) << "Pending connection to " << s;
		} catch(SerialPort::OpenFailed& e) {
			LOG(ERROR) << "While opening " << s << " : " << e.what();
		}
	}
}

class IdleExample {
public:
	IdleExample() {
		Glib::signal_idle().connect( sigc::mem_fun(*this, &IdleExample::on_idle) );
	}

protected:
	bool on_idle() {
		loop();
		return true;
	}
};

int main(int argc, char **argv) {
	START_EASYLOGGINGPP(argc, argv);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	if(!Glib::thread_supported()) Glib::thread_init();

	Gtk::Window window;

	notebook = new Gtk::Notebook();
	window.add(*notebook);

	setup();
	IdleExample idleExample;

	return app->run(window);
}
