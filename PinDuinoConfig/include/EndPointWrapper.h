/*
 * EndPointWrapper.h
 *
 *  Created on: Oct 9, 2015
 *      Author: peter
 */

#ifndef ENDPOINTWRAPPER_H_
#define ENDPOINTWRAPPER_H_

#include <gtkmm.h>

#include <SerialPort.h>

#include <Source.h>
#include <Sink.h>
#include <EscapingSource.h>
#include <EscapingSink.h>

#include <FrameReceiver.h>
#include <FrameTransmitter.h>
#include <EndPoint.h>

#include <Thread.h>
#include <FrameHandler.h>

#include <SolenoidActionController.h>

class SerialSource : public hdlc::Source<uint8_t> {
	SerialPort& serial;

public:
	SerialSource(SerialPort& serial) : serial(serial) {
	}

	bool isReady() { return serial.IsDataAvailable(); }
	uint8_t read() {
		uint8_t octet = serial.ReadByte();
		return octet;
	}
};

class SerialSink : public hdlc::Sink<uint8_t> {
	SerialPort& serial;

public:
	SerialSink(SerialPort& serial) : serial(serial) {
	}

	bool isReady() { return true; }
	void write(const uint8_t& octet) {
		try {
			serial.WriteByte(octet);
		} catch(std::runtime_error &e) {
		}
	}
};

class EndPointWrapper: public hdlc::Thread, public hdlc::FrameHandler {
	std::string device;
	SerialPort serialPort;
	SerialSource serialSource;
	SerialSink serialSink;
	hdlc::EscapingSource escapingSource;
	hdlc::EscapingSink escapingSink;
	hdlc::FrameReceiver frameReceiver;
	hdlc::FrameTransmitter frameTransmitter;
	hdlc::EndPoint endPoint;
	hdlc::FrameBuffer outgoingFrameBuffer;
	int id;
	SolenoidActionController* solenoidActionControllers[12][2];

	Gtk::Notebook* notebook;

	Gtk::Grid *grid;
	Gtk::Label *label;

	Gtk::Label *inputLabel;
	Gtk::Label *onLowLabel;
	Gtk::Label *onHighLabel;

	Gtk::Label *onLowEnableLabel;
	Gtk::Label *onLowSolenoidLabel;
	Gtk::Label *onLowAttackLabel;
	Gtk::Label *onLowSustainLabel;

	Gtk::Label *onHighEnableLabel;
	Gtk::Label *onHighSolenoidLabel;
	Gtk::Label *onHighAttackLabel;
	Gtk::Label *onHighSustainLabel;

	Gtk::Label *rowLabels[12];

protected:
	virtual PT_THREAD(run());

public:
	EndPointWrapper(Gtk::Notebook*, char*);
	virtual ~EndPointWrapper();

	virtual void handle(const uint8_t, const uint8_t*, const uint8_t);
};

#endif /* ENDPOINTWRAPPER_H_ */
