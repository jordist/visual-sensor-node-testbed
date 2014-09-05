#ifndef RADIOSYSTEM_H
#define RADIOSYSTEM_H

#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include "Messages/Message.h"
#include "Telosb/serialsource.h"

class NodeManager;
class TelosbRadioSystem;
class IncomingMessageQueue;

class RadioSystem{

public:
	RadioSystem(NodeManager *nm);
	/*void set_nodeManager(NodeManager *nm){
		nodeManager_ptr = nm;
		cout << "RS node manager set " << nm << endl;
		cout << "RS node manager... " << nodeManager_ptr << endl;
	}*/

	/*void start(){
		m_thread = boost::thread(&RadioSystem::radioSystemThread, this);
	}
	void join(){
		m_thread.join();
	}*/
	int startReceiver(string dev_name);
	void joinReceiver();
	serial_source getTelosb();
	void notifyMsg(Message* msg);

private:

	NodeManager *nodeManager_ptr;
	TelosbRadioSystem *telosbRadioSystem_ptr;
	IncomingMessageQueue *incoming_message_queue_ptr;
	//boost::thread m_thread;
	//void radioSystemThread();

};

#endif
