/*
 * retransmission_handler.h
 *
 *  Created on: March 1, 2014
 * 			Author: alberto
 */

#ifndef RETRANSMISSION_HANDLER_
#define RETRANSMISSION_HANDLER_

#include "asn1includes.h"
#include "SendItem.h"
#include "Sender.h"
#include "Receiver.h"
#include "Encoder.h"
#include "TimingInfo.h"
#include <vector>
#include <unistd.h>
#include "ThreadClass.h"

#define TIME_LIMIT 5
#define NUMBER_OF_REQUESTS 3
#define RETRIES_LIMIT 5

class SendItem;

class Retransmission{

	public:
		Retransmission();
		~Retransmission();
		std::vector<SendItem*> WaitAck(SendItem* const si, Sender* sender, Connection* currentConnection, int waiting_messageID);
		void set_si_for_retransmission(SendItem* const si);
		std::vector<Packet_t*> get_packets_for_retransmission();
		Connection* getCurrentConnection();
		int getWaitingMessageID();
		void SendReportRequest(int messageID);

	private:
		int number_processing_nodes_;
		SendItem* si_;
		SendItem* aux_si_;
		std::vector<SendItem*> re_si_;
		std::vector<Packet_t*> packets_;
		int number_of_reports_;
		int expected_number_of_reports_;
		Connection* currentConnection_;
		int waiting_messageID_;

		Encoder encoder_;
		Sender *sender_;


		pthread_cond_t ack_cond  = PTHREAD_COND_INITIALIZER;
		pthread_mutex_t ack_mutex = PTHREAD_MUTEX_INITIALIZER;
//		pthread_mutex_t* ack_mutex_ptr;
//		pthread_cond_t* ack_cond_ptr;
//		pthread_mutexattr_t mutex_attr;

//		pthread_mutex_t num_reports_mutex = PTHREAD_MUTEX_INITIALIZER;
};

#endif /* RETRANSMISSION_HANDLER_ */
