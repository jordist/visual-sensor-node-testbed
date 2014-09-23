/*
 * cooperatorList.cpp
 *
 *  Created on: 05/giu/2014
 *      Author: antlab
 */

#include "CooperatorList.h"

CooperatorList::CooperatorList() {
	// TODO Auto-generated constructor stub

}

CooperatorList::~CooperatorList() {
	// TODO Auto-generated destructor stub
}

void CooperatorList::addCooperator(cooperator coop){
	boost::mutex::scoped_lock lock(the_mutex);
	cooperators.push_back(coop);
	cout << "CL: cooperator added " << endl;
	printList();
}

void CooperatorList::deleteCooperator(int idx){
	boost::mutex::scoped_lock lock(the_mutex);
	close(cooperators[idx].socket);
	cooperators.erase(cooperators.begin() + idx);
	cout << "CL: cooperator removed " << endl;
	printList();
}

int CooperatorList::size(){
	boost::mutex::scoped_lock lock(the_mutex);
	return cooperators.size();
}

cooperator* CooperatorList::getCooperator(int idx){
	boost::mutex::scoped_lock lock(the_mutex);
	return &cooperators[idx];
}

void CooperatorList::printList(){
	cout << "ID \t ADDRESS \t PORT" << endl;
	for(unsigned int i=0;i<cooperators.size();i++){
		cout << cooperators[i].id << "\t" << inet_ntoa(cooperators[i].address.sin_addr) << "\t" << ntohs(cooperators[i].address.sin_port) << endl;
	}
}
