/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Application.h>
#include <OpCode.h>

#include <easylogging++.h>

Application::Application(Gtk::Notebook* notebook, string device, Hardware* hardware) : notebook(notebook), device(device), hardware(hardware), datalink(*hardware), id(255) {
	PT_INIT(&pt);

	Gtk::Grid *child = new Gtk::Grid();
	Gtk::Label *label = new Gtk::Label(device);

	Gtk::Label *inputLabel = new Gtk::Label("Input");
	child->attach(*inputLabel, 0, 0, 1, 1);
	Gtk::Label *onLowLabel = new Gtk::Label("On Low");
	child->attach_next_to(*onLowLabel, *inputLabel, Gtk::POS_RIGHT, 6, 1);
	Gtk::Label *onHighLabel = new Gtk::Label("On High");
	child->attach_next_to(*onHighLabel, *onLowLabel, Gtk::POS_RIGHT, 6, 1);

	Gtk::Label *onLowEnableLabel = new Gtk::Label("Enable");
	child->attach(*onLowEnableLabel, 1, 1, 1, 1);
	Gtk::Label *onLowSolenoidLabel = new Gtk::Label("Solenoid");
	child->attach_next_to(*onLowSolenoidLabel, *onLowEnableLabel, Gtk::POS_RIGHT, 1, 1);
	Gtk::Label *onLowAttackLabel = new Gtk::Label("Attack");
	child->attach_next_to(*onLowAttackLabel, *onLowSolenoidLabel, Gtk::POS_RIGHT, 1, 1);
	Gtk::Label *onLowSustainLabel = new Gtk::Label("Sustain");
	child->attach_next_to(*onLowSustainLabel, *onLowAttackLabel, Gtk::POS_RIGHT, 1, 1);

	Gtk::Label *onHighEnableLabel = new Gtk::Label("Enable");
	child->attach(*onHighEnableLabel, 7, 1, 1, 1);
	Gtk::Label *onHighSolenoidLabel = new Gtk::Label("Solenoid");
	child->attach_next_to(*onHighSolenoidLabel, *onHighEnableLabel, Gtk::POS_RIGHT, 1, 1);
	Gtk::Label *onHighAttackLabel = new Gtk::Label("Attack");
	child->attach_next_to(*onHighAttackLabel, *onHighSolenoidLabel, Gtk::POS_RIGHT, 1, 1);
	Gtk::Label *onHighSustainLabel = new Gtk::Label("Sustain");
	child->attach_next_to(*onHighSustainLabel, *onHighAttackLabel, Gtk::POS_RIGHT, 1, 1);

	Gtk::Label *rowLabel = new Gtk::Label("A0");
	child->attach(*rowLabel, 0, 2, 1, 1);

	Gtk::CheckButton *enable = new Gtk::CheckButton();
	child->attach(*enable, 1, 2, 1, 1);

	Gtk::SpinButton *solenoid = new Gtk::SpinButton();
	solenoid->set_range(8, 13);
	solenoid->set_increments(1, 1);
	child->attach_next_to(*solenoid, *enable, Gtk::POS_RIGHT, 1, 1);

	Gtk::SpinButton *attack = new Gtk::SpinButton();
	attack->set_range(0, 65000);
	attack->set_increments(100, 1000);
	child->attach_next_to(*attack, *solenoid, Gtk::POS_RIGHT, 1, 1);

	Gtk::SpinButton *sustain = new Gtk::SpinButton();
	sustain->set_range(0, 255);
	sustain->set_increments(1, 10);
	child->attach_next_to(*sustain, *attack, Gtk::POS_RIGHT, 1, 1);

	Gtk::Button *reset = new Gtk::Button();
	Gtk::Image *resetImage = new Gtk::Image("sync-16.png");
	reset->set_image(*resetImage);
	child->attach_next_to(*reset, *sustain, Gtk::POS_RIGHT, 1, 1);

	Gtk::Button *apply = new Gtk::Button();
	Gtk::Image *applyImage = new Gtk::Image("write-16.png");
	apply->set_image(*applyImage);
	child->attach_next_to(*apply, *reset, Gtk::POS_RIGHT, 1, 1);

	notebook -> append_page(*child, *label);
	notebook -> show_all();
}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::schedule() {
	datalink.schedule();
	PT_SCHEDULE(run());
}

PT_THREAD(Application::run()) {
	PT_BEGIN(&pt);
	for(;;) {
		PT_WAIT_UNTIL(&pt, datalink.have_incoming_frame());

		if(datalink.peek(0) == OpCode::PING) { // ping
			datalink.begin_outgoing_frame(OpCode::PONG); // pong
			datalink.append_payload(datalink.peek(1)); // return the payload of the ping
			datalink.end_outgoing_frame();
		} else if(datalink.peek(0) == OpCode::LOG) { // log
			char buf[32];
			for(int i = 1; i < datalink.incoming_frame_length(); i++) {
				buf[i-1] = (char)datalink.peek(i);
			}
			buf[datalink.incoming_frame_length() - 1] = '\0';
			LOG(INFO) << "From " << id << ": " << buf;
		} else if(datalink.peek(0) == OpCode::MY_ID) {
			id = datalink.peek(1);
			LOG(INFO) << "Device " << device << " registered id: " << (int)id;
			datalink.begin_outgoing_frame(OpCode::SR_ENABLE);
			datalink.end_outgoing_frame();
		} else if(datalink.peek(0) == OpCode::PIN_LOW) {
			LOG(INFO) << "Pin Low: " << id << ":" << (int)datalink.peek(1);
		} else if(datalink.peek(0) == OpCode::PIN_HIGH) {
			LOG(INFO) << "Pin High: " << id << ":"  << (int)datalink.peek(1);
		}

		datalink.next_incoming_frame();
	}
	PT_END(&pt);
}
