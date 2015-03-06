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
			applications.push_back(new Application(s, hardware));
			LOG(INFO) << "Pending connection to " << s;
		} catch(SerialPort::OpenFailed& e) {
			LOG(ERROR) << "While opening " << s << " : " << e.what();
		}
	}
}

class Worker {
public:

	Worker() : thread(0), stop(false) {}

	// Called to start the processing on the thread
	void start () {
		thread = Glib::Thread::create(sigc::mem_fun(*this, &Worker::run), true);
	}

	// When shutting down, we need to stop the thread
	~Worker() {
		{
			Glib::Mutex::Lock lock (mutex);
			stop = true;
		}
		if (thread)
			thread->join(); // Here we block to truly wait for the thread to complete
	}

	Glib::Dispatcher sig_done;

protected:
	// This is where the real work happens
	void run () {
		setup();
		for(;;) {
			{
				Glib::Mutex::Lock lock (mutex);
				if (stop) break;
			}
			loop();
		}
	}

	Glib::Thread * thread;
	Glib::Mutex mutex;
	bool stop;
};

int main(int argc, char **argv) {
	START_EASYLOGGINGPP(argc, argv);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	if(!Glib::thread_supported()) Glib::thread_init();

	Gtk::Window window;
	window.set_default_size(200, 200);

	Worker bee;
	bee.start();

	return app->run(window);
}
