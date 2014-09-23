/*
 * ConnectionManager.h
 *
 *  Created on: 04/giu/2014
 *      Author: antlab
 */

#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<poll.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include <RadioSystem/CooperatorList.h>


using namespace std;



class ConnectionManager {
public:
	ConnectionManager();
	virtual ~ConnectionManager();

	void startListener(){
		l_thread = boost::thread(&ConnectionManager::listenerThread, this);
	}

	void startMantainer(){
		m_thread = boost::thread(&ConnectionManager::mantainerThread, this);
	}

	void joinListener(){
		l_thread.join();
	}

	void joinMantainer(){
		m_thread.join();
	}

	int startServer(int port);
	int connectToCamera(char* camera_ip_address, int port);


private:
	boost::thread l_thread,m_thread;
	int my_socket;
	sockaddr_in my_address;

	//listen for connections
	void listenerThread();

	//check if a client is still connected or not
	void mantainerThread();
	int openSocket();
	int bindSocket(int port);

	CooperatorList cooperators;


};
