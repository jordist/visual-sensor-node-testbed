#include "SendItem.h"
#include <cmath>
#include <iostream>

SendItem::SendItem(uint8_t *msg, size_t msgsize, MessageTypes_t msgType,
		Connection *connection, int MTU) {
	MTU_ = MTU;
	max_payload_len_ = compute_max_payload_length();
	connection_ = connection;
	messageType_ = msgType;
	num_pkts_ = 0;
	Packetize(msg, msgsize, msgType, connection);
}

SendItem::SendItem(Connection *connection, int messageID, MessageTypes_t msgType) {
	num_pkts_ = 0;
	messageID_ = messageID;
	connection_ = connection;
	messageType_ = msgType;
}

SendItem::~SendItem() {
	for (int n = 0; n < num_pkts_; n++) {
		if (packets_.at(n) != NULL) {
			free(packets_.at(n));
			packets_.at(n) = NULL;
		}
	}
	free(buffer_);
}

void SendItem::Packetize(uint8_t *message, size_t message_length,
		MessageTypes_t msgType, Connection *connection) {
	PacketHeader_t *pkt_header;

	int num_pkts = compute_num_packets(message_length, max_payload_len_);
//	packets_.resize(num_pkts); //Not if you push_back the packets!!
	pkt_header = (PacketHeader_t*) calloc(1, sizeof(PacketHeader_t));
	buffer_ = (uint8_t*) malloc(message_length);
	memcpy((uint8_t*) buffer_, (uint8_t*) message, message_length);

	Packet_t* pkt;
	size_t data_location = 0;
	int n;
	connection->getNextMessageID(); //Increment messageID
	messageID_ = connection->getLastMessageID();
	for (n = 1; n < num_pkts; n++) {
		pkt = (Packet_t*) calloc(1, sizeof(Packet_t));
		generateHeader(pkt_header, &connection->srcAddress, &connection->dstAddress, &connection->dstAddress2, connection->getLastMessageID(), msgType, num_pkts, n);
		assemblePacket(pkt, pkt_header, &buffer_[data_location],
				max_payload_len_);
		AddPacket(pkt);
		data_location += max_payload_len_;
	}
	//last packet
	pkt = (Packet_t*) calloc(1, sizeof(Packet_t));
	generateHeader(pkt_header, &connection->srcAddress, &connection->dstAddress, &connection->dstAddress2, connection->getLastMessageID(), msgType, num_pkts, n);
	assemblePacket(pkt, pkt_header, &buffer_[data_location],
			message_length - data_location);
	AddPacket(pkt);

	free(pkt_header);
}

void SendItem::assemblePacket(Packet_t* pkt, PacketHeader_t *header,
		uint8_t *encoded_data, size_t encoded_data_length) {
	int encoded_bytes;

	memcpy((PacketHeader_t*) &pkt->header, (PacketHeader_t*) header,
			sizeof(PacketHeader_t));
	pkt->data.buf = encoded_data;
	pkt->data.size = (int) encoded_data_length;

}

void SendItem::generateHeader(PacketHeader_t *header, Address_t *srcAddr,
		Address_t *dstAddr, Address_t *dstAddr2, int messageID, MessageTypes_t msgType,
		int numOfPackets, int PacketSeqNum) {
	memcpy((Address_t*) &header->sourceAddress, (Address_t*) srcAddr,
			sizeof(Address_t));
	memcpy((Address_t*) &header->destinationAddress, (Address_t*) dstAddr,
			sizeof(Address_t));
	memcpy((Address_t*) &header->destinationAddress2, (Address_t*) dstAddr2,
			sizeof(Address_t));

	header->messageID = messageID;
	header->messageType = msgType;
	header->numberOfPackets = numOfPackets;
	header->packetSeqNumber = PacketSeqNum;
}

void SendItem::AddPacket(Packet_t* pkt) {
	packets_.push_back(pkt);
	num_pkts_++;
}

Packet_t* SendItem::get_packet(int n) {
	return packets_.at(n);
}

int SendItem::get_num_packets() {
	return num_pkts_;
}

Connection* SendItem::get_connection() {
	return connection_;
}

int SendItem::get_messageID() {
	return messageID_;
}

int SendItem::compute_num_packets(size_t encoded_message_size,
		size_t max_packet_data_length) {
	return std::ceil(
			(double) encoded_message_size / (double) max_packet_data_length);
}

size_t SendItem::compute_max_payload_length() {
	// MTU_ - IP_header - UDP_header - our_header
	return MTU_ - 20 - 8 - 21;
}

Packet_t* SendItem::back_and_pop_packet() {
	Packet_t* to_return = packets_.back();
	packets_.pop_back();
	return to_return;
}

std::vector<Packet_t*> SendItem::get_packets() {
	return packets_;
}
