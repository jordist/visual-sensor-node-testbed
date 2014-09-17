/*
 * ConnectionManager.cpp
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */


#include "ConnectionManager.h"
#include <algorithm>
#include <boost/bind.hpp>

void ConnectionManager::printActiveConnections(){
	std::cout << "Active connections: " << connections_.size() << std::endl;
	std::set<Connection*>::iterator it;
	for (it = connections_.begin(); it != connections_.end(); ++it)
	{
		Connection* cn = *it; // Note the "*" here
		boost::asio::ip::tcp::socket& sock = cn->socket();
		std::string sClientIp = sock.remote_endpoint().address().to_string();
		unsigned short uiClientPort = sock.remote_endpoint().port();
		std::cout << sClientIp << "		" << uiClientPort << std::endl;
	}
}


void ConnectionManager::start(Connection* c)
{
	connections_.insert(c);
	c->start();
	printActiveConnections();
}

void ConnectionManager::stop(Connection* c)
{
	connections_.erase(c);
	c->stop();
	printActiveConnections();
}

void ConnectionManager::stop_all()
{
	std::for_each(connections_.begin(), connections_.end(),
			boost::bind(&Connection::stop, _1));
	connections_.clear();
}

