#ifndef MESSAGE_H
#define MESSAGE_H


#include <iostream>
#include <vector>
#include <math.h>

#include <opencv2/core/core.hpp>
#include "ASN.1/StartATCMessage.h"
#include "ASN.1/StartCTAMessage.h"
#include "ASN.1/DataCTAMessage.h"
#include "ASN.1/DataATCMessage.h"


using namespace std;

#define MAX_START_CTA_MESSAGE_SIZE 1024
#define MAX_START_ATC_MESSAGE_SIZE 1024
#define MAX_DATA_CTA_MESSAGE_SIZE 102400
#define MAX_DATA_ATC_MESSAGE_SIZE 102400

enum MessageType{
	START_CTA_MESSAGE,
	START_ATC_MESSAGE,
	START_DATC_MESSAGE,
	DATA_CTA_MESSAGE,
	DATA_ATC_MESSAGE,
	STOP_MESSAGE
};

class Message{
private:

	int src_addr; //todo Address class
	int dst_addr;

public:
	virtual ~Message(){};
	MessageType msg_type;
    uint8_t seq_num;


	MessageType getMessageType(){
		return msg_type;
	}

	uint8_t getSeqNum(){
		return seq_num;
	}

	void setSeqNum(uint8_t seqnum){
		seq_num = seqnum;
	}

	void setSource(int src){
		src_addr = src;
	}

	int getSource(){
		return src_addr;
	}

	void setDestination(int dst){
		dst_addr = dst;
	}

	int getDestination(){
		return dst_addr;
	}

	virtual int getBitStream(vector<uchar>& bitstream) = 0;


};


#endif
