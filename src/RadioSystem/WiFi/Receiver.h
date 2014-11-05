#ifndef RECEIVER_H_
#define RECEIVER_H_

#include "asn1includes.h"
#include "Encoder.h"
#include "Connection.h"
#include "ThreadedQue.h"
#include "uncopyable.h"
#include "ReceiveItem.h"
#include "retransmission_handler.h"
#include "Sender.h"
#include <vector>

#define TIME_TO_DELETE_HALF_RECEIVED_ITEM 300

class Retransmission;
class Sender;
class ConnectionManager;
//class ThreadedQue;

class Receiver: private Uncopyable, public ThreadedQue<Packet_t*> {
public:
	Receiver(Sender *sender, Retransmission *retrans,
			ConnectionManager *connectionmanager, int local_inetaddr);
	~Receiver();
	void notifyConnectMsg(sockaddr_in addrcli, Address_t srcAddr, Address_t dstAddr);

private:
	int DoStuffToObject(Packet_t* packet);
	int findReceiveItem(Address_t srcAddress, int msgID);
	int decodeReportRequest(uint8_t* message, size_t message_len);
	ReceptionReportMessage_t* decodeReceptionReport(uint8_t* message,
			size_t message_len);
	SendItem* GetSendItemToReSend(ReceptionReportMessage_t* reception_report, Connection* conn);
	int SendReceptionReport(ReceiveItem* ri);
	Connection* GetConnectionTo(Address_t address);
	int TimeTracking();
	int CheckValidMessageID(Packet_t* packet);
	void SendPositiveReceptionReport(int msgID, Connection *conn);
//  void LogReceiveTime(TypeHeaderAsn_PR decoded_type, float value);

	std::vector<ReceiveItem*> receiveItems_; //TODO: Use a list or forward_list, this can be too inefficient when reallocating the vector

	Encoder encoder_;
	Retransmission *retrans_;
	Sender *sender_;
	ConnectionManager* connection_manager;
	int local_inetaddr_;
//
// Logger* logger_;
//  TimingInfo* ti_;

};

#endif /* RECEIVER_H_ */
