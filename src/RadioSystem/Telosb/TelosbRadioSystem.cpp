/*
 * TelosbRadioSystem.cpp
 *
 *  Created on: 06/giu/2014
 *      Author: antlab
 */

#include "TelosbRadioSystem.h"


static char *msgs[] = {
		(char*)"unknown_packet_type",
		(char*)"ack_timeout"	,
		(char*)"sync"	,
		(char*)"too_long"	,
		(char*)"too_short"	,
		(char*)"bad_sync"	,
		(char*)"bad_crc"	,
		(char*)"closed"	,
		(char*)"no_memory"	,
		(char*)"unix_error"
};

void stderr_msg(serial_source_msg problem)
{
	fprintf(stderr, "Note: %s\n", msgs[problem]);
}


int TelosbRadioSystem::openRadio(const char *serial_device, int baud_rate, int non_blocking){
	//packet = NULL;
	//dealloc_packet = false;
	//serial_source src;

	telosb = open_serial_source(serial_device, baud_rate, non_blocking, stderr_msg);

	//RADIO_packetID = 0xFF;

	if(!telosb)
		return -1;
	else
		return 0;

}

void TelosbRadioSystem::receiverThread(){
	int len;
	uchar* packet = NULL;
	char buf[1024];
	asn_dec_rval_t rval;

	while(1){
		packet = (uchar*)read_serial_packet(telosb, &len);

		//READ PACKET HEADER
		cout << "TRS: message received!" << endl;

		int src_addr = packet[8]*256+packet[9];
		int dst_addr = packet[10]*256+packet[11];
		MessageType message_type = static_cast<MessageType>(packet[17]);
		int seq_num = static_cast<int>(packet[12]);
		int num_packets = *(unsigned short*)&packet[13];
		int packet_id = *(unsigned short*)&packet[15];

		int bitstream_size = (int)packet[5] - 10;

		//copy the bitstream
		vector<char> bitstream;
		for(int i=0;i<bitstream_size;i++){
			bitstream.push_back(packet[18+i]);
		}

		incoming_message_queue_ptr->addPacketToQueue(src_addr, dst_addr,message_type,seq_num,num_packets,packet_id,bitstream);

		if(packet!=NULL)
			free(packet);


//		switch(message_type){
//		case START_CTA_MESSAGE:
//		{
//			cout << "It's a start cta message!" << endl;
//			int bitstream_size = (int)packet[5] - 10;
//			cout << "Bitstream size is " << bitstream_size << endl;
//
//			//copy the bitstream
//			vector<char> bitstream;
//			for(int i=0;i<bitstream_size;i++){
//				bitstream.push_back(packet[18+i]);
//			}
//
//
//			//the notification shouldn't be here
//			//here the packet should be moved in the incoming queue
//			incoming_message_queue_ptr->addPacketToQueue(src_addr, dst_addr,message_type,seq_num,num_packets,packet_id,bitstream);
//			break;
//		}
//		case START_ATC_MESSAGE:
//		{
//			cout << "It's a start atc message!" << endl;
//
//			int bitstream_size = (int)packet[5] - 10;
//			cout << "Bitstream size is " << bitstream_size << endl;
//
//			//copy the bitstream
//			vector<char> bitstream;
//			for(int i=0;i<bitstream_size;i++){
//				bitstream.push_back(packet[18+i]);
//			}
//
//			incoming_message_queue_ptr->addPacketToQueue(src_addr, dst_addr,message_type,seq_num,num_packets,packet_id,bitstream);
//			break;
//		}
//		case DATA_CTA_MESSAGE:
//		{
//			cout << "It's a data cta message!" << endl;
//
//			int bitstream_size = (int)packet[5] - 10;
//			cout << "Bitstream size is " << bitstream_size << endl;
//
//			//copy the bitstream
//			vector<char> bitstream;
//			for(int i=0;i<bitstream_size;i++){
//				bitstream.push_back(packet[18+i]);
//			}
//
//			incoming_message_queue_ptr->addPacketToQueue(src_addr, dst_addr, message_type,seq_num,num_packets,packet_id,bitstream);
//			break;
//		}
//		case STOP_MESSAGE:
//		{
//			vector<char> bitstream;
//			cout << " It's a stop message!" << endl;
//
//			incoming_message_queue_ptr->addPacketToQueue(src_addr, dst_addr, message_type, seq_num, num_packets, packet_id, bitstream);
//		}
//
//		default:
//		{
//			cout << "Message unkwown!" <<endl;
//			break;
//		}
//
//		}


	}
}

serial_source TelosbRadioSystem::getTelosb(){
	return telosb;
}

void TelosbRadioSystem::setIncomingMessageQueue(IncomingMessageQueue* incoming_message_queue){
	incoming_message_queue_ptr = incoming_message_queue;
}





