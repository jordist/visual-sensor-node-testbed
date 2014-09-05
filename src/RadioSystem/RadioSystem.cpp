#include <stdlib.h>
#include <iostream>
#include "RadioSystem/RadioSystem.h"
#include "NodeManager/NodeManager.h"

using namespace std;

RadioSystem::RadioSystem(NodeManager* nm){
	nodeManager_ptr = nm;
	incoming_message_queue_ptr = new IncomingMessageQueue(this);
	telosbRadioSystem_ptr = new TelosbRadioSystem();
	telosbRadioSystem_ptr->setIncomingMessageQueue(incoming_message_queue_ptr);
}


int RadioSystem::startReceiver(string dev_name){
	if(telosbRadioSystem_ptr->openRadio(dev_name.c_str(),115200,0)==0){
		telosbRadioSystem_ptr->startReceiver();
		cout << "RS: telosb ready!" << endl;
		return 0;
	}
	cout << "RS: Error in opening radio device" << endl;
	return -1;
}

void RadioSystem::joinReceiver(){
	telosbRadioSystem_ptr->joinReceiver();
}

serial_source RadioSystem::getTelosb(){
	return telosbRadioSystem_ptr->getTelosb();
}

void RadioSystem::notifyMsg(Message* msg){
	cout << "RS: Notifying message to " << nodeManager_ptr << endl;
	nodeManager_ptr->notify_msg(msg);
}

//void RadioSystem::radioSystemThread(){
//	int counter=0;
//	while(0){
//
//		for(int op=0; op<3; op++){
//			// simulate the transmission delay
//			int wait_time = 103000;
//			usleep(wait_time);
//
//			// simulate the reception of messages
//			int data1 = rand() % 10 + 1;
//			int data2 = rand() % 10 + 1;
//
//			message *msg = new message(data1,data2,op);
//			cout << "RS: Message received: " << op << "," << data1 << "," << data2 << endl;
//			// notify the message
//			nodeManager_ptr->notify_msg(msg);
//		}
//		counter++;
//		if(counter==30){
//			sleep(3);
//			counter = 0;
//		}
//	}
//
//}
