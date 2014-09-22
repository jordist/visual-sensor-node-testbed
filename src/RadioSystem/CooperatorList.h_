/*
 * cooperatorList.h
 *
 *  Created on: 05/giu/2014
 *      Author: antlab
 */

#include<iostream>
#include<vector>
#include "boost/thread/condition.hpp"

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
#include <unistd.h>


using namespace std;

typedef struct cooperator{
	int socket;
	sockaddr_in address;

	float bandwidth;
	float CPUspeed;

	float load;

	int id;
} cooperator;

class CooperatorList {
public:
	CooperatorList();
	virtual ~CooperatorList();

	void addCooperator(cooperator coop);
	void deleteCooperator(int coop_id);
	int size();
	cooperator* getCooperator(int idx);

private:
	vector<cooperator> cooperators;
	mutable boost::mutex the_mutex;

	void printList();


};

