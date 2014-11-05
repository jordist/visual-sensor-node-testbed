/*
 * ConnectionManager.h
 *
 *  Created on: 28/oct/2014
 *      Author: jordi
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include <set>
#include <boost/noncopyable.hpp>
#include "Connection.h"

class ConnectionManager: private boost::noncopyable
{
public:

  ConnectionManager(NodeManager* nm){
	  node_manager = nm;
  }

  /// Add the specified connection to the manager and start it.
  void start(Connection* c);
  void start(sockaddr_in addrcli, Address_t srcAddr, Address_t dstAddr);

  /// Stop the specified connection.
  void stop(Connection* c);

  /// Stop all connections.
  void stop_all();
  void printActiveConnections();
  std::set<Connection*> getWiFiConnections(){
	  return connections_;
  }

  Connection* GetConnectionTo(Address_t address);
  void setSender(Sender* s);
  void setConnectionBroadcast(Connection* cn);
  Connection* GetConnectionBroadcast();
  void setRemotePort(short port);

private:
  /// The managed connections.
  std::set<Connection*> connections_;
  NodeManager* node_manager;
  Sender* sender;
  Connection* broadcast_connection = NULL;
  short remote_port_;
};


#endif /* CONNECTIONMANAGER_H_ */
