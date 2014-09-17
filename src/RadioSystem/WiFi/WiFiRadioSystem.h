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
	WiFiRadioSystem(boost::asio::ip::tcp::resolver::query, std::string mode);
	void startReceiver();

private:
	void handleConnect(const boost::system::error_code& error,
			tcp::resolver::iterator endpoint_iter);
	void handleAccept(const boost::system::error_code&);

	boost::asio::io_service io_service;
	tcp::acceptor acceptor;
	tcp::socket socket;
	ConnectionManager connection_manager;
	Connection* new_connection;


};


#endif /* WIFIRADIOSYSTEM_H_ */
