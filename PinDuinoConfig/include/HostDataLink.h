/*
 * HostDataLink.h
 *
 *  Created on: May 1, 2015
 *      Author: peter
 */

#ifndef HOSTDATALINK_H_
#define HOSTDATALINK_H_

#include <DataLink.h>

class HostDataLink: public DataLink {
public:
	HostDataLink(Hardware&);
	virtual ~HostDataLink();

	virtual void log(const char*, ...);
};

#endif /* HOSTDATALINK_H_ */
