/*
 * S2GInterface.cpp
 *
 *  Created on: 15/lug/2014
 *      Author: greeneyes
 */

#include "S2GInterface.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "Messages/StartCTAMsg.h"
#include "Messages/StartATCMsg.h"
#include "Messages/StartDATCMsg.h"
#include "Messages/DataCTAMsg.h"
#include "Messages/DataATCMsg.h"
#include "NodeManager/NodeManager.h"

S2GInterface::S2GInterface(NodeManager* nm, boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator)
: io_service_(io_service),
  socket_(io_service)
{
	node_manager = nm;
	cout << "connecting..." << endl;
	boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&S2GInterface::handle_connect, this,
					boost::asio::placeholders::error, endpoint_iterator));
}

void S2GInterface::runThread(){
	io_service_.run();
}

void S2GInterface::stop()
{
	socket_.close();
}

void S2GInterface::handle_connect(const boost::system::error_code& error,
		tcp::resolver::iterator endpoint_iter)
{
	if (!socket_.is_open())
	{
		std::cout << "Connect timed out\n";
	}

	else if (error)
	{
		std::cout << "Connect error: " << error.message() << "\n";

		socket_.close();
	}
	// Otherwise we have successfully established a connection.
	else
	{
		std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

		// Start reading the header
		cout << "Start reading..." << endl;
		start_read_header();
	}
}

void S2GInterface::start_read_header()
{
	// Set a deadline for the read operation.
	// deadline_.expires_from_now(boost::posix_time::seconds(30));

	// Start an asynchronous operation to read a newline-delimited message.

	cout << "waiting for new messages from GUI..." << endl << endl;

	socket_.async_read_some(
			boost::asio::buffer(header_, sizeof(header_)),
			boost::bind(&S2GInterface::handle_read_message, this, boost::asio::placeholders::error));

}

void S2GInterface::handle_read_message(const boost::system::error_code& ec)
{

	cout << "Header received" << endl;


	if (!ec)
	{
		cout << "Received new message header" << endl;

		Header h = parseHeader(header_);
		//cout << h << endl;

		if (h.getNumPackets()>1){
			std::cout << "Error: packetization is not supported for this type of message \n";
			stop();
		}

		uchar data[h.getPayloadSize()];
		boost::system::error_code ec;

		//read the payload
		socket_.read_some(
				boost::asio::buffer(data, h.getPayloadSize()), ec);

		Message* msg;

		if(!ec){
			switch(h.getMsgT()){

			case START_CTA_MESSAGE:
			{
				cout << "Message is START_CTA" << endl;

				char buf[MAX_START_CTA_MESSAGE_SIZE];

				cout << "Deserializing start cta message" << endl;
				int bitstream_size = h.getPayloadSize();
				cout << "Bitstream size is " << bitstream_size << endl;

				//copy the bitstream (MAYBE REMOVED?)
				for(int i=0;i<bitstream_size;i++){
					buf[i] = data[i];
				}

				StartCTAMessage_t* internal_message = (StartCTAMessage_t*) calloc(1, sizeof(*internal_message));
				asn_dec_rval_t rval;

				rval = uper_decode_complete(0, &asn_DEF_StartCTAMessage,(void **)&internal_message, buf, MAX_START_CTA_MESSAGE_SIZE);
				msg = new StartCTAMsg(internal_message);
				msg->setSource(h.getSrcAddr());
				msg->setDestination(h.getDstAddr());


				if(rval.code != RC_OK) {
					fprintf(stderr,
							"Broken message encoding at byte %ld\n",
							(long)rval.consumed);
					exit(65); /* better, EX_DATAERR */
				} else {
					fprintf(stdout,"Printing msg as XML...\n");
					xer_fprint(stdout, &asn_DEF_StartCTAMessage, internal_message);
				}

				break;
			}

			case START_ATC_MESSAGE:
			{
				cout << "Message is START_ATC" << endl;

				char buf[MAX_START_ATC_MESSAGE_SIZE];

				cout << "Deserializing start atc message" << endl;
				int bitstream_size = h.getPayloadSize();
				cout << "Bitstream size is " << bitstream_size << endl;

				//copy the bitstream (MAYBE REMOVED)
				for(int i=0;i<bitstream_size;i++){
					buf[i] = data[i];
				}

				StartATCMessage_t* internal_message = (StartATCMessage_t*) calloc(1, sizeof(*internal_message));
				asn_dec_rval_t rval;
				rval = uper_decode_complete(0, &asn_DEF_StartATCMessage,(void **)&internal_message, buf, MAX_START_ATC_MESSAGE_SIZE);
				msg = new StartATCMsg(internal_message);
				msg->setSource(h.getSrcAddr());
				msg->setDestination(h.getDstAddr());

				if(rval.code != RC_OK) {
					fprintf(stderr,
							"Broken message encoding at byte %ld\n",
							(long)rval.consumed);
					exit(65); /* better, EX_DATAERR */
				} else {
					fprintf(stdout,"Printing msg as XML...\n");
					xer_fprint(stdout, &asn_DEF_StartATCMessage, internal_message);
				}

				break;
			}

			case START_DATC_MESSAGE:
			{
				cout << "Message is START_DATC" << endl;

				char buf[MAX_START_DATC_MESSAGE_SIZE];

				cout << "Deserializing start atc message" << endl;
				int bitstream_size = h.getPayloadSize();
				cout << "Bitstream size is " << bitstream_size << endl;

				//copy the bitstream (MAYBE REMOVED)
				for(int i=0;i<bitstream_size;i++){
					buf[i] = data[i];
				}

				StartDATCMessage_t* internal_message = (StartDATCMessage_t*) calloc(1, sizeof(*internal_message));
				asn_dec_rval_t rval;
				rval = uper_decode_complete(0, &asn_DEF_StartDATCMessage,(void **)&internal_message, buf, MAX_START_DATC_MESSAGE_SIZE);
				msg = new StartDATCMsg(internal_message);
				msg->setSource(h.getSrcAddr());
				msg->setDestination(h.getDstAddr());

				if(rval.code != RC_OK) {
					fprintf(stderr,
							"Broken message encoding at byte %ld\n",
							(long)rval.consumed);
					exit(65); /* better, EX_DATAERR */
				} else {
					fprintf(stdout,"Printing msg as XML...\n");
					xer_fprint(stdout, &asn_DEF_StartDATCMessage, internal_message);
				}

				break;
			}

			case STOP_MESSAGE:
			{
				cout << "Message is STOP" << endl;
				msg = new StopMsg();
				msg->setSource(h.getSrcAddr());
				msg->setDestination(h.getDstAddr());
				break;
			}

			default:
			{
				break;
			}

			}

			//writeMsg();
			node_manager->notify_msg(msg);
			start_read_header();

		}
		else{
			std::cout << "Error on receive: " << ec.message() << "\n";
			stop();
		}
	}
	else{
		std::cout << "Error on receive: " << ec.message() << "\n";
		stop();
	}
}

void S2GInterface::writeMsg(Message* msg){

	vector<uchar> out;
	vector<uchar> temp1;
	vector<uchar> temp2;

	msg->getBitStream(temp1);

	Header h(msg->getSource(), msg->getDestination(), msg->msg_type, 0, 1, msg->getSeqNum(), temp1.size());

	temp2 = h.serialization();

	out.reserve( temp1.size() + temp2.size() ); // preallocate memory
	//write header
	out.insert( out.end(), temp2.begin(), temp2.end() );
	//write payload
	out.insert( out.end(), temp1.begin(), temp1.end() );


	boost::asio::async_write(socket_,
			boost::asio::buffer(out),
			boost::bind(&S2GInterface::data_sent_handler, this, boost::asio::placeholders::error));

	delete(msg);
}

void S2GInterface::data_sent_handler(const boost::system::error_code& ec){

	cout << "data written..." << endl;
	if (!ec){
		//		  heartbeat_timer_.expires_from_now(boost::posix_time::seconds(1));
		// heartbeat_timer_.async_wait(boost::bind(&client::start_processing, this));
	}
	else{
		std::cout << "Error on sending data: " << ec.message() << "\n";
		stop();
	}

	cout << "ok" << endl;

}


Header S2GInterface::parseHeader(uchar data[]){

	vector<unsigned char> vec;

	for (int i = 0; i < HEADER_SIZE; i++){
		vec.push_back(data[i]);
	}

	Header msg(vec);
	return msg;
}


