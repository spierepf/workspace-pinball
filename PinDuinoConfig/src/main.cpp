/*
 * main.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <gtkmm.h>

#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

#include <EndPointManager.h>

EndPointManager endPointManager;

Gtk::Notebook* notebook;

int main(int argc, char **argv) {
	START_EASYLOGGINGPP(argc, argv);
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");

	if(!Glib::thread_supported()) Glib::thread_init();

	Gtk::Window window;

	notebook = new Gtk::Notebook();
	window.add(*notebook);

	endPointManager.setup();
	Glib::signal_idle().connect( sigc::mem_fun(&endPointManager, &EndPointManager::loop) );

	return app->run(window);
}
