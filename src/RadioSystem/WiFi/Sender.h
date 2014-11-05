#ifndef SENDER_H_
#define SENDER_H_

#include "asn1includes.h"
#include "ThreadedQue.h"
#include "uncopyable.h"
#include "SendItem.h"
#include "Encoder.h"
#include "Connection.h"
#include "retransmission_handler.h"

#include <vector>

class Retransmission;

class Sender : private Uncopyable, public ThreadedQue<SendItem*>{
	public:
	Sender(Retransmission *retrans);
	//~Sender(); //TODO
	int Send(uint8_t *msg, size_t msgsize, MessageTypes_t msgType, Connection *connection);
	double SendBlocking(uint8_t *msg, size_t msgsize, MessageTypes_t msgType, Connection *connection); // Returns transmission time
	int DoStuffToObject(SendItem* si);
	int DoStuffToObject_then_delete_si(SendItem* si);
	int get_number_of_retransmissions();
	int get_MTU();
	private:
	void transmit(uint8_t *encoded_packet, size_t len, Connection *connection);
	size_t compute_max_packet_length();
	void WaitToRetransmit(SendItem* si);
	int TimeTracking();


	int so; //Socket
	int so_broadcast;//Socket for broadcast
	int MTU_;
	size_t max_packet_len_;
	int number_of_retransmissions_;
	Encoder encoder_;
	Retransmission *retrans_;
};


#endif /* SENDER_H_ */
