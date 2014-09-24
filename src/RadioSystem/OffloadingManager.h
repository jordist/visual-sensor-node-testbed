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

	double t0; //start tx of image load
	double t1; //end rx of features
	int tx_bytes,rx_bytes;
}cooperator;

class OffloadingManager{
public:
	OffloadingManager(NodeManager* nm){
		cooperators_to_use = 0;
		received_cooperators = 0;
		node_manager = nm;
	}

	//add keypoints and features from cooperators
	void addKeypointsAndFeatures(vector<KeyPoint>& kpts,Mat& features,Connection* cn);

	//reset variables and keep track of progresses
	void createOffloadingTask(int num_cooperators);

	void addCooperator(Connection* c);
	void removeCooperator(Connection* c);
	Mat computeLoads(Mat& image);
	void transmitLoads();
	void transmitStartDATC(StartDATCMsg* msg);
	int probeLinks();
	void sortCooperators();


private:

	int cooperators_to_use;
	int received_cooperators;
	vector<cooperator> cooperatorList;

	NodeManager* node_manager;
	//used to store keypoints and features from cooperators
	vector<KeyPoint> keypoint_buffer;
	Mat features_buffer;


};


#endif /* OFFLOADINGMANAGER_H_ */
