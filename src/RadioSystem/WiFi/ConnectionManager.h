/*
 * ConnectionManager.h
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */

#ifndef CONNECTIONMANAGER_H_
#define CONNECTIONMANAGER_H_

#include <set>
#include <boost/noncopyable.hpp>
#include "Connection.h"

class ConnectionManager: private boost::noncopyable
{
public:
  /// Add the specified connection to the manager and start it.
  void start(Connection* c);

  /// Stop the specified connection.
  void stop(Connection* c);

  /// Stop all connections.
  void stop_all();
  void printActiveConnections();

private:
  /// The managed connections.
  std::set<Connection*> connections_;
};


#endif /* CONNECTIONMANAGER_H_ */
