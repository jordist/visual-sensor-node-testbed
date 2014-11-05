#ifndef SEND_ITEM_H_
#define SEND_ITEM_H_

#include "asn1includes.h"
#include "Connection.h"
#include <vector>

class SendItem {
public:
	SendItem(uint8_t *msg, size_t msgsize, MessageTypes_t msgType,
			Connection *connection, int MTU_);
	SendItem(Connection *connection, int messageID, MessageTypes_t msgType);
	~SendItem();
	Packet_t* get_packet(int n);
	void AddPacket(Packet_t* pkt);
	int get_num_packets();
	Connection* get_connection();
	int get_messageID();
	std::vector<Packet_t*> get_packets();
	Packet_t* back_and_pop_packet();
	MessageTypes_t messageType_;
private:
	void Packetize(uint8_t *msg, size_t msgsize, MessageTypes_t msgType,
			Connection *connection);
	void assemblePacket(Packet_t* pkt, PacketHeader_t *header,
			uint8_t *encoded_data, size_t encoded_data_length);
	void generateHeader(PacketHeader_t *header, Address_t *srcAddr,
			Address_t *dstAddr, Address_t *dstAddr2, int messageID, MessageTypes_t msgType,
			int numOfPackets, int PacketSeqNum);
	int compute_num_packets(size_t encoded_message_size,
			size_t max_packet_data_length);
	size_t compute_max_payload_length();
//	size_t compute_max_packet_length();
	size_t max_payload_len_;
	int num_pkts_;
	int MTU_;
	int messageID_;
	Connection *connection_;
	std::vector<Packet_t*> packets_;
	uint8_t *buffer_;

};

#endif /* SEND_ITEM_H_ */
