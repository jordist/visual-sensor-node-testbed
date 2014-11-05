#include "Sender.h"
#include "TimingInfo.h"
#include <iostream>

Sender::Sender(Retransmission *retrans) {
	retrans_ = retrans;

	so = socket(PF_INET, SOCK_DGRAM, 0);
/*	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sockaddr_in));
	sa.sin_family= AF_INET;
	sa.sin_port = htons(9001); //LOCAL_PORT
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(so, (struct sockaddr *)&sa, sizeof(struct sockaddr)) != 0){
		std::cerr << "Sender: bind failed" << std::endl;
	}
*/


	so_broadcast = socket(PF_INET, SOCK_DGRAM, 0);

	int broadcast = 1;
	setsockopt(so_broadcast, SOL_SOCKET, SO_BROADCAST, &broadcast,
			sizeof broadcast);
	MTU_ = 1500; //Wifi
	max_packet_len_ = compute_max_packet_length();
	number_of_retransmissions_ = 0;
}

/*Sender::~Sender(){

 }*/

double Sender::SendBlocking(uint8_t *msg, size_t msgsize,
		MessageTypes_t msgType, Connection *connection) {
	SendItem* si = new SendItem(msg, msgsize, msgType, connection, MTU_);
//	debug(printf("SendBlocking...\n"));
	DoStuffToObject_then_delete_si(si);
//	debug(printf("...done SendBlocking\n"));
}

int Sender::Send(uint8_t *msg, size_t msgsize, MessageTypes_t msgType,
		Connection *connection) {
	SendItem* si = new SendItem(msg, msgsize, msgType, connection, MTU_); //TODO Remember to delete later!
	AddToQue(si);
	return 0;
}

int Sender::DoStuffToObject_then_delete_si(SendItem* si) {
	DoStuffToObject(si);
//	debug(printf("Sent!\n"));
	delete si;
}

int Sender::DoStuffToObject(SendItem* si) {
//	std::cout << "Sender::DoStuffToObject: Sending message, msgtype=" << si->messageType_ << std::endl;
	Packet_t* pkt;
	uint8_t *encoded_pkt;
	size_t encoded_bytes;
	TimingInfo ti;

	for (int n = 0; n < si->get_num_packets(); n++) {
		pkt = si->get_packet(n);
		encoded_pkt = new uint8_t[max_packet_len_];
		encoded_bytes = encoder_.Encode(pkt, encoded_pkt, max_packet_len_);
		assert(encoded_bytes <= max_packet_len_ && encoded_bytes > 0);
		transmit(encoded_pkt, encoded_bytes, si->get_connection());
	}
	if (si->messageType_ != MessageTypes_receptionReport
			&& si->messageType_ != MessageTypes_receptionReportRequest
			&& si->messageType_ != MessageTypes_connectMsg
			&& (number_of_retransmissions_ < RETRIES_LIMIT)) {
//		std::cout << "Waiting To Retransmit\n";
		number_of_retransmissions_ += 1;
		WaitToRetransmit(si);
		number_of_retransmissions_ -= 1;
	}
	if(si->messageType_ == MessageTypes_dataMsg && number_of_retransmissions_==0 && si->get_connection()->isBroadcast()==0){
			si->get_connection()->setTxTime(ti.GetSecondsPassed());
	}
//	std::cerr << "Exiting DoStuffToObject\n";
//	delete si; //TODO
}

void Sender::transmit(uint8_t *encoded_packet, size_t len,
		Connection *connection) {
	int i;
	if (connection->isBroadcast() == 1) {
		i = sendto(so_broadcast, encoded_packet, len, 0,
				(struct sockaddr *) &connection->addr,
				sizeof(connection->addr));
	} else {
		i = sendto(so, encoded_packet, len, 0,
				(struct sockaddr *) &connection->addr,
				sizeof(connection->addr));
	}
	delete[] encoded_packet;
//	debug(printf("Sent %d bytes\tMessageID=%d\n", i,connection->getLastMessageID()));
//	std::cout << "Sent " << i << " bytes\tMessageID: " << connection->getLastMessageID() << "\n";
}

size_t Sender::compute_max_packet_length() {
	//MTU_ - IP_header - UDP_header
	return MTU_ - 20 - 8;
}

void Sender::WaitToRetransmit(SendItem* si) {
//	printf("In WaitToRetranmit!\n");
	std::vector<SendItem*> re_si = retrans_->WaitAck(si, this,si->get_connection(), si->get_messageID());
//	debug(printf("Number of reports received in Sender: %ld.\n", re_si.size()));
	for (int i = 0; i < re_si.size(); i++) {
		if (re_si.at(i)->get_num_packets() != 0) {
			//debug(printf("This should appear on screen, before DoStuff again. Number of packets of si: %d to address: %ld.\n",re_si.at(i)->get_num_packets(),re_si.at(i)->get_connection()->dstAddress));
			DoStuffToObject(re_si.at(i));
		}
	}
}

int Sender::TimeTracking() {
	return 0;
}

int Sender::get_number_of_retransmissions() {
	return number_of_retransmissions_;
}

int Sender::get_MTU() {
	return MTU_;
}
