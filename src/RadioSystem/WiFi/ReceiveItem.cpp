/*
 * ReceiveItem.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: jordi
 */

#include "ReceiveItem.h"
#include <cmath>
	#include <iostream>

#define SAFETY_MARGIN 0.2
//#define SAFETY_MARGIN 0


//TODO check correct dstAddress for all packets

ReceiveItem::ReceiveItem(Address_t srcAddress, int messageID, int numPackets, MessageTypes_t msgType){
	srcAddress_=srcAddress;
	messageID_=messageID;
	numPackets_=numPackets;
	msgType_=msgType;
	num_packets_missing_=-1;
	report_sent_ = false;
	inter_packet_time_dev_=1;
	inter_packet_time_=1;
	ti_last_packet_ = new TimingInfo();
	packets_.resize(numPackets_,NULL);
}

ReceiveItem::~ReceiveItem(){
	free_all_packets();
	delete ti_last_packet_;
}

int ReceiveItem::NewPacketReceived(Packet_t* pkt){
	int is_last_packet=0;
	cur_pkt_=pkt->header.packetSeqNumber;
	if(pkt->header.packetSeqNumber <= numPackets_){ //Bound check
		packets_.at(pkt->header.packetSeqNumber-1)=pkt;
		if(pkt->header.packetSeqNumber==numPackets_){ // Check if it is the last packet
			is_last_packet=1;
		}
	}
	if (ti_last_packet_ != NULL) {
		double time_since_last_packet = ti_last_packet_->GetSecondsPassed();
		inter_packet_time_dev_ = (1-BETA) * inter_packet_time_dev_ + BETA * std::abs(time_since_last_packet - inter_packet_time_);
		inter_packet_time_ = (1-ALPHA)*inter_packet_time_ + ALPHA *	time_since_last_packet;
		ti_last_packet_->update_starttime();
	}
	if (num_packets_missing_ == -1) {
			last_packet_expected_arrival_ = (numPackets_ - cur_pkt_) * (inter_packet_time_ + 4*inter_packet_time_dev_) + SAFETY_MARGIN;
//			debug(printf("last_packet_expected_arrival_=%f\tIPTD=%f\n",last_packet_expected_arrival_,inter_packet_time_dev_));
	} else {
			report_sent_ = false;
			num_packets_missing_ -= 1;
			last_packet_expected_arrival_ = num_packets_missing_ * (inter_packet_time_ + 4*inter_packet_time_dev_) + SAFETY_MARGIN;
	}
	return is_last_packet;
}

bool ReceiveItem::IsPacketReceived(int pkt_num) {
/*  if (pkt_num < 0) {
    fprintf(stderr, "You are checking if the packets before the first was "
          "received.\n");
    return false;
  }
  if (pkt_num > packets_->size()) {
    fprintf(stderr, "You are checking if the packets after the last was "
        "received.\n");
    return false;
  }*/
  if (packets_.at(pkt_num - 1) != NULL) {
    // Packet numbering starts at 1.
    return true;
  } else {
    return false;
  }
}

int ReceiveItem::ComputeNumPacketsMissing(){
	int missing=0;
	lost_packets_.clear();
	for(int i=1;i<=numPackets_;i++){
		if(packets_.at(i-1)==NULL){
			missing++;
			lost_packets_.push_back(i);
		}
	}
	return missing;
}

int ReceiveItem::ComputeNumGroupsMissing() {
	int num_groups_lost = 0;
	lost_groups_.clear();
/*  if (packets_->size() != tot_pkts_) {
    fprintf(stderr, "The container to mark if packets are received does not "
        "have the correct size.\n");
    return -1;
  }*/
	if (!IsPacketReceived(1)) {
		lost_groups_.push_back(1);
		num_groups_lost += 1;
	}
	for (int n = 2; n <= packets_.size(); n++) {
		if (!IsPacketReceived(n) && (IsPacketReceived(n-1))) {
			lost_groups_.push_back(n);
			num_groups_lost += 1;
		}
		else if (IsPacketReceived(n) && (!IsPacketReceived(n-1))) {
			lost_groups_.push_back(n-1);
		}
	}
	if(!IsPacketReceived(packets_.size())){
		lost_groups_.push_back(packets_.size());
	}
	return num_groups_lost;
}

uint8_t* ReceiveItem::ConcatenateData(){
	//TODO bound check
	uint8_t *buffer;
	size_t max_length;
//	assert(ComputeNumPacketsMissing()==0);
	max_length=numPackets_ * packets_.at(0)->data.size; // numPackets * length of the first packet
	buffer=new uint8_t[max_length];
//	assert(buffer!=NULL);

	concatenated_data_length_=0;
	uint8_t *ptr_to_data;
	size_t data_size;
	for (int n=0; n<numPackets_; n++){
		ptr_to_data=packets_.at(n)->data.buf;
		data_size=packets_.at(n)->data.size;
//		assert(concatenated_data_length_ + data_size <= max_length);
		memcpy((uint8_t*)&buffer[concatenated_data_length_], (uint8_t*) ptr_to_data, data_size);
		concatenated_data_length_ += data_size;
		//Free will be done in the destructor for the receiveItem
//		free_packet(n); //This frees the struct AND the pointer
	}
	return buffer;
}

size_t ReceiveItem::GetConcatenatedDataLength(){
	return concatenated_data_length_;
}

void ReceiveItem::free_packet(int n){
	if(packets_.at(n)!=NULL){
		asn_DEF_Packet.free_struct(&asn_DEF_Packet, packets_.at(n), 0); //This frees the struct AND the pointer (That last 0 frees the pointer)
		packets_.at(n)=NULL;
	}
}

void ReceiveItem::free_all_packets(){
	for(int n=0;n<numPackets_;n++){
		free_packet(n);
	}
}

void ReceiveItem::set_report_mode(int report_mode) {
	report_mode_ = report_mode;
}

const ReceptionReportMessage_t* ReceiveItem::GetAsn1Struct(ReceptionReportMessage_t* const asn_receptionreport){
	//debug(printf("In ReceiveItem::GetAsn1Struct()!, report mode = %d.\n", report_mode_));
	if (report_mode_==0) {
		debug(printf("Size of lost_packets_ = %ld.\n", lost_packets_.size()));
		for (int i=0; i<lost_packets_.size(); i++) {
			debug(printf("Content of lost_packets_ = %d.\n", lost_packets_.at(i)));
			int* lost_packet = (int*) calloc(1,sizeof(int));
			*lost_packet = lost_packets_.at(i);
//			asn_set_add((void*) (&asn_receptionreport->droppedPackets.list.array), (void*) (lost_packet));
			asn_sequence_add((void*) (&asn_receptionreport->droppedPackets.list), (void*) (lost_packet));
		}
		asn_receptionreport->mode = 0;
	} else if (report_mode_==1) {
		debug(printf("Size of lost_groups_ = %ld.\n", lost_groups_.size()));
		for (int i=0; i<lost_groups_.size(); i++) {
			debug(printf("Content of lost_groups_: %d.\n", lost_groups_.at(i)));
			int* lost_groups = (int*) calloc(1, sizeof(int));
			*lost_groups = lost_groups_.at(i);
//			asn_set_add((void*) (&asn_receptionreport->droppedPackets.list.array), (void*) (lost_groups));
			asn_sequence_add((void*) (&asn_receptionreport->droppedPackets.list), (void*) (lost_groups));
		}
		asn_receptionreport->mode = 1;
	}
	//Asn1StructGen::StringToBuffer(report_mode_, &asn_receptionreport->reportmode);
	return asn_receptionreport;
}

double ReceiveItem::GetSecondsPassed(){
	return ti_last_packet_->GetSecondsPassed();
}
