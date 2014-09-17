/*
 * ConnectionManager.cpp
 *
 *  Created on: 16/set/2014
 *      Author: greeneyes
 */


#include "ConnectionManager.h"
#include <algorithm>
#include <boost/bind.hpp>



void ConnectionManager::start(Connection_ptr c)
{
  connections_.insert(c);
  c->start();
}

void ConnectionManager::stop(Connection_ptr c)
{
  connections_.erase(c);
  c->stop();
}

void ConnectionManager::stop_all()
{
  std::for_each(connections_.begin(), connections_.end(),
      boost::bind(&Connection::stop, _1));
  connections_.clear();
}

