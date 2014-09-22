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

using namespace std;
using namespace cv;

class NodeManager;

class OffloadingManager{
public:
	OffloadingManager(NodeManager* nm){
		cooperators_to_use = 0;
		received_cooperators = 0;
		node_manager = nm;
	}

	//add keypoints from cooperators
	void addKeypoints(vector<KeyPoint>& kpts);

	//add features from cooperators
	void addFeatures(Mat& features);

	//reset variables and keep track of progresses
	void createOffloadingTask(int num_cooperators);

	//something that computes the slices based on the cooperators
	//vector<Mat>

private:

	int cooperators_to_use;
	int received_cooperators;

	NodeManager* node_manager;
	//used to store keypoints and features from cooperators
	vector<KeyPoint> keypoint_buffer;
	Mat features_buffer;


};


#endif /* OFFLOADINGMANAGER_H_ */
