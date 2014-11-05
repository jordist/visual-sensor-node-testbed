/*
 * WiFiRadioSystem.cpp
 *
 *  Created on: 28/oct/2014
 *      Author: jordi
 */

#include "WiFiRadioSystem.h"
#include <boost/bind.hpp>
#include "NodeManager/NodeManager.h"


WiFiRadioSystem::WiFiRadioSystem(string ip_address_local, string port_local, string ip_address2, string port2, std::string mode, NodeManager* nm) :
connection_manager(nm),
new_connection(new Connection(connection_manager)),
node_manager(nm)
{
	new_connection->setNodeManager(node_manager);

	retrans_ = new Retransmission;
	sender_ = new Sender(retrans_);
	sender_->StartThread();
	receiver_ = new Receiver(sender_, retrans_, &connection_manager, inet_addr(ip_address_local.c_str()));
	receiver_->StartThread();
	listener_ = new Listener(atoi(port_local.c_str()), receiver_);
	listener_->StartThread();
	connection_manager.setSender(sender_);
	connection_manager.setRemotePort(atoi(port2.c_str()));

	if(mode.compare("server") == 0){
		//Set connection to broadcast:
		Connection* cn = new Connection(connection_manager);
		cn->setNodeManager(node_manager);
		cn->setSender(sender_);
		struct sockaddr_in addr;
		memset(&addr,0,sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port =htons(atoi(port2.c_str()));
		inet_aton(ip_address2.c_str(), &(addr.sin_addr));
		Address_t srcAddr = inet_addr(ip_address_local.c_str()); //TODO
		cn->setConnectionBroadcast(&addr, &srcAddr);
		connection_manager.setConnectionBroadcast(cn);
	}

	if(mode.compare("client") == 0){
		Connection* cn = new Connection(connection_manager);
		cn->setNodeManager(node_manager);
		cn->setSender(sender_);
		struct sockaddr_in addr;
		memset(&addr,0,sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port =htons(atoi(port2.c_str()));
		inet_aton(ip_address2.c_str(), &(addr.sin_addr));
		Address_t srcAddr = inet_addr(ip_address_local.c_str());
		Address_t dstAddr = inet_addr(ip_address2.c_str());
		cn->setConnection(&addr, &srcAddr, &dstAddr, 0);
		connection_manager.start(cn);

		sender_->Send(NULL, 0, MessageTypes_connectMsg, cn);
	}
}

void WiFiRadioSystem::startReceiver(){

}

//void WiFiRadioSystem::startReceiver(){
//	io_service.run();
//}

//void WiFiRadioSystem::handleConnect(const boost::system::error_code& error,
//		tcp::resolver::iterator endpoint_iter, Connection* connection){
//	if (!connection->socket().is_open())
//	{
//		std::cout << "Connect timed out\n";
//	}
//
//	else if (error)
//	{
//		std::cout << "Connect error: " << error.message() << "\n";
//
//		connection->socket().close();
//	}
//	// Otherwise we have successfully established a connection.
//	else
//	{
//		std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";
//
//		// Start reading the header
//		std::cout << "Start reading..." << std::endl;
//		connection_manager.start(connection);
//		//start_read_header();
//	}
//}

//void WiFiRadioSystem::handleAccept(const boost::system::error_code& e){
//	if (!e)
//	{
//		std::cout << "connected..." << std::endl;
//		connection_manager.start(new_connection);
//
//
//		new_connection = new Connection(io_service,
//				connection_manager);
//		new_connection->setNodeManager(node_manager);
//		acceptor.async_accept(new_connection->socket(),
//				boost::bind(&WiFiRadioSystem::handleAccept, this,
//						boost::asio::placeholders::error));
//
//
//	}
//}

std::set<Connection*> WiFiRadioSystem::getWiFiConnections(){
	return connection_manager.getWiFiConnections();
}

