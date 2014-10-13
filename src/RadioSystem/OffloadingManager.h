/*
 * OffloadingManager.h
 *
 *  Created on: 19/set/2014
 *      Author: greeneyes
 */

#ifndef OFFLOADINGMANAGER_H_
#define OFFLOADINGMANAGER_H_
#include <opencv2/opencv.hpp>
#include <vector>
#include "NodeManager/NodeManager.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



using namespace std;
using namespace cv;

#define F 1514
#define H 66


class Connection;

typedef struct cooperator{
	Connection* connection;

	double bandwidth;
	double CPUspeed;

	double load;
	Mat image_slice;
	int col_offset;

	double detTime;
	double descTime;
	double kencTime;
	double fencTime;
}cooperator;

class OffloadingManager{
public:
	OffloadingManager(NodeManager* nm):
	io(),
	t(io),
	work(io)
	{
		cooperators_to_use = 0;
		received_cooperators = 0;
		node_manager = nm;
		startTimer();
	}

	//add keypoints and features from cooperators
	void addKeypointsAndFeatures(vector<KeyPoint>& kpts,Mat& features,Connection* cn,
			double detTime, double descTime, double kencTime, double fencTime);

	//reset variables and keep track of progresses
	void createOffloadingTask(int num_cooperators);

	void addCooperator(Connection* c);
	void removeCooperator(Connection* c);
	Mat computeLoads(Mat& image);
	void transmitLoads();
	void transmitStartDATC(StartDATCMsg* msg);
	int probeLinks();
	void sortCooperators();
	void timerExpired(const boost::system::error_code& error);
	void startTimer();
	void runThread();
	int getNumAvailableCoop();

private:

	int cooperators_to_use;
	int received_cooperators;
	vector<cooperator> cooperatorList;

	NodeManager* node_manager;
	//used to store keypoints and features from cooperators
	vector<KeyPoint> keypoint_buffer;
	Mat features_buffer;
	boost::thread r_thread;
	double camDetTime, camDescTime, camkEncTime, camfEncTime;

	boost::asio::io_service io;
	//deadline timer for receiving data from all cooperators
	boost::asio::deadline_timer t;
	boost::asio::io_service::work work;


};


#endif /* OFFLOADINGMANAGER_H_ */
