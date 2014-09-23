/*
 * S2GInterface.h
 *
 *  Created on: 15/lug/2014
 *      Author: greeneyes
 */

#ifndef S2GINTERFACE_H_
#define S2GINTERFACE_H_


#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <opencv2/core/core.hpp>
#include "Messages/Header.h"

class NodeManager;


using boost::asio::ip::tcp;
using namespace std;



class S2GInterface{

public:
	S2GInterface(NodeManager* nm, boost::asio::io_service& io_service,
			tcp::resolver::iterator endpoint_iterator);
	void startInterface(){
		r_thread = boost::thread(&S2GInterface::runThread, this);
	}
	void runThread();
	void writeMsg(Message* msg);




private:
	void stop();
	void start_read_header();
	void handle_read_message(const boost::system::error_code& ec);


	Header parseHeader(uchar data[]);
	void data_sent_handler(const boost::system::error_code& ec);
	void handle_connect(const boost::system::error_code& error,
			tcp::resolver::iterator endpoint_iter);
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	boost::thread r_thread;
	uchar header_[HEADER_SIZE];
	vector<short> op_mode_;
	vector<uchar> buffer_;
	vector<int> cta_cur_slice_idx_;
	vector<int> atc_cur_slice_idx_;
	vector<int> cta_cur_frame_idx_;
	vector<int> atc_cur_frame_idx_;
	vector<int> messageId_;
	vector<int> packetId_;

	NodeManager* node_manager;
};



#endif /* S2GINTERFACE_H_ */
