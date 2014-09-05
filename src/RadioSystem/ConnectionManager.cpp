/*
 * ConnectionManager.cpp
 *
 *  Created on: 04/giu/2014
 *      Author: antlab
 */

#include "ConnectionManager.h"

ConnectionManager::ConnectionManager() {}

ConnectionManager::~ConnectionManager() {}

void ConnectionManager::listenerThread(){

	//this is the ID that will be assigned to cooperators
	int coop_id = 0;
	while(1){

		//local variables
		int client_socket;
		sockaddr_in client_address;
		unsigned int slen=sizeof(client_address);

		//Listen for client to connect
		if(listen(my_socket,1)==-1){
			cout << "CM: Error during listen()" << endl;
			break;
		}

		client_socket = accept(my_socket,(struct sockaddr*)&client_address,&slen);
		cout << "CM: Connection accepted!" << endl;

		//add the cooperator to the cooperator list
		cooperator coop;
		coop.socket = client_socket;
		coop.address = client_address;
		coop.id = coop_id;
		cooperators.addCooperator(coop);
		coop_id++;
	}
}

void ConnectionManager::mantainerThread(){

	while(1){
		char buffer[32];
		int status;
		for(int i=0;i<cooperators.size();i++){
			int socket = cooperators.getCooperator(i)->socket;
			int rec_bytes = recv(socket, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT);
			if (rec_bytes == 0) {
				cooperators.deleteCooperator(i);
			}
		}
		sleep(1);
	}

}

int ConnectionManager::openSocket(){
	my_socket=socket(AF_INET, SOCK_STREAM, 0);
	return my_socket;
}

int ConnectionManager::bindSocket(int port){
	memset((char *) &my_address, 0, sizeof(my_address));
	my_address.sin_family = AF_INET;
	my_address.sin_port = htons(port);
	my_address.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(my_socket, (struct sockaddr *)&my_address, sizeof(my_address))==-1){
		cout << "CM: Wrong binding. The server is NOT running" << endl;
		return -1;
	}
	else
		return 0;
}

int ConnectionManager::startServer(int port){
	if(openSocket()<0){
		cout << "CM: error opening socket" << endl;
		return -1;
	}
	else{
		cout << "CM: socket opened. Binding..." << endl;
		if(bindSocket(port)<0){
			cout << "CM: bind error" << endl;
			return -1;
		}
		else{
			cout << "CM: Binding done. Starting Listener + Mantainer." << endl;
			startListener();
			startMantainer();
		}
		return 0;
	}
}

int ConnectionManager::connectToCamera(char* camera_ip_address, int port){
	sockaddr_in camera_address;
	bzero(&camera_address, sizeof(camera_address));
	camera_address.sin_family = AF_INET;
	camera_address.sin_port = htons(port);

	if(openSocket()<0){
		cout << "CM: error opening socket" << endl;
		return -1;
	}

	if(inet_aton(camera_ip_address,&camera_address.sin_addr)==0){
		cout << "CM: camera IP address not correct" << endl;
		return -1;
	}

	if(connect(my_socket,(const struct sockaddr*)&camera_address,sizeof(camera_address))<0){
		cout << "CM: Connection refused" << endl;
		return -1;
	}
	cout << "CM: Connected to camera!" << endl;
	return 0;
}



