/*
 * TelosbRadioSystem.h
 *
 *  Created on: 06/giu/2014
 *      Author: antlab
 */
#ifndef TELOSB_RADIO_SYSTEM_H
#define TELOSB_RADIO_SYSTEM_H

#include <stdio.h>
#include "serialsource.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/thread/condition.hpp>
#include "opencv2/core/core.hpp"
#include "Messages/Message.h"
#include "Messages/StartCTAMsg.h"
#include "Messages/StartATCMsg.h"
#include "RadioSystem/IncomingMessageQueue.h"

using namespace std;


/*typedef struct telosb_pkt_header_t{
	uchar reserved_fields[8];

	uchar src_addr_msb;
	uchar src_addr_lsb;
	uchar dst_addr_msb;
	uchar dst_addr_lsb;

	uchar message_id;

	uchar num_frames_msb;
	uchar num_frames_lsb;

	uchar pkt_id_msb;
	uchar pkt_id_lsb;

	uchar type;
}telosb_pkt_header;*/


class TelosbRadioSystem {
public:
	void startReceiver(){
			r_thread = boost::thread(&TelosbRadioSystem::receiverThread, this);
		}
	void joinReceiver(){
			r_thread.join();
		}

	int openRadio(const char *serial_device, int baud_rate, int non_blocking);

	//int sendTestPacket(int radio_dst, MessageType message_type, vector<char>& bitstream);
	serial_source getTelosb();
	void setIncomingMessageQueue(IncomingMessageQueue* incoming_message_queue);

private:
	//vector<vector<uchar>> packet_queue;
	serial_source telosb;
	boost::thread r_thread;
	IncomingMessageQueue* incoming_message_queue_ptr;

	void receiverThread();

};

#endif
