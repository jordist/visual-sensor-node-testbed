/*
 * retransmission_handler.cc
 *
 *  Created on: March 1, 2014
 * 			Author: alberto
 */

#include "retransmission_handler.h"

//namespace dfe {

/*Retransmission::Retransmission(INodeManager* node_man) {
	number_processing_nodes_ = node_man->get_node_ids().size();
	// node_ids_ is used only when we use broadcast
	node_ids_ = node_man->get_node_ids();
	node_ids_.push_back(node_man->get_sink_node_id());
	number_of_reports_ = 0;
}*/

Retransmission::Retransmission() {
//	number_processing_nodes_ = node_man->get_node_ids().size();
	// node_ids_ is used only when we use broadcast
//	node_ids_ = node_man->get_node_ids();
//	node_ids_.push_back(node_man->get_sink_node_id());
	number_of_reports_ = 0;
}

Retransmission::~Retransmission() {
/*	while (0 < packets_.size()) {
		Packet_t* to_delete = packets_.back();
		packets_.pop_back();
    delete to_delete;
  }*/
}

std::vector<SendItem*> Retransmission::WaitAck(SendItem* const si, Sender* const sender, Connection* currentConnection, int waiting_messageID) {
	printf("In Retranmission::WaitAck!\n");
	currentConnection_=currentConnection;
	waiting_messageID_=waiting_messageID;
	sender_=sender;
	// re_si_ will be return to the sender
	re_si_.clear();
	si_ = si;

	struct timespec   ts;

	if(currentConnection->isBroadcast()==1){
		expected_number_of_reports_=currentConnection->getNumBroadcastNodes();
	}
	else{
		expected_number_of_reports_ = 1;
	}
	debug(printf("Expected number of reports: %d.\n", expected_number_of_reports_));

	//Save the packets of the senditem in packets_ only when the senditem is complete; the first time that it is sent
	debug(printf("Number of retransmissions: %d.\n", sender->get_number_of_retransmissions()));
	if (sender->get_number_of_retransmissions() == 1) {
		packets_.clear();
		std::vector<Packet*> aux;
		for(int i=0;i<si_->get_num_packets();i++){
			packets_.push_back(si_->get_packet(i));
		}
	}

	TimingInfo* ti = new TimingInfo();
	// If the TIME_LIMIT is reached a ReceptionReportRequestMessage is sent
	pthread_mutex_lock(&ack_mutex);
	for (int i=0; i < NUMBER_OF_REQUESTS; i++) {
		while ((number_of_reports_ < expected_number_of_reports_) && (ti->GetSecondsPassed() < TIME_LIMIT) ) {
			clock_gettime(CLOCK_REALTIME, &ts);
			ts.tv_nsec += 100*1000000; //100ms
		    pthread_cond_timedwait(&ack_cond, &ack_mutex, &ts);
		}
		if (number_of_reports_ < expected_number_of_reports_) {
			ReceptionReportRequestMessage_t* asn_reportrequest = (ReceptionReportRequestMessage_t*)
					calloc(1, sizeof(ReceptionReportRequestMessage_t));
			asn_reportrequest->messageID = waiting_messageID;
			number_of_reports_=0;
			SendReportRequest(waiting_messageID);
			debug(printf("Report request sent for messageID: %d.\n", waiting_messageID_));
//			debug(printf("A request has been sent to %s for this messageID :%d.\n",
//					si->get_address().c_str(), si->get_messageID()));
			ti = new TimingInfo();
		}
	}

	if (number_of_reports_ < expected_number_of_reports_) {
		printf("This should not appear now!\n");
		// TODO Compute union and decide whether to retransmit using broadcast or unicast
		// TODO Don't clear re_si_, we may have a prepared senditem
//		re_si_.clear();
		// For now we return the broadcast senditem again
		// TODO Include in the senditem a sequence number to be read in the receiver, and thus not take action more than once
		for (int i=0; i<packets_.size(); i++) {
			si_->AddPacket(packets_.at(i));
		}
		for (int j=0; j<expected_number_of_reports_; j++) {
//			si_->set_address(node_ids_.at(j));
			re_si_.push_back(si_);
		}
	}

	debug(printf("Number of packets in si in Retransmission: %d.\n", si->get_num_packets()));
	//packets_.clear();
	number_of_reports_ = 0;
	waiting_messageID_=-1;

	pthread_mutex_unlock(&ack_mutex);

	delete ti;
	return re_si_;
}

void Retransmission::set_si_for_retransmission(SendItem* const si) {
	pthread_mutex_lock(&ack_mutex);
	debug(printf("In set_si_for_retransmission!\n"));
	re_si_.push_back(si);
	debug(printf("This should appear on screen, in Retransmission. Number of packets of si: %d to address: %ld.\n",
					re_si_.at(number_of_reports_)->get_num_packets(), re_si_.at(number_of_reports_)->get_connection()->dstAddress));

	number_of_reports_ += 1;
//	std::cout << "  RTX: number_of_reports received=" << number_of_reports_ << " of " << expected_number_of_reports_ << std::endl;
	debug(printf("Number of reports received in Retransmission: %d.\n", number_of_reports_));
	pthread_cond_signal(&ack_cond);
	pthread_mutex_unlock(&ack_mutex);
}

std::vector<Packet_t*> Retransmission::get_packets_for_retransmission() {
	return packets_;
}

Connection* Retransmission::getCurrentConnection(){
	return currentConnection_;
}

int Retransmission::getWaitingMessageID(){
	return waiting_messageID_;
}

void Retransmission::SendReportRequest(int messageID){
	uint8_t *message;
	message=(uint8_t*)malloc(200);
	ReceptionReportRequestMessage* request = (ReceptionReportRequestMessage*)calloc(1,sizeof(ReceptionReportRequestMessage));
	request->messageID=messageID;
	int encoded_bytes = encoder_.Encode(request, message, 200);
	if(encoded_bytes>0){
		SendItem* si = new SendItem(message, encoded_bytes, MessageTypes_receptionReportRequest, currentConnection_, sender_->get_MTU());
		sender_->DoStuffToObject(si);
	}
	free(request);
	free(message);
}
//} /* namespace dfe */
