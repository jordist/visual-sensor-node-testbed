/*
 * ReceiveItem.h
 *
 *  Created on: Oct 28, 2014
 *      Author: jordi
 */

#ifndef RECEIVEITEM_H_
#define RECEIVEITEM_H_

#include "asn1includes.h"
#include "TimingInfo.h"
#include <vector>
#include "debug.h"

#define ALPHA 0.1
#define BETA 0.2

class ReceiveItem {
public:
	Address_t srcAddress_;
	int messageID_;
	int numPackets_;
	MessageTypes_t msgType_;
	int cur_pkt_;
	int num_packets_missing_;
	bool report_sent_;
	double inter_packet_time_dev_;
	double inter_packet_time_;
	double last_packet_expected_arrival_;
	TimingInfo* ti_last_packet_;

	ReceiveItem(Address_t srcAddress, int messageID, int numPackets,
			MessageTypes_t msgType);
	~ReceiveItem();
	int NewPacketReceived(Packet_t* pkt); //Returns 1 if it is the last packet (does not mean we have all the previous ones), 0 otherwise
	int ComputeNumPacketsMissing();
	int ComputeNumGroupsMissing();
	uint8_t* ConcatenateData(); //Returns pointer to concatenated data
	size_t GetConcatenatedDataLength(); //Returns length of the concatenated data we saved in ConcatenateData()
	void free_packet(int n); //TODO make private?
	void free_all_packets(); //TODO make private?
	void set_report_mode(int report_mode);
	const ReceptionReportMessage_t* GetAsn1Struct(
			ReceptionReportMessage_t* const asn_receptionreport);
	double GetSecondsPassed();
private:
	bool IsPacketReceived(int pkt_num);
	std::vector<Packet_t*> packets_;
	size_t concatenated_data_length_;
	int report_mode_;
	std::vector<int> lost_packets_;
	std::vector<int> lost_groups_;

};

#endif /* RECEIVEITEM_H_ */
