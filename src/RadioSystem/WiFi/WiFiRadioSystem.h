/*
 * WiFiRadioSystem.h
 *
 *  Created on: 28/oct/2014
 *      Author: jordi
 */

#ifndef WIFIRADIOSYSTEM_H_
#define WIFIRADIOSYSTEM_H_


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include "ConnectionManager.h"
#include "Connection.h"
#include "Messages/Header.h"
#include "Receiver.h"
#include "Sender.h"
#include "Listener.h"

using boost::asio::ip::tcp;

class WiFiRadioSystem{

public:
	WiFiRadioSystem(string ip_address, string port, string ip_address2, string port2, std::string mode, NodeManager* nm);
	void startReceiver();
	std::set<Connection*> getWiFiConnections();

private:
//	void handleConnect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iter,  Connection* connection);
//	void handleAccept(const boost::system::error_code&);

//	boost::asio::io_service io_service;
//	tcp::acceptor acceptor;
	ConnectionManager connection_manager;
	Connection* new_connection;

	NodeManager* node_manager;

	Retransmission* retrans_;
	Sender* sender_;
	Receiver* receiver_;
	Listener* listener_;

};


#endif /* WIFIRADIOSYSTEM_H_ */
