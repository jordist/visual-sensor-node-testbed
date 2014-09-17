/*
 * WiFiRadioSystem.h
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */

#ifndef WIFIRADIOSYSTEM_H_
#define WIFIRADIOSYSTEM_H_


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include "ConnectionManager.h"
#include "Connection.h"

using boost::asio::ip::tcp;

class WiFiRadioSystem{

public:
	//server mode
	WiFiRadioSystem(const std::string& address, const std::string& port);

	//client mode
	WiFiRadioSystem(tcp::resolver::query query);

	void run();
	void stop();
	void connectToCamera(tcp::resolver::query query);

private:
	 void handle_accept(const boost::system::error_code& e);
	  void handle_stop();
	  void handle_connect(const boost::system::error_code& error,
	  		tcp::resolver::iterator endpoint_iter);
	  boost::asio::io_service io_service_;
	  boost::asio::ip::tcp::acceptor acceptor_;
	  tcp::socket socket_;
	  ConnectionManager connection_manager_;

	  /// The next connection to be accepted.
	  Connection_ptr new_connection_;

	  /// The handler for all incoming requests.
	  //request_handler request_handler_;
};


#endif /* WIFIRADIOSYSTEM_H_ */
