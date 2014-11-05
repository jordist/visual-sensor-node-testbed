/*
 * Listener.h
 *
 *  Created on: Oct 28, 2014
 *      Author: jordi
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include "ThreadClass.h"
#include "Receiver.h"
#include "asn1includes.h"
#include "Receiver.h"

#include <unistd.h>

class Listener : public ThreadClass{
	public:
	Listener(short listenPort, Receiver* receiver);
	~Listener();
	void listen();
	private:
	void ThreadEntry();
	void release_mutex(void* mutex);
	Packet_t* decode_buffer(uint8_t *buffer, int buflen);
	Receiver* receiver_;
	uint8_t *buffer_;
	size_t buflen_;
	short listenPort_;
	int so_; //Socket
};

#endif /* LISTENER_H_ */
