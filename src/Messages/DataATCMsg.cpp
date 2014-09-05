/*
 * DataATCMsg.cpp
 *
 *  Created on: Jul 11, 2014
 *      Author: Ale
 */


/*
 * DataCTAMsg.cpp
 *
 *  Created on: Jul 9, 2014
 *      Author: Ale
 */
#include "DataATCMsg.h"

DataATCMsg::~DataATCMsg(){
	free(internal_msg);
}

DataATCMsg::DataATCMsg(int frameID, int blockNumber, vector<uchar>& features_data,
		vector<uchar>& keypoints_data){
	msg_type = DATA_ATC_MESSAGE;
	internal_msg = (DataATCMessage_t*) calloc(1, sizeof(*internal_msg));
	assert(internal_msg);
	internal_msg->frameID = frameID;
	internal_msg->blockNumber = blockNumber;
	OCTET_STRING_t ft_data;
	OCTET_STRING_t kp_data;

	ft_data.size = features_data.size();
	ft_data.buf = static_cast<uint8_t*>(&features_data[0]);
	internal_msg->featuresData = ft_data;

	//todo: modify this... should probably check if the pointer is != from null
	if(keypoints_data.size() != 0){
		kp_data.size = keypoints_data.size();
		kp_data.buf = static_cast<uint8_t*>(&keypoints_data[0]);
		internal_msg->kptsData = kp_data;
	}
}

DataATCMsg::DataATCMsg(DataATCMessage_t *internal_message){
	msg_type = DATA_ATC_MESSAGE;
	internal_msg = internal_message;
}

int DataATCMsg::getFrameId(){
	return internal_msg->frameID;
}
int DataATCMsg::getBlockNumber(){
	return internal_msg->blockNumber;
}

OCTET_STRING_t DataATCMsg::getFeaturesData(){
	return internal_msg->featuresData;
}

OCTET_STRING_t DataATCMsg::getKeypointsData(){
	return internal_msg->kptsData;
}

int DataATCMsg::getBitStream(vector<uchar>& bitstream){
	uchar buf[MAX_DATA_ATC_MESSAGE_SIZE];         /* Temporary buffer - 10K*/
	asn_enc_rval_t ec;

	ec = uper_encode_to_buffer(&asn_DEF_DataATCMessage,internal_msg, buf, MAX_DATA_ATC_MESSAGE_SIZE);
	for(int i=0;i<ceil(double(ec.encoded)/8);i++){
		bitstream.push_back(buf[i]);
	}
	return ec.encoded;
}

