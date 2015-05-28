/*
 * EndPoint.h
 *
 *  Created on: Feb 3, 2015
 *      Author: peter
 */

#ifndef ENDPOINT_H_
#define ENDPOINT_H_

#include <AbstractEndPoint.h>

#include <OutgoingPinDuinoDataLink.h>
#include <PingPong.h>

class EndPoint : public AbstractEndPoint {
	PingPong& pingPong;
	OutgoingPinDuinoDataLink& logger;

protected:
	void handleIncomingFrame();

public:
	EndPoint(IncomingDataLink&, OutgoingPinDuinoDataLink&, PingPong&);
	virtual ~EndPoint();
};

#endif /* ENDPOINT_H_ */
