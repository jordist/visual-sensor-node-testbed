#include "Receiver.h"
#include "ConnectionManager.h"
	#include <iostream>
//	#define DEBUG

#define MAX_REPORT_LEN 10000

Receiver::Receiver(Sender *sender, Retransmission *retrans, ConnectionManager *connectionmanager, int local_inetaddr){
	sender_=sender;
	retrans_=retrans;
	connection_manager=connectionmanager;
	local_inetaddr_=local_inetaddr;
}

Receiver::~Receiver(){
	for(int i=0; i<receiveItems_.size(); i++){
		delete receiveItems_.at(i);
	}
}

int Receiver::DoStuffToObject(Packet_t* packet){	//Add the packet to a ReceiveItem, creating a new one if necessary
	//Ignore our own broadcasts:
	if(packet->header.sourceAddress == local_inetaddr_){
//		debug(printf("Ignored our own broadcast packet\n"));
		asn_DEF_Packet.free_struct(&asn_DEF_Packet, packet, 0);
		return 0;
	}
	//Check destinationAddress (both, for "broadcast"/multicast)
	if(packet->header.destinationAddress != local_inetaddr_ && packet->header.destinationAddress2 != local_inetaddr_){
		//Not directed to us, discard packet
		asn_DEF_Packet.free_struct(&asn_DEF_Packet, packet, 0);
		return 0;
	}

	int pos;
	int is_last_packet=0;
	//Let's see if we already have a ReceiveItem for this messageID and sourceAddress:
	int found=0;
	for(int i=0; i<receiveItems_.size(); i++){ //TODO break when found?
    	if( (receiveItems_.at(i)->srcAddress_ == packet->header.sourceAddress) && (receiveItems_.at(i)->messageID_ == packet->header.messageID) ){
			//Found
			found=1;
			pos=i; //Position the found ReceiveItem
		}
	}
	if(found==0){ //Not found, we create a new ReceiveItem
/*		if (CheckValidMessageID(packet)==-1){
			//We discard the packet
			return 0;
		}
*/
		ReceiveItem *rcvItem=new ReceiveItem(packet->header.sourceAddress, packet->header.messageID, packet->header.numberOfPackets, packet->header.messageType);
		pos=receiveItems_.size(); //This is the position where the new ReceiveItem will be push_back'ed
		receiveItems_.push_back(rcvItem);
	}
	//Let's add the packet to the ReceiveItem:
	is_last_packet=receiveItems_.at(pos)->NewPacketReceived(packet);
//		std::cout << "New packet type: " << packet->header.messageType << "\t messageID="<< packet->header.messageID << "\tseqNum=" << packet->header.packetSeqNumber << "\n";


	if(receiveItems_.at(pos)->ComputeNumPacketsMissing()==0){
//			std::cout << "Full message received: We can now assemble. MessageID=" << receiveItems_.at(pos)->messageID_ << "\n";
		uint8_t* data=receiveItems_.at(pos)->ConcatenateData(); //Free later at higher layers!
		size_t data_length=receiveItems_.at(pos)->GetConcatenatedDataLength();
		if(receiveItems_.at(pos)->msgType_ == MessageTypes_receptionReport){
				debug(printf("Receiver::DoStuffToObject : ReceptionReport received\n"));
				//std::cout << "Receiver: Got reception report\n";
			Connection* conn=GetConnectionTo(packet->header.sourceAddress);
			SendItem* si_to_resend = GetSendItemToReSend(decodeReceptionReport(data, data_length), conn);
			if(si_to_resend!=NULL) retrans_->set_si_for_retransmission(si_to_resend);
		}else if(receiveItems_.at(pos)->msgType_ == MessageTypes_receptionReportRequest){
				//debug(printf("Received receptionReportRequest\n"));
				//std::cout << "Received receptionReportRequest\n";
			//Find matching messageID receive item and the messageID field in the request and then SendReceptionReport(ri)
			int requested_report = decodeReportRequest(data, data_length);
			Connection* conn=GetConnectionTo(packet->header.sourceAddress);
			delete[] data;
			int requested_ri = findReceiveItem(receiveItems_.at(pos)->srcAddress_, requested_report);
			//std::cout << " Received receptionReportRequest. requested_ri=" << requested_ri << std::endl;
			if(requested_ri!=-1){
				//Current message
				SendReceptionReport(receiveItems_.at(requested_ri));
			}else{
				//Send a positive receptionreport for items already received
				//std::cout << "  Sending positiveReceptionReport" << std::endl;
				SendPositiveReceptionReport(requested_report, conn);
				/*int lastMsgID=conn->getLastReceivedMessageID();
				if (((lastMsgID - requested_report) < 32768) && (lastMsgID - requested_report)) {
					SendPositiveReceptionReport(requested_report, conn);
				}else{
					if ((requested_report - lastMsgID) > 32768) {
						SendPositiveReceptionReport(requested_report, conn);
					}
				}*/
			}
		}else{
			debug(printf("Full message received: We can now assemble. MessageID=%d\n", receiveItems_.at(pos)->messageID_));
			SendReceptionReport(receiveItems_.at(pos));
			receiveItems_.at(pos)->report_sent_=true;
			//Deliver received data:
			Connection* cn=GetConnectionTo(receiveItems_.at(pos)->srcAddress_);
			cn->notifyMsgReceived(data, data_length);

			//delete[] data; //This will have to be done somewhere in the higher layers, when the message is processed!
		}
		delete receiveItems_.at(pos);
		receiveItems_.erase(receiveItems_.begin()+pos);
	}else if(is_last_packet==1){
		//ReliableTransfer (send receptionReport)
		SendReceptionReport(receiveItems_.at(pos));
		receiveItems_.at(pos)->report_sent_=true;
//			std::cout << "Last message received, but we are missing " << receiveItems_.at(pos)->ComputeNumPacketsMissing() << " packets\n";
	}
	return 0;
}

int Receiver::findReceiveItem(Address_t srcAddress, int msgID){
	//Returns position of the matching receiveItem, returns -1 if not found
	for(int i=0; i<receiveItems_.size(); i++){
    	if( (receiveItems_.at(i)->srcAddress_ == srcAddress) && (receiveItems_.at(i)->messageID_ == msgID) ){
			return i; //Position the found ReceiveItem
		}
	}
	return -1;
}

Connection* Receiver::GetConnectionTo(Address_t address){
	return connection_manager->GetConnectionTo(address);
}

int Receiver::SendReceptionReport(ReceiveItem* ri) {
	printf("In SendReceptionReport!\n");
	int num_packets_missing = ri->ComputeNumPacketsMissing();
//		std::cout << "num_packets_missing=" << num_packets_missing << "\n";
    if (num_packets_missing == 0) {
//      float receive_time = ti_->GetSecondsPassed();
		if (ri->msgType_ != MessageTypes_receptionReport) {
			ReceptionReportMessage_t* asn_receptionreport = (ReceptionReportMessage_t*)
					calloc(1, sizeof(ReceptionReportMessage_t));
			ri->set_report_mode(0);
			uint8_t* buffer=(uint8_t*)malloc(MAX_REPORT_LEN);
			size_t buffer_len;
			ri->GetAsn1Struct(asn_receptionreport);
			buffer_len=encoder_.Encode(asn_receptionreport, buffer, MAX_REPORT_LEN);
			ASN_STRUCT_FREE(asn_DEF_ReceptionReportMessage, asn_receptionreport);
			Connection* conn=GetConnectionTo(ri->srcAddress_);
//			sender_->Send(buffer, buffer_len, MessageTypes_receptionReport, conn);
			SendItem* si = new SendItem(buffer, buffer_len, MessageTypes_receptionReport, GetConnectionTo(ri->srcAddress_), 1500);
			sender_->DoStuffToObject_then_delete_si(si);

			conn->setLastReceivedMessageID(ri->messageID_);
			free(buffer);
		}else{
			printf("We don't need to send a reception report, we have received one.\n");
		}
//      LogReceiveTime(decoded_type, receive_time);

//      delete ri;
 //     delete ti_;
//      ri = NULL;
			ri->num_packets_missing_ = -1;
      return 0;
    } else {
			int num_packets_missing = ri->ComputeNumPacketsMissing();
			ri->num_packets_missing_ = num_packets_missing;
//			float receive_time = ti_->GetSecondsPassed();
//  		logger_->AddRow("number of packets missing");
//  		logger_->AddValue("time", num_packets_missing);
//  		logger_->AddRow();
//  		logger_->AddValue("time", receive_time);
			ReceptionReportMessage_t* asn_receptionreport = (ReceptionReportMessage_t*)
					calloc(1, sizeof(ReceptionReportMessage_t));
			if (num_packets_missing <= ri->ComputeNumGroupsMissing()) {
				ri->set_report_mode(0);
			}	else {
				ri->set_report_mode(1);
			}
			size_t max_buffer_len=MAX_REPORT_LEN;
			uint8_t* buffer=(uint8_t*)malloc(max_buffer_len);
			size_t buffer_len;
			ri->GetAsn1Struct(asn_receptionreport);
			buffer_len=encoder_.Encode(asn_receptionreport, buffer, MAX_REPORT_LEN);
			ASN_STRUCT_FREE(asn_DEF_ReceptionReportMessage, asn_receptionreport);
//			sender_->Send(buffer, buffer_len, MessageTypes_receptionReport, GetConnectionTo(ri->srcAddress_)); //TODO
			SendItem* si = new SendItem(buffer, buffer_len, MessageTypes_receptionReport, GetConnectionTo(ri->srcAddress_), 1500);
			sender_->DoStuffToObject_then_delete_si(si);
			free(buffer);
    }
//	inter_packet_time_dev_ = 1;
//	inter_packet_time_ = 1;
	return 0;
}

int Receiver::TimeTracking() {
	for(int i=0;i<receiveItems_.size();i++){
		if((receiveItems_.at(i)->GetSecondsPassed() > receiveItems_.at(i)->last_packet_expected_arrival_) && (receiveItems_.at(i)->report_sent_==false)){
			printf("SendRececptionReport called from TimeTracking.\n");
			receiveItems_.at(i)->report_sent_=true;
			//TODO Fix this, the case when we don't receive all the reception reports
				// So ask Alberto about this TODO?
			SendReceptionReport(receiveItems_.at(i));
		}
		if (receiveItems_.at(i)->GetSecondsPassed() > TIME_TO_DELETE_HALF_RECEIVED_ITEM) {
			delete receiveItems_.at(i);
			receiveItems_.erase(receiveItems_.begin()+i);
		}
	}
}

int Receiver::decodeReportRequest(uint8_t* message, size_t message_len){
	ReceptionReportRequestMessage_t* request=0; //It really has to be 0!
//	request = (ReceptionReportRequestMessage_t*)calloc(1,sizeof(ReceptionReportRequestMessage_t)); //Don't
	asn_dec_rval_t dc;
	dc=uper_decode(0,&asn_DEF_ReceptionReportRequestMessage,(void**) &request, message, message_len, 0,0);
	assert(dc.consumed>0);
	int ret_value = request->messageID;
	free(request);
//	free(message);
	return ret_value;
}

ReceptionReportMessage_t* Receiver::decodeReceptionReport(uint8_t* message, size_t message_len){
	ReceptionReportMessage_t* report=0; //It really has to be 0!
 //	report = (ReceptionReportMessage_t*)calloc(1,sizeof(ReceptionReportMessage_t*)); //Don't
	asn_dec_rval_t dc;
	dc=uper_decode(0,&asn_DEF_ReceptionReportMessage,(void**) &report, message, message_len, 0,0);
	debug(printf("Decoded ReceptionReport\n"));
//	free(message);
	delete[] message;
	return report;
}

SendItem* Receiver::GetSendItemToReSend(ReceptionReportMessage_t* reception_report, Connection* conn) {
	printf("In GetSendItemToReSend!\n");
	//Check if the Request is valid (if we have anything waiting to be resent):
/*	if(retrans_->getWaitingMessageID() == -1){
//		debug(printf("GetSendItemToReSend : Invalid request\n"));
		return NULL;
	}*/

//	SendItem* si = new SendItem(retrans_->getCurrentConnection(),retrans_->getWaitingMessageID());
	SendItem* si = new SendItem(conn,retrans_->getWaitingMessageID(), MessageTypes_dataMsg);
	std::vector<Packet_t*> packets_ = retrans_->get_packets_for_retransmission();
	int report_mode = reception_report->mode;
	debug(printf("ReportMode, in Receiver::GetAsnReport!: %d.\n", report_mode));
	debug(printf("Count, in Receiver::GetAsnReport!: %d.\n", reception_report->droppedPackets.list.count));
	if (report_mode == 0) {
		debug(printf("Count of droppedPackets: %d.\n", reception_report->droppedPackets.list.count));
		for (int i = 0; i < reception_report->droppedPackets.list.count; i++) {
			debug(printf("This packet is lost (report_mode = 0): %ld.\n", *reception_report->droppedPackets.list.array[i]));
			si->AddPacket(packets_.at(*reception_report->droppedPackets.list.array[i] - 1));
		}
	} else if (report_mode == 1) {
		debug(printf("Count of lostGroups: %d.\n", reception_report->droppedPackets.list.count/2));
		for (int i = 0; i < reception_report->droppedPackets.list.count; i=i+2) {
			for (int j = *reception_report->droppedPackets.list.array[i];
					j <= *reception_report->droppedPackets.list.array[i+1]; j++) {
				debug(printf("This packet is lost (report_mode = 1): %d.\n", j));
				si->AddPacket(packets_.at(j-1));
			}
		}
	}
	debug(printf("This should appear on screen, in Receiver. Number of packets of si: %d to address: %ld.\n",
			si->get_num_packets(), si->get_connection()->dstAddress));

	//Free reception_report:
	asn_DEF_Packet.free_struct(&asn_DEF_ReceptionReportMessage, reception_report, 0);
	return si;
}

int Receiver::CheckValidMessageID(Packet_t* packet){
	Connection *conn = GetConnectionTo(packet->header.sourceAddress);
	if(conn==NULL){
		fprintf(stderr, "Receiver: Connection object not found\n");
		return -1;
	}
	int LastMessageID=conn->getLastReceivedMessageID();

	if (packet->header.messageID <= LastMessageID) {
		if (((LastMessageID - packet->header.messageID) < 32768) && (LastMessageID - packet->header.messageID)) {
			//In this case, the packet must be discarded
			debug(printf("This item has already been received\n"));
			return -1;
		}
	} else {
		if ((packet->header.messageID - LastMessageID) > 32768) {
			debug(printf("This item has already been received\n"));
			return -1;
		}
	}
}

void Receiver::notifyConnectMsg(sockaddr_in addrcli, Address_t srcAddr, Address_t dstAddr) {
	connection_manager->start(addrcli, srcAddr, dstAddr);
}

void Receiver::SendPositiveReceptionReport(int msgID, Connection *conn){
	ReceptionReportMessage_t* asn_receptionreport = (ReceptionReportMessage_t*)
					calloc(1, sizeof(ReceptionReportMessage_t));
	asn_receptionreport->mode = 0;
	uint8_t* buffer=(uint8_t*)malloc(MAX_REPORT_LEN);
	size_t buffer_len;
	buffer_len=encoder_.Encode(asn_receptionreport, buffer, MAX_REPORT_LEN);
	ASN_STRUCT_FREE(asn_DEF_ReceptionReportMessage, asn_receptionreport);
//	sender_->Send(buffer, buffer_len, MessageTypes_receptionReport, conn);
	SendItem* si = new SendItem(buffer, buffer_len, MessageTypes_receptionReport, conn, 1500);
	sender_->DoStuffToObject_then_delete_si(si);
	free(buffer);
}
