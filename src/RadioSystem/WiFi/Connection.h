#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "opencv2/core/core.hpp"
#include "Messages/Message.h"
#include "Messages/Header.h"

#include "ASN1/PacketHeader.h"
#include "ASN1/Address.h"
#include "ASN1/MessageTypes.h"
#include "ASN1/ReceptionReportModes.h"
//#include "ASN1/ReceptionReportMessage.h"
//#include "ASN1/ReceptionReportRequestMessage.h"

#define CONN_UNICAST	0
#define CONN_BROADCAST	1

class NodeManager;
class ConnectionManager;
class Message;
class MessageParser;
class Sender;

class Connection
{
public:
	explicit Connection(ConnectionManager& manager);

	std::string getRemoteIP();
	int getRemotePort();

/*	/// Start the first asynchronous operation for the connection.
	//void start();
	void readHeader();
*/

	/// Stop all asynchronous operations associated with the connection.
	void stop();

	void writeMsg(Message* msg);
	void writeMsgBroadcast(Message* msg, Connection* cn2);
	void setNodeManager(NodeManager* nm);
	void setMessageParser(MessageParser* m);
	void setSender(Sender* s);
	void notifyMsgReceived(uchar* msg, int msg_len);

//Connection(sockaddr_in *address, Address_t *source_address, Address_t *destination_address, int conn_type);
	void setConnection(sockaddr_in *address, Address_t *source_address, Address_t *destination_address, int conn_type);
	void setConnectionBroadcast(sockaddr_in *address, Address_t *source_address);
	int getLastMessageID(); //Returns msgID
	int getNextMessageID(); //Increments msgID and returns the new msgID
	int getLastReceivedMessageID();
	void setLastReceivedMessageID(int messageID);
	int isBroadcast();
	int getNumBroadcastNodes();
	void setNumBroadcastNodes(int n);
	void setTxTime(double t);
	double getTxTime();

	struct sockaddr_in addr;
	Address_t srcAddress;
	Address_t dstAddress;
	Address_t dstAddress2;


private:
	int msgID; //Last used msgID
	int LastReceivedMessageID_;
	int is_broadcast_;
	int num_broadcast_nodes_;
	double txTime;


/*

	/// Handle completion of a read operation.

	void data_sent_handler(const boost::system::error_code& ec);

	void handleReadMessage(const boost::system::error_code& ec);
	Header parseHeader(uchar data[]);

	/// Handle completion of a write operation.
	void handle_write(const boost::system::error_code& e);

	void parseMessage(const boost::system::error_code& error, size_t bytes_transferred);


	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket_;
*/

	/// The manager for this connection.
	ConnectionManager& connection_manager_;


	uchar header_[HEADER_SIZE];
	vector<uchar> out;
	vector<uchar> readBuffer_;
	Header* h;


	NodeManager* node_manager;
	MessageParser* message_parser;
	Sender* sender;

};

//typedef boost::shared_ptr<Connection> Connection_ptr;

#endif /* CONNECTION_H_ */
