#include <stdio.h>
#include <iostream>

#include "NodeManager/NodeManager.h"
#include "RadioSystem/RadioSystem.h"
#include "RadioSystem/ConnectionManager.h"
#include "Messages/Message.h"
#include "S2GInterface/S2GInterface.h"

using namespace std;

int main(int argc, char ** argv){

	NodeType type;
	if(strcmp(argv[1],"sink")==0){
		type = SINK;
	}
	if(strcmp(argv[1],"camera")==0){
		type = CAMERA;
	}
	if(strcmp(argv[1],"cooperator")==0){
		type = COOPERATOR;
	}

	NodeManager *nodeMng;
	RadioSystem *radioSys;
	TaskManager *taskMng;
	S2GInterface *s2ginterface;
	ConnectionManager *connMng;
	boost::asio::io_service io_service;

	switch(type){
	case SINK:{
		//create the main components
		nodeMng  = new NodeManager(SINK);
		radioSys = new RadioSystem(nodeMng);
		taskMng  = new TaskManager(nodeMng);
		nodeMng->set_radioSystem(radioSys);
		nodeMng->set_taskManager(taskMng);

		//start the task manager
		taskMng->start();

		//start a telosb receiver
		radioSys->startReceiver("/dev/ttyUSB0");

		//start the sink2gui interface
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "1234");
		tcp::resolver::iterator iterator = resolver.resolve(query);
		s2ginterface = new S2GInterface(nodeMng, io_service, iterator);
		s2ginterface->startInterface();
		nodeMng->set_s2gInterface(s2ginterface);

		break;
	}
	case CAMERA:{
		nodeMng  = new NodeManager(CAMERA);
		radioSys = new RadioSystem(nodeMng);
		taskMng  = new TaskManager(nodeMng);
		nodeMng->set_radioSystem(radioSys);
		nodeMng->set_taskManager(taskMng);

		//start the task manager
		taskMng->start();

		//start a telosb receiver
		radioSys->startReceiver("/dev/ttyUSB1");

		//start the server for connection with cooperators
		//connMng->startServer(port);
		break;
	}
	case COOPERATOR:{
		nodeMng  = new NodeManager(COOPERATOR);
		radioSys = new RadioSystem(nodeMng);
		taskMng  = new TaskManager(nodeMng);
		connMng = new ConnectionManager();

		nodeMng->set_radioSystem(radioSys);
		nodeMng->set_taskManager(taskMng);


		//start the task manager
		taskMng->start();

		//start a telosb receiver
		radioSys->startReceiver("/dev/ttyUSB1");

		//connect to the camera
		//connMng->connectToCamera(address,port);
		break;
	}
	default:

		break;
	}

	radioSys->joinReceiver();
	taskMng->join();


	return 0;
}
