/*
 * Connection.h
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */

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

//#include "reply.hpp"
//#include "request.hpp"
//#include "MessageHandler.h"
//#include "request_parser.hpp"
class NodeManager;
class ConnectionManager;
class Message;

class Connection
{
public:
	/// Construct a connection with the given io_service.
	explicit Connection(boost::asio::io_service& io_service,
			ConnectionManager& manager/*, MessageHandler& handler*/);

	/// Get the socket associated with the connection.
	boost::asio::ip::tcp::socket& socket();

	/// Start the first asynchronous operation for the connection.
	//void start();
	void readHeader();

	/// Stop all asynchronous operations associated with the connection.
	void stop();

	void writeMsg(Message* msg);
	void setNodeManager(NodeManager* nm);


private:
	/// Handle completion of a read operation.


	void handleReadMessage(const boost::system::error_code& ec);
	Header parseHeader(uchar data[]);

	/// Handle completion of a write operation.
	void handle_write(const boost::system::error_code& e);

	/// Socket for the connection.
	boost::asio::ip::tcp::socket socket_;

	/// The manager for this connection.
	ConnectionManager& connection_manager_;

	/// The handler used to process the incoming request.
	//MessageHandler& message_handler_;

	/// Buffer for incoming data.
	boost::array<char, 8192> buffer_;

	uchar header_[HEADER_SIZE];

	NodeManager* node_manager;

	/// The incoming request.
	//request request_;
	//Message message_;

	/// The parser for the incoming request.
	//request_parser request_parser_;
	//MessageParser message_parser;

};

//typedef boost::shared_ptr<Connection> Connection_ptr;

#endif /* CONNECTION_H_ */
