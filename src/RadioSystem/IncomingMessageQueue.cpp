/*
 * IncomingMessageQueue.cpp
 *
 *  Created on: 06/giu/2014
 *      Author: antlab
 */

#include "IncomingMessageQueue.h"
IncomingMessageQueue::IncomingMessageQueue(RadioSystem* radio_system) {
	radio_system_ptr = radio_system;
	last_seq_num = -1;
}

IncomingMessageQueue::~IncomingMessageQueue() {
	// TODO Auto-generated destructor stub
}

int IncomingMessageQueue::size(){
	//boost::mutex::scoped_lock lock(the_mutex);
	return message_queue.size();
}

void IncomingMessageQueue::addPacketToQueue(int src_addr, int dst_addr, MessageType message_type, int seq_num, int num_packets, int packet_id, vector<char> packet_bitstream){
	//boost::mutex::scoped_lock lock(the_mutex);
	bool entry_exists = false;
	unsigned int cur_pos;
	cout << "IMQ: a new packet has arrived" << endl;
	cout << "Src addr" << src_addr << endl;
	cout << "Dst addr" << dst_addr << endl;
	cout << "MessageType " << message_type << endl;
	cout << "SeqNum " << seq_num << endl;
	cout << "Num_packets " << num_packets << endl;
	cout << "Packet id " << packet_id << endl;

	//check if the entry is there
	for(cur_pos=0;cur_pos<message_queue.size();cur_pos++){
		if(seq_num == message_queue[cur_pos].seq_num ){
			entry_exists = true;
			break;
		}
	}

	//create new entry if seq_num is different...
	if(!entry_exists && seq_num!=last_seq_num){
		message_queue_entry new_entry;
		last_seq_num = seq_num;
		new_entry.src_addr = src_addr;
		new_entry.dst_addr = dst_addr;
		new_entry.message_type = message_type;
		new_entry.seq_num = seq_num;
		new_entry.num_packets = num_packets;
		new_entry.last_packet_id = packet_id;
		new_entry.bitstream.insert(new_entry.bitstream.begin(),packet_bitstream.begin(),packet_bitstream.end());
		if(new_entry.last_packet_id == 0){
			cout << "IMQ: a new entry is created..." << endl;
			message_queue.push_back(new_entry);

			if(num_packets == 1){
				deserializeAndNotify(message_queue.size()-1);
			}
		}
		else{
			cout << "Lost first packet of the message, dropping this packet " << endl;
		}
	}
	//else append the packet_bitstream in the right position
	else{
		int last_packet_id = message_queue[cur_pos].last_packet_id;

		//check if the received packet is in the right order

		if(packet_id == last_packet_id+1){
			//append the bitstream
			cout << "adding packet to the queue" << endl;
			message_queue[cur_pos].bitstream.insert(message_queue[cur_pos].bitstream.end(),packet_bitstream.begin(),packet_bitstream.end());
			message_queue[cur_pos].last_packet_id++;
			//deserialize if it was the last packet!
			if(packet_id == num_packets-1){
				deserializeAndNotify(cur_pos);
			}
		}
		else{
			cout << "not adding packet to the queue: ";
			if(packet_id == last_packet_id){
				cout << "duplicate packet" << endl;
			}
			if(packet_id > last_packet_id+1){
				cout << "packet out of order" << endl;
			}
			//todo: delete entry
		}


	}

}

void IncomingMessageQueue::deserializeAndNotify(int cur_pos){
	//boost::mutex::scoped_lock lock(the_mutex);

	asn_dec_rval_t rval;
	Message* msg;
	int src_addr = message_queue[cur_pos].src_addr;
	int dst_addr = message_queue[cur_pos].dst_addr;

	switch(message_queue[cur_pos].message_type){
	case START_CTA_MESSAGE:
	{
		char buf[MAX_START_CTA_MESSAGE_SIZE];

		cout << "Deserializing start cta message at position " << cur_pos << endl;
		int bitstream_size = message_queue[cur_pos].bitstream.size();
		cout << "Bitstream size is " << bitstream_size << endl;

		//copy the bitstream (MAYBE REMOVED)
		for(int i=0;i<bitstream_size;i++){
			buf[i] = message_queue[cur_pos].bitstream[i];
		}

		StartCTAMessage_t* internal_message = (StartCTAMessage_t*) calloc(1, sizeof(*internal_message));
		rval = uper_decode_complete(0, &asn_DEF_StartCTAMessage,(void **)&internal_message, buf, MAX_START_CTA_MESSAGE_SIZE);
		msg = new StartCTAMsg(internal_message);
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);
		cout << "fps: " << ((StartCTAMsg*)msg)->getFramesPerSecond() << endl;
		cout << "qf: " << ((StartCTAMsg*)msg)->getQualityFactor() << endl;
		cout << "w: " << ((StartCTAMsg*)msg)->getFrameWidth() << endl;
		cout << "h: " << ((StartCTAMsg*)msg)->getFrameHeight() << endl;

		if(rval.code != RC_OK) {
			fprintf(stderr,
					"Broken message encoding at byte %ld\n",
					(long)rval.consumed);
			exit(65); /* better, EX_DATAERR */
		} else {
			fprintf(stdout,"Printing msg as XML...\n");
			xer_fprint(stdout, &asn_DEF_StartCTAMessage, internal_message);
		}

		radio_system_ptr->notifyMsg(msg);
		break;
	}

	case START_ATC_MESSAGE:
	{
		char buf[MAX_START_ATC_MESSAGE_SIZE];

		cout << "Deserializing start atc message at position " << cur_pos << endl;
		int bitstream_size = message_queue[cur_pos].bitstream.size();
		cout << "Bitstream size is " << bitstream_size << endl;

		//copy the bitstream (MAYBE REMOVED)
		for(int i=0;i<bitstream_size;i++){
			buf[i] = message_queue[cur_pos].bitstream[i];
		}

		StartATCMessage_t* internal_message = (StartATCMessage_t*) calloc(1, sizeof(*internal_message));
		rval = uper_decode_complete(0, &asn_DEF_StartATCMessage,(void **)&internal_message, buf, MAX_START_ATC_MESSAGE_SIZE);
		msg = new StartATCMsg(internal_message);
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);
		cout << "fps: " << ((StartATCMsg*)msg)->getFramesPerSecond() << endl;
		cout << "desc length: " << ((StartATCMsg*)msg)->getDescriptorLength() << endl;
		cout << "thr: " << ((StartATCMsg*)msg)->getDetectorThreshold() << endl;

		if(rval.code != RC_OK) {
			fprintf(stderr,
					"Broken message encoding at byte %ld\n",
					(long)rval.consumed);
			exit(65); /* better, EX_DATAERR */
		} else {
			fprintf(stdout,"Printing msg as XML...\n");
			xer_fprint(stdout, &asn_DEF_StartATCMessage, internal_message);
		}

		radio_system_ptr->notifyMsg(msg);
		break;
	}
	case START_DATC_MESSAGE:
	{
		char buf[MAX_START_DATC_MESSAGE_SIZE];

		cout << "Deserializing start datc message at position " << cur_pos << endl;
		int bitstream_size = message_queue[cur_pos].bitstream.size();
		cout << "Bitstream size is " << bitstream_size << endl;

		//copy the bitstream (MAYBE REMOVED)
		for(int i=0;i<bitstream_size;i++){
			buf[i] = message_queue[cur_pos].bitstream[i];
		}

		StartDATCMessage_t* internal_message = (StartDATCMessage_t*) calloc(1, sizeof(*internal_message));
		rval = uper_decode_complete(0, &asn_DEF_StartDATCMessage,(void **)&internal_message, buf, MAX_START_DATC_MESSAGE_SIZE);
		msg = new StartDATCMsg(internal_message);
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);
		cout << "fps: " << ((StartDATCMsg*)msg)->getFramesPerSecond() << endl;
		cout << "desc length: " << ((StartDATCMsg*)msg)->getDescriptorLength() << endl;
		cout << "thr: " << ((StartDATCMsg*)msg)->getDetectorThreshold() << endl;

		if(rval.code != RC_OK) {
			fprintf(stderr,
					"Broken message encoding at byte %ld\n",
					(long)rval.consumed);
			exit(65); /* better, EX_DATAERR */
		} else {
			fprintf(stdout,"Printing msg as XML...\n");
			xer_fprint(stdout, &asn_DEF_StartDATCMessage, internal_message);
		}

		radio_system_ptr->notifyMsg(msg);
		break;
	}


	case DATA_CTA_MESSAGE:
	{
		char buf[MAX_DATA_CTA_MESSAGE_SIZE];

		cout << "Deserializing data cta message at position " << cur_pos << endl;
		int bitstream_size = message_queue[cur_pos].bitstream.size();
		cout << "Bitstream size is " << bitstream_size << endl;

		//copy the bitstream (MAYBE REMOVED)
		for(int i=0;i<bitstream_size;i++){
			buf[i] = message_queue[cur_pos].bitstream[i];
		}

		DataCTAMessage_t* internal_message = (DataCTAMessage_t*) calloc(1, sizeof(*internal_message));
		rval = uper_decode_complete(0, &asn_DEF_DataCTAMessage,(void **)&internal_message, buf, MAX_DATA_CTA_MESSAGE_SIZE);
		msg = new DataCTAMsg(internal_message);
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);
		cout << "slice number: " << ((DataCTAMsg*)msg)->getSliceNumber() << endl;
		cout << "frame id: " << ((DataCTAMsg*)msg)->getFrameId() << endl;

		if(rval.code != RC_OK) {
			fprintf(stderr,
					"Broken message encoding at byte %ld\n",
					(long)rval.consumed);
			exit(65); /* better, EX_DATAERR */
		} else {
			fprintf(stdout,"Printing msg as XML...\n");
			xer_fprint(stdout, &asn_DEF_DataCTAMessage, internal_message);
		}

		radio_system_ptr->notifyMsg(msg);
		break;
	}
	case DATA_ATC_MESSAGE:
	{
		char buf[MAX_DATA_CTA_MESSAGE_SIZE];

		cout << "Deserializing data cta message at position " << cur_pos << endl;
		int bitstream_size = message_queue[cur_pos].bitstream.size();
		cout << "Bitstream size is " << bitstream_size << endl;

		//copy the bitstream (MAYBE REMOVED)
		for(int i=0;i<bitstream_size;i++){
			buf[i] = message_queue[cur_pos].bitstream[i];
		}

		DataATCMessage_t* internal_message = (DataATCMessage_t*) calloc(1, sizeof(*internal_message));
		rval = uper_decode_complete(0, &asn_DEF_DataATCMessage,(void **)&internal_message, buf, MAX_DATA_ATC_MESSAGE_SIZE);
		msg = new DataATCMsg(internal_message);
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);

		if(rval.code != RC_OK) {
			fprintf(stderr,
					"Broken message encoding at byte %ld\n",
					(long)rval.consumed);
			exit(65); /* better, EX_DATAERR */
		} else {
			fprintf(stdout,"Printing msg as XML...\n");
			xer_fprint(stdout, &asn_DEF_DataATCMessage, internal_message);
		}

		radio_system_ptr->notifyMsg(msg);
		break;
	}
	case STOP_MESSAGE:
	{
		//don't need to deserialize (there's no payload)
		msg = new StopMsg();
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);
		radio_system_ptr->notifyMsg(msg);
		break;
	}
	case COOP_INFO_MESSAGE:{
		char buf[MAX_COOP_INFO_MESSAGE_SIZE];

		cout << "Deserializing coop info message at position " << cur_pos << endl;
		int bitstream_size = message_queue[cur_pos].bitstream.size();
		cout << "Bitstream size is " << bitstream_size << endl;

		//copy the bitstream (MAYBE REMOVED)
		for(int i=0;i<bitstream_size;i++){
			buf[i] = message_queue[cur_pos].bitstream[i];
		}

		CooperatorInfo_t* internal_message = (CooperatorInfo_t*) calloc(1, sizeof(*internal_message));
		rval = uper_decode_complete(0, &asn_DEF_CooperatorInfo,(void **)&internal_message, buf, MAX_COOP_INFO_MESSAGE_SIZE);
		msg = new CoopInfoMsg(internal_message);
		msg->setSource(src_addr);
		msg->setDestination(dst_addr);

		if(rval.code != RC_OK) {
			fprintf(stderr,
					"Broken message encoding at byte %ld\n",
					(long)rval.consumed);
			exit(65); /* better, EX_DATAERR */
		} else {
			fprintf(stdout,"Printing msg as XML...\n");
			xer_fprint(stdout, &asn_DEF_CooperatorInfo, internal_message);
		}

		radio_system_ptr->notifyMsg(msg);
		break;
	}
	default:
		break;
	}

	//erase the entry from the message queue once notified
	message_queue.erase(message_queue.begin()+cur_pos);
}



