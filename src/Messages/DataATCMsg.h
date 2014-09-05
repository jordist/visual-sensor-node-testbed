/*
 * DataATCMsg.h
 *
 *  Created on: Jul 11, 2014
 *      Author: Ale
 */

#ifndef DATAATCMSG_H_
#define DATAATCMSG_H_

#include <vector>
#include "Message.h"


class DataATCMsg : public Message{
private:
	DataATCMessage_t* internal_msg;

public:
	~DataATCMsg();
	DataATCMsg(int frameID, int blockNumber, vector<uchar>& features_data,
			vector<uchar>& keypoints_data);
	DataATCMsg(DataATCMessage_t* internal_message);
	int getBitStream(vector<uchar>& bitstream);

	int getFrameId();
	int getBlockNumber();
	OCTET_STRING_t getFeaturesData();
	OCTET_STRING_t getKeypointsData(); //may return directly the opencv Mat
};

#endif /* DATAATCMSG_H_ */
