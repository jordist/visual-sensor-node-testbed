/*
 * Connection.cpp
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */

#include "Connection.h"
#include <vector>
#include <boost/bind.hpp>
#include "ConnectionManager.h"
#include "Messages/StartCTAMsg.h"
#include "Messages/StartATCMsg.h"
#include "Messages/StartDATCMsg.h"
#include "Messages/DataCTAMsg.h"
#include "Messages/DataATCMsg.h"
#include "Messages/StopMsg.h"
#include "NodeManager/NodeManager.h"
#include "RadioSystem/MessageParser.h"


using namespace std;

Connection::Connection(boost::asio::io_service& io_service,
		ConnectionManager& manager/*, MessageHandler& handler*/)
: socket_(io_service),
  connection_manager_(manager)
/*,
    message_handler_(handler)*/
{
}

void Connection::setNodeManager(NodeManager* nm){
	node_manager = nm;
}

void Connection::setMessageParser(MessageParser* m){
	message_parser = m;
}

boost::asio::ip::tcp::socket& Connection::socket()
{
	return socket_;
}

/*void Connection::start()
{
	socket_.async_read_some(boost::asio::buffer(header_, sizeof(header_)),
			boost::bind(&Connection::readHeader, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}*/

void Connection::stop()
{
	socket_.close();
}

//UNDERSTAND WHAT TO DO...
void Connection::readHeader()
{
	//something like this:

	cout << "Ready to read from client..." << endl;
	//read wifi packet header and then read the rest of the message
	socket_.async_read_some(
			boost::asio::buffer(header_, sizeof(header_)),
			boost::bind(&Connection::handleReadMessage, this, boost::asio::placeholders::error));
}


// TODO: create a message parser, since this code is repeated several times in the project!
void Connection::handleReadMessage(const boost::system::error_code& ec)
{

	cout << "Header received" << endl;


	if (!ec)
	{
		cout << "Received new message header" << endl;

		Header* h = message_parser->parseHeader(header_);
		//Header h = parseHeader(header_);
		cout << h << endl;

		if (h->getNumPackets()>1){
			std::cout << "Error: packetization is not supported for this type of message \n";
			stop();
		}

		uchar data[h->getPayloadSize()];
		boost::system::error_code ec;

		//read the payload
		socket_.read_some(
				boost::asio::buffer(data, h->getPayloadSize()), ec);

		Message* msg;

		if(!ec){
			Message* msg = message_parser->parseMessage(h,data,this);
			node_manager->notify_msg(msg);
			readHeader();

//			switch(h.getMsgT()){
//
//			case START_CTA_MESSAGE:
//			{
//				cout << "Message is START_CTA" << endl;
//
//				char buf[MAX_START_CTA_MESSAGE_SIZE];
//
//				cout << "Deserializing start cta message" << endl;
//				int bitstream_size = h.getPayloadSize();
//				cout << "Bitstream size is " << bitstream_size << endl;
//
//				//copy the bitstream (MAYBE REMOVED?)
//				for(int i=0;i<bitstream_size;i++){
//					buf[i] = data[i];
//				}
//
//				StartCTAMessage_t* internal_message = (StartCTAMessage_t*) calloc(1, sizeof(*internal_message));
//				asn_dec_rval_t rval;
//
//				rval = uper_decode_complete(0, &asn_DEF_StartCTAMessage,(void **)&internal_message, buf, MAX_START_CTA_MESSAGE_SIZE);
//				msg = new StartCTAMsg(internal_message);
//				msg->setSource(h.getSrcAddr());
//				msg->setDestination(h.getDstAddr());
//				msg->setTcpConnection(this);
//
//
//
//				if(rval.code != RC_OK) {
//					fprintf(stderr,
//							"Broken message encoding at byte %ld\n",
//							(long)rval.consumed);
//					exit(65); /* better, EX_DATAERR */
//				} else {
//					fprintf(stdout,"Printing msg as XML...\n");
//					xer_fprint(stdout, &asn_DEF_StartCTAMessage, internal_message);
//				}
//
//				break;
//			}
//
//			case START_ATC_MESSAGE:
//			{
//				cout << "Message is START_ATC" << endl;
//
//				char buf[MAX_START_ATC_MESSAGE_SIZE];
//
//				cout << "Deserializing start atc message" << endl;
//				int bitstream_size = h.getPayloadSize();
//				cout << "Bitstream size is " << bitstream_size << endl;
//
//				//copy the bitstream (MAYBE REMOVED)
//				for(int i=0;i<bitstream_size;i++){
//					buf[i] = data[i];
//				}
//
//				StartATCMessage_t* internal_message = (StartATCMessage_t*) calloc(1, sizeof(*internal_message));
//				asn_dec_rval_t rval;
//				rval = uper_decode_complete(0, &asn_DEF_StartATCMessage,(void **)&internal_message, buf, MAX_START_ATC_MESSAGE_SIZE);
//				msg = new StartATCMsg(internal_message);
//				msg->setSource(h.getSrcAddr());
//				msg->setDestination(h.getDstAddr());
//				msg->setTcpConnection(this);
//
//				if(rval.code != RC_OK) {
//					fprintf(stderr,
//							"Broken message encoding at byte %ld\n",
//							(long)rval.consumed);
//					exit(65); /* better, EX_DATAERR */
//				} else {
//					fprintf(stdout,"Printing msg as XML...\n");
//					xer_fprint(stdout, &asn_DEF_StartATCMessage, internal_message);
//				}
//
//				break;
//			}
//
//			case START_DATC_MESSAGE:
//			{
//				cout << "Message is START_DATC" << endl;
//
//				char buf[MAX_START_DATC_MESSAGE_SIZE];
//
//				cout << "Deserializing start atc message" << endl;
//				int bitstream_size = h.getPayloadSize();
//				cout << "Bitstream size is " << bitstream_size << endl;
//
//				//copy the bitstream (MAYBE REMOVED)
//				for(int i=0;i<bitstream_size;i++){
//					buf[i] = data[i];
//				}
//
//				StartDATCMessage_t* internal_message = (StartDATCMessage_t*) calloc(1, sizeof(*internal_message));
//				asn_dec_rval_t rval;
//				rval = uper_decode_complete(0, &asn_DEF_StartDATCMessage,(void **)&internal_message, buf, MAX_START_DATC_MESSAGE_SIZE);
//				msg = new StartDATCMsg(internal_message);
//				msg->setSource(h.getSrcAddr());
//				msg->setDestination(h.getDstAddr());
//				msg->setTcpConnection(this);
//
//				if(rval.code != RC_OK) {
//					fprintf(stderr,
//							"Broken message encoding at byte %ld\n",
//							(long)rval.consumed);
//					exit(65); /* better, EX_DATAERR */
//				} else {
//					fprintf(stdout,"Printing msg as XML...\n");
//					xer_fprint(stdout, &asn_DEF_StartDATCMessage, internal_message);
//				}
//
//				break;
//			}
//
//			case DATA_CTA_MESSAGE:
//			{
//				cout << "Message is DATA_CTA" << endl;
//
//				char buf[MAX_DATA_CTA_MESSAGE_SIZE];
//
//				cout << "Deserializing data cta message" << endl;
//				int bitstream_size = h.getPayloadSize();
//				cout << "Bitstream size is " << bitstream_size << endl;
//
//				//copy the bitstream (MAYBE REMOVED)
//				for(int i=0;i<bitstream_size;i++){
//					buf[i] = data[i];
//				}
//
//				DataCTAMessage_t* internal_message = (DataCTAMessage_t*) calloc(1, sizeof(*internal_message));
//				asn_dec_rval_t rval;
//				rval = uper_decode_complete(0, &asn_DEF_DataCTAMessage,(void **)&internal_message, buf, MAX_DATA_CTA_MESSAGE_SIZE);
//				msg = new DataCTAMsg(internal_message);
//				msg->setSource(h.getSrcAddr());
//				msg->setDestination(h.getDstAddr());
//				msg->setTcpConnection(this);
//
//				if(rval.code != RC_OK) {
//					fprintf(stderr,
//							"Broken message encoding at byte %ld\n",
//							(long)rval.consumed);
//					exit(65); /* better, EX_DATAERR */
//				} else {
//					fprintf(stdout,"Printing msg as XML...\n");
//					xer_fprint(stdout, &asn_DEF_DataCTAMessage, internal_message);
//				}
//
//				break;
//			}
//
//			case DATA_ATC_MESSAGE:
//			{
//				cout << "Message is DATA_ATC" << endl;
//
//				char buf[MAX_DATA_ATC_MESSAGE_SIZE];
//
//				cout << "Deserializing data atc message" << endl;
//				int bitstream_size = h.getPayloadSize();
//				cout << "Bitstream size is " << bitstream_size << endl;
//
//				//copy the bitstream (MAYBE REMOVED)
//				for(int i=0;i<bitstream_size;i++){
//					buf[i] = data[i];
//				}
//
//				DataATCMessage_t* internal_message = (DataATCMessage_t*) calloc(1, sizeof(*internal_message));
//				asn_dec_rval_t rval;
//				rval = uper_decode_complete(0, &asn_DEF_DataATCMessage,(void **)&internal_message, buf, MAX_DATA_ATC_MESSAGE_SIZE);
//				msg = new DataATCMsg(internal_message);
//				msg->setSource(h.getSrcAddr());
//				msg->setDestination(h.getDstAddr());
//				msg->setTcpConnection(this);
//
//				if(rval.code != RC_OK) {
//					fprintf(stderr,
//							"Broken message encoding at byte %ld\n",
//							(long)rval.consumed);
//					exit(65); /* better, EX_DATAERR */
//				} else {
//					fprintf(stdout,"Printing msg as XML...\n");
//					xer_fprint(stdout, &asn_DEF_DataATCMessage, internal_message);
//				}
//
//				break;
//			}
//
//			case STOP_MESSAGE:
//			{
//				cout << "Message is STOP" << endl;
//				msg = new StopMsg();
//				msg->setSource(h.getSrcAddr());
//				msg->setDestination(h.getDstAddr());
//				msg->setTcpConnection(this);
//				break;
//			}
//
//			default:
//			{
//				break;
//			}
//
//			}

			//writeMsg();
			//node_manager->notify_msg(msg);
			//readHeader();

		}
		else{
			connection_manager_.stop(this);
		}
	}
	else{
		connection_manager_.stop(this);
	}
}



void Connection::handle_write(const boost::system::error_code& e)
{
	if (!e)
	{
		std::cout << "tx ok" << std::endl;
		// Initiate graceful connection closure.
		//boost::system::error_code ignored_ec;
		//socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	}

	if (e != boost::asio::error::operation_aborted)
	{
		std::cout << "tx error" << std::endl;
		//connection_manager_.stop(this);
	}
}

void Connection::writeMsg(Message* msg){

	vector<uchar> out;
	vector<uchar> temp1;
	vector<uchar> temp2;

	if(msg->getBitStream(temp1)<=0)
		cout << "error in getting the bitstream from msg!" << endl;

	Header h(msg->getSource(), msg->getDestination(), msg->msg_type, 0, 1, msg->getSeqNum(), temp1.size());
	temp2 = h.serialization();
	std::cout << h;

	out.reserve( temp1.size() + temp2.size() ); // preallocate memory
	//write header
	out.insert( out.end(), temp2.begin(), temp2.end() );
	//write payload
	out.insert( out.end(), temp1.begin(), temp1.end() );

	boost::asio::write(socket_, boost::asio::buffer(out, out.size()));
	std::cout << "writing to: " << socket_.remote_endpoint().address().to_string() << std::endl;
	std::cout << "writing to: " << socket_.remote_endpoint().port() << std::endl;
	std::cout << out.size() << std::endl;

	//if(ignored_error!=0){
	//	std::cout << "tx error" << std::endl;
	//}


}



