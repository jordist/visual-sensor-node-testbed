/*
 * IncomingMessageQueue.cpp
 *
 *  Created on: 06/giu/2014
 *      Author: antlab
 */

#include "IncomingMessageQueue.h"
#include "Messages/Header.h"
#include "MessageParser.h"

IncomingMessageQueue::IncomingMessageQueue(RadioSystem* radio_system, MessageParser* m) {
	radio_system_ptr = radio_system;
	msg_parser = m;
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

	Header* h = new Header(src_addr,dst_addr,message_queue[cur_pos].message_type,0,0,0,message_queue[cur_pos].bitstream.size());
	msg = msg_parser->parseMessage(h,&message_queue[cur_pos].bitstream[0]);
	radio_system_ptr->notifyMsg(msg);

	//erase the entry from the message queue once notified
	message_queue.erase(message_queue.begin()+cur_pos);
}



