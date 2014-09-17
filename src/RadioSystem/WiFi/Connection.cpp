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

using namespace std;

Connection::Connection(boost::asio::io_service& io_service,
    ConnectionManager& manager/*, MessageHandler& handler*/)
  : socket_(io_service),
    connection_manager_(manager)/*,
    message_handler_(handler)*/
{
}

boost::asio::ip::tcp::socket& Connection::socket()
{
  return socket_;
}

void Connection::start()
{
  socket_.async_read_some(boost::asio::buffer(buffer_),
      boost::bind(&Connection::handle_read, this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void Connection::stop()
{
  socket_.close();
}

//UNDERSTAND WHAT TO DO...
void Connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{
	//something like this:
	if(!e){
		cout << "Ready to read from client..." << endl;
		//read wifi packet header and then read the rest of the message
	}
	else{
		cout << "Connection broken!" << endl;
		//connection broken, remove connection from set
		connection_manager_.stop(this);
	}

//  if (!e)
//  {
//    boost::tribool result;
//    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
//        request_, buffer_.data(), buffer_.data() + bytes_transferred);
//
//    if (result)
//    {
//      request_handler_.handle_request(request_, reply_);
//      boost::asio::async_write(socket_, reply_.to_buffers(),
//          boost::bind(&Connection::handle_write, shared_from_this(),
//            boost::asio::placeholders::error));
//    }
//    else if (!result)
//    {
//      reply_ = reply::stock_reply(reply::bad_request);
//      boost::asio::async_write(socket_, reply_.to_buffers(),
//          boost::bind(&Connection::handle_write, shared_from_this(),
//            boost::asio::placeholders::error));
//    }
//    else
//    {
//      socket_.async_read_some(boost::asio::buffer(buffer_),
//          boost::bind(&Connection::handle_read, shared_from_this(),
//            boost::asio::placeholders::error,
//            boost::asio::placeholders::bytes_transferred));
//    }
//  }
//  else if (e != boost::asio::error::operation_aborted)
//  {
//    connection_manager_.stop(shared_from_this());
//  }
}

void Connection::handle_write(const boost::system::error_code& e)
{
  if (!e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  if (e != boost::asio::error::operation_aborted)
  {
    connection_manager_.stop(this);
  }
}





