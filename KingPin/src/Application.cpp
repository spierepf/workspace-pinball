/*
 * Application.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#include <Application.h>
#include <OpCode.h>

#include <easylogging++.h>

char* labels[] = {
		"A0",
		"A1",
		"A2",
		"A3",
		"A4",
		"A5",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
};

Application::Application(Gtk::Notebook* notebook, string device, Hardware* hardware) : notebook(notebook), device(device), hardware(hardware), datalink(*hardware), id(255) {
	PT_INIT(&pt);

	grid = new Gtk::Grid();
	label = new Gtk::Label(device);

	inputLabel = new Gtk::Label("Input");
	grid->attach(*inputLabel, 0, 0, 1, 1);
	onLowLabel = new Gtk::Label("On Low");
	grid->attach_next_to(*onLowLabel, *inputLabel, Gtk::POS_RIGHT, 6, 1);
	onHighLabel = new Gtk::Label("On High");
	grid->attach_next_to(*onHighLabel, *onLowLabel, Gtk::POS_RIGHT, 6, 1);

	onLowEnableLabel = new Gtk::Label("Enable");
	grid->attach(*onLowEnableLabel, 1, 1, 1, 1);
	onLowSolenoidLabel = new Gtk::Label("Solenoid");
	grid->attach_next_to(*onLowSolenoidLabel, *onLowEnableLabel, Gtk::POS_RIGHT, 1, 1);
	onLowAttackLabel = new Gtk::Label("Attack");
	grid->attach_next_to(*onLowAttackLabel, *onLowSolenoidLabel, Gtk::POS_RIGHT, 1, 1);
	onLowSustainLabel = new Gtk::Label("Sustain");
	grid->attach_next_to(*onLowSustainLabel, *onLowAttackLabel, Gtk::POS_RIGHT, 1, 1);

	onHighEnableLabel = new Gtk::Label("Enable");
	grid->attach(*onHighEnableLabel, 7, 1, 1, 1);
	onHighSolenoidLabel = new Gtk::Label("Solenoid");
	grid->attach_next_to(*onHighSolenoidLabel, *onHighEnableLabel, Gtk::POS_RIGHT, 1, 1);
	onHighAttackLabel = new Gtk::Label("Attack");
	grid->attach_next_to(*onHighAttackLabel, *onHighSolenoidLabel, Gtk::POS_RIGHT, 1, 1);
	onHighSustainLabel = new Gtk::Label("Sustain");
	grid->attach_next_to(*onHighSustainLabel, *onHighAttackLabel, Gtk::POS_RIGHT, 1, 1);

	for(int i = 0; i < 12; i++) {
		rowLabels[i] = new Gtk::Label(labels[i]);
		grid->attach(*rowLabels[i], 0, i+2, 1, 1);
		solenoidActionControllers[i][0] = new SolenoidActionController(grid, 1, i+2);
		solenoidActionControllers[i][1] = new SolenoidActionController(grid, 7, i+2);
	}

	notebook -> append_page(*grid, *label);
	notebook -> show_all();
}

Application::~Application() {
	delete label;

	delete inputLabel;
	delete onLowLabel;
	delete onHighLabel;

	delete onLowEnableLabel;
	delete onLowSolenoidLabel;
	delete onLowAttackLabel;
	delete onLowSustainLabel;

	delete onHighEnableLabel;
	delete onHighSolenoidLabel;
	delete onHighAttackLabel;
	delete onHighSustainLabel;

	for(int i = 0; i < 12; i++) {
		delete rowLabels[i];
		delete solenoidActionControllers[i][0];
		delete solenoidActionControllers[i][1];
	}

	delete grid;
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
