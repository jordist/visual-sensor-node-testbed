#include <stdio.h>
#include <iostream>

#include "NodeManager/NodeManager.h"
#include "RadioSystem/RadioSystem.h"
#include "RadioSystem/MessageParser.h"
#include "S2GInterface/S2GInterface.h"

using namespace std;
/*
 * USAGE:
 * testbed-v2 sink 						SINK MODE
 * testbed-v2 camera port 				CAMERA MODE, LISTENING ON PORT port FOR COOP.
 * testbed-v2 cooperator address port   COOP MPDE, CONNECTING TO address:port
 */

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
	MessageParser *msg_parser;
	//ConnectionManager *connMng;
	boost::asio::io_service io_service;

	switch(type){
	case SINK:{
		//create the main components
		nodeMng  = new NodeManager(SINK);
		msg_parser = new MessageParser();
		radioSys = new RadioSystem(nodeMng, msg_parser);
		taskMng  = new TaskManager(nodeMng);


		nodeMng->set_radioSystem(radioSys);
		nodeMng->set_taskManager(taskMng);

		//start the task manager
		taskMng->start();

		//start a telosb receiver
		radioSys->startTelosbReceiver("/dev/ttyUSB0");

		//start the sink2gui interface
		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "1234");
		tcp::resolver::iterator iterator = resolver.resolve(query);
		s2ginterface = new S2GInterface(nodeMng, msg_parser, io_service, iterator);
		s2ginterface->startInterface();
		nodeMng->set_s2gInterface(s2ginterface);



		break;
	}
	case CAMERA:{
		nodeMng  = new NodeManager(CAMERA);
		msg_parser = new MessageParser();
		radioSys = new RadioSystem(nodeMng,msg_parser);
		taskMng  = new TaskManager(nodeMng);
		//connMng = new ConnectionManager();


		nodeMng->set_radioSystem(radioSys);
		nodeMng->set_taskManager(taskMng);

		//start the task manager
		taskMng->start();

		//start a telosb receiver
		radioSys->startTelosbReceiver("/dev/ttyUSB1");

		//start the WiFi manager
		radioSys->startWiFiReceiver();

		//start the server for connection with cooperators
		//int port = atoi(argv[2]);
		//connMng->startServer(port);

		radioSys->joinTelosbReceiver();
		break;
	}
	case COOPERATOR:{
		nodeMng  = new NodeManager(COOPERATOR);
		msg_parser = new MessageParser();
		radioSys = new RadioSystem(nodeMng,msg_parser);
		taskMng  = new TaskManager(nodeMng);
		//connMng = new ConnectionManager();

		nodeMng->set_radioSystem(radioSys);
		nodeMng->set_taskManager(taskMng);


		//start the task manager
		taskMng->start();

		radioSys->startWiFiReceiver();


		//start a telosb receiver
		//radioSys->startReceiver("/dev/ttyUSB1");

		//connect to the camera
		//connMng->connectToCamera(argv[2],atoi(argv[3]));
		break;
	}
	default:

		break;
	}


	taskMng->join();


	return 0;
}
