/*
 * ConnectionManager.cpp
 *
 *  Created on: 28/oct/2014
 *      Author: jordi
 */


#include "ConnectionManager.h"
#include <algorithm>
#include <boost/bind.hpp>
#include "NodeManager/NodeManager.h"

void ConnectionManager::printActiveConnections(){
	std::cout << "Active connections: " << connections_.size() << std::endl;
	std::set<Connection*>::iterator it;
	for (it = connections_.begin(); it != connections_.end(); ++it)
	{
		Connection* cn = *it; // Note the "*" here
		std::string sClientIp = cn->getRemoteIP();
		unsigned short uiClientPort = cn->getRemotePort();
		std::cout << sClientIp << "		" << uiClientPort << std::endl;
		std::cout << "  src=" << cn->srcAddress << "\tdst=" << cn->dstAddress << std::endl;
	}
}


void ConnectionManager::start(Connection* c)
{
	connections_.insert(c);
	if(node_manager->getNodeType() == CAMERA)
		node_manager->notifyCooperatorOnline(c);
//	c->readHeader();
	printActiveConnections();
}

void ConnectionManager::start(sockaddr_in addrcli, Address_t srcAddr, Address_t dstAddr) {
	Connection* c = new Connection(*this);
	c->setNodeManager(node_manager);
	c->setSender(sender);
	addrcli.sin_port=htons(remote_port_);
	c->setConnection(&addrcli, &dstAddr, &srcAddr, 0);


	connections_.insert(c);
	if(node_manager->getNodeType() == CAMERA)
		node_manager->notifyCooperatorOnline(c);
std::cerr << "======> CAMERA: Added new cooperator" << std::endl;
//	c->readHeader();
	printActiveConnections();
}

void ConnectionManager::stop(Connection* c)
{
	connections_.erase(c);
	if(node_manager->getNodeType() == CAMERA)
		node_manager->notifyCooperatorOffline(c);
	c->stop();
	printActiveConnections();

}

void ConnectionManager::stop_all()
{
	std::for_each(connections_.begin(), connections_.end(),
			boost::bind(&Connection::stop, _1));
	connections_.clear();
}

Connection* ConnectionManager::GetConnectionTo(Address_t address) {
	std::set<Connection*>::iterator it;
	for (it = connections_.begin(); it != connections_.end(); ++it)
	{
		Connection* cn = *it; // Note the "*" here
		if(cn->dstAddress == address){
			return cn;
		}
	}
	return NULL;
}

void ConnectionManager::setSender(Sender* s) {
	sender = s;
}

void ConnectionManager::setConnectionBroadcast(Connection* cn) {
	broadcast_connection = cn;
}

Connection* ConnectionManager::GetConnectionBroadcast() {
	return broadcast_connection;
}

void ConnectionManager::setRemotePort(short port) {
	remote_port_ = port;
}
