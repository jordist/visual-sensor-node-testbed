#include "Connection.h"
#include <vector>
#include <boost/bind.hpp>
#include "ConnectionManager.h"
#include "NodeManager/NodeManager.h"
#include "RadioSystem/MessageParser.h"


using namespace std;

Connection::Connection(ConnectionManager& manager) : connection_manager_(manager)
{
}

void Connection::setNodeManager(NodeManager* nm){
	node_manager = nm;
}

void Connection::setMessageParser(MessageParser* m){
	message_parser = m;
}

void Connection::stop()
{
//	socket_.close();
}

//void Connection::readHeader()
//{
//	cout << "Ready to read from client..." << endl;
//	//read wifi packet header and then read the rest of the message
//	async_read(socket_,
//			boost::asio::buffer(header_, sizeof(header_)),
//			boost::bind(&Connection::handleReadMessage, this, boost::asio::placeholders::error));
//}
//
//
//void Connection::handleReadMessage(const boost::system::error_code& ec)
//{
//
//	cout << "Header received" << endl;
//
//
//	if (!ec)
//	{
//		cout << "Received new message header" << endl;
//
//		h = message_parser->parseHeader(header_);
//		//Header h = parseHeader(header_);
//		cout << h << endl;
//
//		if (h->getNumPackets()>1){
//			std::cout << "Error: packetization is not supported for this type of message \n";
//			stop();
//		}
//
//		//uchar data[h->getPayloadSize()];
//		readBuffer_.clear();
//		readBuffer_.resize(h->getPayloadSize());
//		boost::system::error_code ec;
//
//		//read the payload
//		/*socket_.read_some(
//				boost::asio::buffer(data, h->getPayloadSize()), ec);*/
//
//		async_read(socket_,boost::asio::buffer(readBuffer_, h->getPayloadSize()),
//				boost::bind(&Connection::parseMessage, this,
//						boost::asio::placeholders::error,
//						boost::asio::placeholders::bytes_transferred));
//
//		/*if(!ec){
//			Message* msg = message_parser->parseMessage(h,data,this);
//			node_manager->notify_msg(msg);
//			readHeader();
//		}
//		else{
//			connection_manager_.stop(this);
//		}*/
//	}
//	else{
//		connection_manager_.stop(this);
//	}
//}

std::string Connection::getRemoteIP() {
	return std::string(inet_ntoa(addr.sin_addr));
}

int Connection::getRemotePort() {
	return ntohs(addr.sin_port);
}

//void Connection::parseMessage(const boost::system::error_code& error, size_t bytes_transferred){
//	if(!error){
//		Message* msg = message_parser->parseMessage(h,&readBuffer_[0],this);
//		if(msg!=NULL)
//			node_manager->notify_msg(msg);
//		//delete(h);
//		readHeader();
//	}
//	else{
//		connection_manager_.stop(this);
//	}
//}


/*void Connection::handle_write(const boost::system::error_code& e)
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
}*/

void Connection::writeMsg(Message* msg){
	vector<uchar> temp1;
	vector<uchar> temp2;

	if(msg->getBitStream(temp1)<=0)
		cout << "error in getting the bitstream from msg!" << endl;

	Header h(msg->getSource(), msg->getDestination(), msg->msg_type, 0, 1, msg->getSeqNum(), temp1.size());
	temp2 = h.serialization();
//	std::cout << h;

	uint8_t *out;
	int out_len = temp1.size() + temp2.size();
	out=(uint8_t*)malloc(out_len);
	//Write header
	for(size_t i=0; i<temp2.size(); i++){
		out[i]=temp2[i];
	}

	//Write payload
	int offset = temp2.size();
	for(size_t i=0; i<temp1.size(); i++){
		out[offset+i]=temp1[i];
	}
//	std::cerr << "Connection: Calling Send\n";
//	sender->SendBlocking(out, out_len, MessageTypes_dataMsg, this);
	sender->Send(out, out_len, MessageTypes_dataMsg, this);
	free(out);
}

void Connection::setConnection(sockaddr_in* address, Address_t* source_address,
		Address_t* destination_address, int conn_type) {
	memcpy((sockaddr_in*) &addr, (sockaddr_in*)address, sizeof(sockaddr_in));
	memcpy((Address_t*) &srcAddress, (Address_t*) source_address, sizeof(Address_t));
	memcpy((Address_t*) &dstAddress, (Address_t*) destination_address, sizeof(Address_t));
	msgID=0;
	LastReceivedMessageID_=0;
	if(conn_type==CONN_BROADCAST){
		is_broadcast_=1;
	}else{
		is_broadcast_=0;
	}
	num_broadcast_nodes_=1; //default, set with setBroadcastNodes(int n)
	dstAddress2 = 0; //No broadcast/multicast, so 0
}

void Connection::setConnectionBroadcast(sockaddr_in* address,
		Address_t* source_address) {
	memcpy((sockaddr_in*) &addr, (sockaddr_in*)address, sizeof(sockaddr_in));
	memcpy((Address_t*) &srcAddress, (Address_t*) source_address, sizeof(Address_t));
	msgID=0;
	LastReceivedMessageID_=0;
	is_broadcast_=1;
	num_broadcast_nodes_=2;
}

int Connection::getLastMessageID(){ //Returns msgID
	return msgID;
}

int Connection::getNextMessageID(){ //Increments msgID and returns the new msgID
	if(msgID==65535){ //We are using 2 bytes for msgID
		msgID=0;
	}else{
		msgID++;
	}
	return msgID;
}

int Connection::getLastReceivedMessageID(){
	return LastReceivedMessageID_;
}

void Connection::setLastReceivedMessageID(int messageID){
	LastReceivedMessageID_=messageID;
}

int Connection::isBroadcast(){
	return is_broadcast_;
}

void Connection::setSender(Sender* s) {
	sender = s;
}

void Connection::notifyMsgReceived(uchar* data, int data_len) {
	h = message_parser->parseHeader(data);
	Message* msg = message_parser->parseMessage(h, &data[sizeof(header_)],this);
	delete[] data;
	if(msg!=NULL){
		node_manager->notify_msg(msg);
	}
}

void Connection::writeMsgBroadcast(Message* msg, Connection* cn2) {
	Connection* cn = connection_manager_.GetConnectionBroadcast();
	if(cn->isBroadcast()==0 || cn==NULL){
		std::cerr << "Error, tried to broadcast to non-broadcast connection" << std::endl;
		return;
	}
	cn->dstAddress = dstAddress;
	cn->dstAddress2 = cn2->dstAddress;
	cn->writeMsg(msg);
}

void Connection::setNumBroadcastNodes(int n){
	num_broadcast_nodes_=n;
}

int Connection::getNumBroadcastNodes(){
	return num_broadcast_nodes_;
}

void Connection::setTxTime(double t) {
	txTime = t;
}

double Connection::getTxTime() {
	return txTime;
}
