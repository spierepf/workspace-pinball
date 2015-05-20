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

protected:
	void handleIncomingFrame();

public:
	EndPoint(IncomingDataLink&, OutgoingDataLink&, PingPong&);
	virtual ~EndPoint();
};

#endif /* ENDPOINT_H_ */
