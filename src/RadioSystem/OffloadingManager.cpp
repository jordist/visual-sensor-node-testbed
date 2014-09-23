/*
 * OffloadingManager.cpp
 *
 *  Created on: 19/set/2014
 *      Author: greeneyes
 */

#include "OffloadingManager.h"

void OffloadingManager::addCooperator(Connection* c){
	cooperator temp_coop;
	temp_coop.connection = c;
}



Mat OffloadingManager::computeLoads(int cooperatorsToUse, Mat image){
	Mat myLoad;
	double camera_load;

	// TO DO: DEFINE THESE VARIABLES
	//int F = 1514;
	//int H = 66;
	double v = cooperatorList[0].CPUspeed;
	double ovl = 15/100.0;

	double c_2,c_3,c_4;
	double s_0,s_1,s_2,s_3,s_4;

	cooperators_to_use = cooperatorsToUse;

	switch(cooperatorsToUse){
	case 1:
		camera_load = (0.5 - ovl/2);
		cooperatorList[0].load = (ovl/2 + 0.5);

		s_0 = camera_load;
		s_1 = camera_load + cooperatorList[0].load;

		cout<<"image size: " << image.rows << "x" << image.cols << endl;
		cout<<"a0: " << camera_load << ". cols from " << 0 << " to " << ceil((camera_load+ovl)*image.cols) << endl;
		cout<<"a1: " << cooperatorList[0].load << ". cols from " << ceil(camera_load*image.cols) << " to " << image.cols << endl;

		myLoad = Mat(image, Range(0, image.rows), Range(0, ceil((camera_load+ovl)*image.cols))).clone();
		cooperatorList[0].image_slice = Mat(image, Range(0, image.rows), Range(ceil(camera_load*image.cols),image.cols)).clone();
		cooperatorList[0].col_offset = ceil(camera_load*image.cols);
		break;

	case 2:
		c_2 = cooperatorList[1].bandwidth;
		camera_load = -(F*c_2*ovl - F*c_2 + F*ovl*v + H*ovl*v)/(3*F*c_2 + F*v + H*v);
		cooperatorList[0].load = (F*c_2 + F*v + H*v - F*c_2*ovl + F*ovl*v + H*ovl*v)/(3*F*c_2 + F*v + H*v);
		cooperatorList[1].load = (F*c_2 + 2*F*c_2*ovl)/(3*F*c_2 + F*v + H*v);

		s_0 = camera_load;
		s_1 = s_0 + cooperatorList[0].load;
		s_2 = s_1 + cooperatorList[1].load;

		cout<<"image size: " << image.rows << "x" << image.cols << endl;
		cout<<"a0: " << camera_load << ". cols from " << 0 << " to " << ceil((s_0+ovl)*image.cols) << endl;
		cout<<"a1: " << cooperatorList[0].load << ". cols from " << ceil(s_0*image.cols) << " to " << ceil((s_1+ovl)*image.cols) << endl;
		cout<<"a2: " << cooperatorList[1].load << ". cols from " << ceil(s_1*image.cols) << " to " << image.cols << endl;

		myLoad = Mat(image, Range(0, image.rows), Range(0, ceil((s_0+ovl)*image.cols))).clone();
		cooperatorList[0].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_0*image.cols),ceil((s_1+ovl)*image.cols))).clone();
		cooperatorList[0].col_offset = ceil(s_0*image.cols);
		cooperatorList[1].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_1*image.cols),image.cols)).clone();
		cooperatorList[1].col_offset = ceil(s_1*image.cols);


		break;

	case 3:
		c_2 = cooperatorList[1].bandwidth;
		c_3 = cooperatorList[2].bandwidth;
		camera_load = -(pow(F,2)*ovl*pow(v,2) + pow(H,2)*ovl*pow(v,2) - pow(F,2)*c_2*c_3 + 2*F*H*ovl*pow(v,2) + pow(F,2)*c_2*c_3*ovl + 2*pow(F,2)*c_2*ovl*v + pow(F,2)*c_3*ovl*v + 2*F*H*c_2*ovl*v + F*H*c_3*ovl*v)
				/(pow(F,2)*pow(v,2) + pow(H,2)*pow(v,2) + 2*F*H*pow(v,2) + 4*pow(F,2)*c_2*c_3 + 2*pow(F,2)*c_2*v + pow(F,2)*c_3*v + 2*F*H*c_2*v + F*H*c_3*v);
		cooperatorList[0].load = (pow(F,2)*pow(v,2) + pow(H,2)*pow(v,2) + 2*pow(F,2)*ovl*pow(v,2) + 2*pow(H,2)*ovl*pow(v,2) + 2*F*H*pow(v,2) + pow(F,2)*c_2*c_3 + pow(F,2)*c_2*v + pow(F,2)*c_3*v + 4*F*H*ovl*pow(v,2) - pow(F,2)*c_2*c_3*ovl + pow(F,2)*c_2*ovl*v + 2*pow(F,2)*c_3*ovl*v + F*H*c_2*v + F*H*c_3*v + F*H*c_2*ovl*v + 2*F*H*c_3*ovl*v)
				/(pow(F,2)*pow(v,2) + pow(H,2)*pow(v,2) + 2*F*H*pow(v,2) + 4*pow(F,2)*c_2*c_3 + 2*pow(F,2)*c_2*v + pow(F,2)*c_3*v + 2*F*H*c_2*v + F*H*c_3*v);
		cooperatorList[1].load = -(pow(F,2)*ovl*pow(v,2) + pow(H,2)*ovl*pow(v,2) - pow(F,2)*c_2*c_3 - pow(F,2)*c_2*v + 2*F*H*ovl*pow(v,2) + pow(F,2)*c_2*c_3*ovl - pow(F,2)*c_2*ovl*v + pow(F,2)*c_3*ovl*v - F*H*c_2*v - F*H*c_2*ovl*v + F*H*c_3*ovl*v)
				/(pow(F,2)*pow(v,2) + pow(H,2)*pow(v,2) + 2*F*H*pow(v,2) + 4*pow(F,2)*c_2*c_3 + 2*pow(F,2)*c_2*v + pow(F,2)*c_3*v + 2*F*H*c_2*v + F*H*c_3*v);
		cooperatorList[2].load = (pow(F,2)*c_2*c_3 + 3*pow(F,2)*c_2*c_3*ovl)/(pow(F,2)*pow(v,2) + pow(H,2)*pow(v,2) + 2*F*H*pow(v,2) + 4*pow(F,2)*c_2*c_3 + 2*pow(F,2)*c_2*v + pow(F,2)*c_3*v + 2*F*H*c_2*v + F*H*c_3*v);

		s_0 = camera_load;
		s_1 = s_0 + cooperatorList[0].load;
		s_2 = s_1 + cooperatorList[1].load;
		s_3 = s_2 + cooperatorList[2].load;

		cout<<"image size: " << image.rows << "x" << image.cols << endl;
		cout<<"a0: " << camera_load << ". cols from " << 0 << " to " << ceil((s_0+ovl)*image.cols) << endl;
		cout<<"a1: " << cooperatorList[0].load << ". cols from " << ceil(s_0*image.cols) << " to " << ceil((s_1+ovl)*image.cols) << endl;
		cout<<"a2: " << cooperatorList[1].load << ". cols from " << ceil(s_1*image.cols) << " to " << ceil((s_2+ovl)*image.cols) << endl;
		cout<<"a3: " << cooperatorList[2].load << ". cols from " << ceil(s_2*image.cols) << " to " << image.cols << endl;

		myLoad = Mat(image, Range(0, image.rows), Range(0, ceil((s_0+ovl)*image.cols))).clone();
		cooperatorList[0].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_0*image.cols),ceil((s_1+ovl)*image.cols))).clone();
		cooperatorList[0].col_offset = ceil(s_0*image.cols);
		cooperatorList[1].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_1*image.cols),ceil((s_2+ovl)*image.cols))).clone();
		cooperatorList[1].col_offset = ceil(s_1*image.cols);
		cooperatorList[2].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_2*image.cols),image.cols)).clone();
		cooperatorList[2].col_offset = ceil(s_2*image.cols);
		break;

	case 4:
		c_2 = cooperatorList[1].bandwidth;
		c_3 = cooperatorList[2].bandwidth;
		c_4 = cooperatorList[3].bandwidth;

		camera_load = -(pow(F,3)*ovl*pow(v,3) + pow(H,3)*ovl*pow(v,3) - pow(F,3)*c_2*c_3*c_4 + 3*F*pow(H,2)*ovl*pow(v,3) + 3*pow(F,2)*H*ovl*pow(v,3) + 2*pow(F,3)*c_2*ovl*pow(v,2) + pow(F,3)*c_3*ovl*pow(v,2) + pow(F,3)*c_4*ovl*pow(v,2) + pow(F,3)*c_2*c_3*c_4*ovl + 3*pow(F,3)*c_2*c_3*ovl*v + 2*pow(F,3)*c_2*c_4*ovl*v + pow(F,3)*c_3*c_4*ovl*v + 2*F*pow(H,2)*c_2*ovl*pow(v,2) + 4*pow(F,2)*H*c_2*ovl*pow(v,2) + F*pow(H,2)*c_3*ovl*pow(v,2) + 2*pow(F,2)*H*c_3*ovl*pow(v,2) + F*pow(H,2)*c_4*ovl*pow(v,2) + 2*pow(F,2)*H*c_4*ovl*pow(v,2) + 3*pow(F,2)*H*c_2*c_3*ovl*v + 2*pow(F,2)*H*c_2*c_4*ovl*v + pow(F,2)*H*c_3*c_4*ovl*v)/(pow(F,3)*pow(v,3) + pow(H,3)*pow(v,3) + 3*F*pow(H,2)*pow(v,3) + 3*pow(F,2)*H*pow(v,3) + 2*pow(F,3)*c_2*pow(v,2) + pow(F,3)*c_3*pow(v,2) + pow(F,3)*c_4*pow(v,2) + 5*pow(F,3)*c_2*c_3*c_4 + 3*pow(F,3)*c_2*c_3*v + 2*pow(F,3)*c_2*c_4*v + pow(F,3)*c_3*c_4*v + 2*F*pow(H,2)*c_2*pow(v,2) + 4*pow(F,2)*H*c_2*pow(v,2) + F*pow(H,2)*c_3*pow(v,2) + 2*pow(F,2)*H*c_3*pow(v,2) + F*pow(H,2)*c_4*pow(v,2) + 2*pow(F,2)*H*c_4*pow(v,2) + 3*pow(F,2)*H*c_2*c_3*v + 2*pow(F,2)*H*c_2*c_4*v + pow(F,2)*H*c_3*c_4*v);
		cooperatorList[0].load = (pow(F,3)*pow(v,3) + pow(H,3)*pow(v,3) + 3*F*pow(H,2)*pow(v,3) + 3*pow(F,2)*H*pow(v,3) + pow(F,3)*c_2*pow(v,2) + pow(F,3)*c_3*pow(v,2) + pow(F,3)*c_4*pow(v,2) + 3*pow(F,3)*ovl*pow(v,3) + 3*pow(H,3)*ovl*pow(v,3) + pow(F,3)*c_2*c_3*c_4 + pow(F,3)*c_2*c_3*v + pow(F,3)*c_2*c_4*v + pow(F,3)*c_3*c_4*v + F*pow(H,2)*c_2*pow(v,2) + 2*pow(F,2)*H*c_2*pow(v,2) + F*pow(H,2)*c_3*pow(v,2) + 2*pow(F,2)*H*c_3*pow(v,2) + F*pow(H,2)*c_4*pow(v,2) + 2*pow(F,2)*H*c_4*pow(v,2) + 9*F*pow(H,2)*ovl*pow(v,3) + 9*pow(F,2)*H*ovl*pow(v,3) + 2*pow(F,3)*c_2*ovl*pow(v,2) + 3*pow(F,3)*c_3*ovl*pow(v,2) + 3*pow(F,3)*c_4*ovl*pow(v,2) + pow(F,2)*H*c_2*c_3*v + pow(F,2)*H*c_2*c_4*v + pow(F,2)*H*c_3*c_4*v - pow(F,3)*c_2*c_3*c_4*ovl + pow(F,3)*c_2*c_3*ovl*v + 2*pow(F,3)*c_2*c_4*ovl*v + 3*pow(F,3)*c_3*c_4*ovl*v + 2*F*pow(H,2)*c_2*ovl*pow(v,2) + 4*pow(F,2)*H*c_2*ovl*pow(v,2) + 3*F*pow(H,2)*c_3*ovl*pow(v,2) + 6*pow(F,2)*H*c_3*ovl*pow(v,2) + 3*F*pow(H,2)*c_4*ovl*pow(v,2) + 6*pow(F,2)*H*c_4*ovl*pow(v,2) + pow(F,2)*H*c_2*c_3*ovl*v +2*pow(F,2)*H*c_2*c_4*ovl*v + 3*pow(F,2)*H*c_3*c_4*ovl*v)/(pow(F,3)*pow(v,3) + pow(H,3)*pow(v,3) + 3*F*pow(H,2)*pow(v,3) + 3*pow(F,2)*H*pow(v,3) + 2*pow(F,3)*c_2*pow(v,2) + pow(F,3)*c_3*pow(v,2) + pow(F,3)*c_4*pow(v,2) + 5*pow(F,3)*c_2*c_3*c_4 + 3*pow(F,3)*c_2*c_3*v + 2*pow(F,3)*c_2*c_4*v + pow(F,3)*c_3*c_4*v + 2*F*pow(H,2)*c_2*pow(v,2) + 4*pow(F,2)*H*c_2*pow(v,2) + F*pow(H,2)*c_3*pow(v,2) + 2*pow(F,2)*H*c_3*pow(v,2) + F*pow(H,2)*c_4*pow(v,2) + 2*pow(F,2)*H*c_4*pow(v,2) + 3*pow(F,2)*H*c_2*c_3*v + 2*pow(F,2)*H*c_2*c_4*v + pow(F,2)*H*c_3*c_4*v);
		cooperatorList[1].load = (pow(F,3)*c_2*pow(v,2) - pow(F,3)*ovl*pow(v,3) - pow(H,3)*ovl*pow(v,3) + pow(F,3)*c_2*c_3*c_4 + pow(F,3)*c_2*c_3*v + pow(F,3)*c_2*c_4*v + F*pow(H,2)*c_2*pow(v,2) + 2*pow(F,2)*H*c_2*pow(v,2) - 3*F*pow(H,2)*ovl*pow(v,3) - 3*pow(F,2)*H*ovl*pow(v,3) + 2*pow(F,3)*c_2*ovl*pow(v,2) - pow(F,3)*c_3*ovl*pow(v,2) - pow(F,3)*c_4*ovl*pow(v,2) + pow(F,2)*H*c_2*c_3*v + pow(F,2)*H*c_2*c_4*v - pow(F,3)*c_2*c_3*c_4*ovl + pow(F,3)*c_2*c_3*ovl*v + 2*pow(F,3)*c_2*c_4*ovl*v - pow(F,3)*c_3*c_4*ovl*v + 2*F*pow(H,2)*c_2*ovl*pow(v,2) + 4*pow(F,2)*H*c_2*ovl*pow(v,2) - F*pow(H,2)*c_3*ovl*pow(v,2) - 2*pow(F,2)*H*c_3*ovl*pow(v,2) - F*pow(H,2)*c_4*ovl*pow(v,2) - 2*pow(F,2)*H*c_4*ovl*pow(v,2) + pow(F,2)*H*c_2*c_3*ovl*v + 2*pow(F,2)*H*c_2*c_4*ovl*v - pow(F,2)*H*c_3*c_4*ovl*v)/(pow(F,3)*pow(v,3) + pow(H,3)*pow(v,3) + 3*F*pow(H,2)*pow(v,3) + 3*pow(F,2)*H*pow(v,3) + 2*pow(F,3)*c_2*pow(v,2) + pow(F,3)*c_3*pow(v,2) + pow(F,3)*c_4*pow(v,2) + 5*pow(F,3)*c_2*c_3*c_4 + 3*pow(F,3)*c_2*c_3*v + 2*pow(F,3)*c_2*c_4*v + pow(F,3)*c_3*c_4*v + 2*F*pow(H,2)*c_2*pow(v,2) + 4*pow(F,2)*H*c_2*pow(v,2) + F*pow(H,2)*c_3*pow(v,2) + 2*pow(F,2)*H*c_3*pow(v,2) + F*pow(H,2)*c_4*pow(v,2) + 2*pow(F,2)*H*c_4*pow(v,2) + 3*pow(F,2)*H*c_2*c_3*v + 2*pow(F,2)*H*c_2*c_4*v + pow(F,2)*H*c_3*c_4*v);
		cooperatorList[2].load = -(pow(F,3)*ovl*pow(v,3) + pow(H,3)*ovl*pow(v,3) - pow(F,3)*c_2*c_3*c_4 - pow(F,3)*c_2*c_3*v + 3*F*pow(H,2)*ovl*pow(v,3) + 3*pow(F,2)*H*ovl*pow(v,3) + 2*pow(F,3)*c_2*ovl*pow(v,2) + pow(F,3)*c_3*ovl*pow(v,2) + pow(F,3)*c_4*ovl*pow(v,2) - pow(F,2)*H*c_2*c_3*v + pow(F,3)*c_2*c_3*c_4*ovl - pow(F,3)*c_2*c_3*ovl*v + 2*pow(F,3)*c_2*c_4*ovl*v + pow(F,3)*c_3*c_4*ovl*v + 2*F*pow(H,2)*c_2*ovl*pow(v,2) + 4*pow(F,2)*H*c_2*ovl*pow(v,2) + F*pow(H,2)*c_3*ovl*pow(v,2) + 2*pow(F,2)*H*c_3*ovl*pow(v,2) + F*pow(H,2)*c_4*ovl*pow(v,2) + 2*pow(F,2)*H*c_4*ovl*pow(v,2) - pow(F,2)*H*c_2*c_3*ovl*v + 2*pow(F,2)*H*c_2*c_4*ovl*v + pow(F,2)*H*c_3*c_4*ovl*v)/(pow(F,3)*pow(v,3) + pow(H,3)*pow(v,3) + 3*F*pow(H,2)*pow(v,3) + 3*pow(F,2)*H*pow(v,3) + 2*pow(F,3)*c_2*pow(v,2) + pow(F,3)*c_3*pow(v,2) + pow(F,3)*c_4*pow(v,2) + 5*pow(F,3)*c_2*c_3*c_4 + 3*pow(F,3)*c_2*c_3*v + 2*pow(F,3)*c_2*c_4*v + pow(F,3)*c_3*c_4*v + 2*F*pow(H,2)*c_2*pow(v,2) + 4*pow(F,2)*H*c_2*pow(v,2) + F*pow(H,2)*c_3*pow(v,2) + 2*pow(F,2)*H*c_3*pow(v,2) + F*pow(H,2)*c_4*pow(v,2) + 2*pow(F,2)*H*c_4*pow(v,2) + 3*pow(F,2)*H*c_2*c_3*v + 2*pow(F,2)*H*c_2*c_4*v + pow(F,2)*H*c_3*c_4*v);
		cooperatorList[3].load = (pow(F,3)*c_2*c_3*c_4 + 4*pow(F,3)*c_2*c_3*c_4*ovl)/(pow(F,3)*pow(v,3) + pow(H,3)*pow(v,3) + 3*F*pow(H,2)*pow(v,3) + 3*pow(F,2)*H*pow(v,3) + 2*pow(F,3)*c_2*pow(v,2) + pow(F,3)*c_3*pow(v,2) + pow(F,3)*c_4*pow(v,2) + 5*pow(F,3)*c_2*c_3*c_4 + 3*pow(F,3)*c_2*c_3*v + 2*pow(F,3)*c_2*c_4*v + pow(F,3)*c_3*c_4*v + 2*F*pow(H,2)*c_2*pow(v,2) + 4*pow(F,2)*H*c_2*pow(v,2) + F*pow(H,2)*c_3*pow(v,2) + 2*pow(F,2)*H*c_3*pow(v,2) + F*pow(H,2)*c_4*pow(v,2) + 2*pow(F,2)*H*c_4*pow(v,2) + 3*pow(F,2)*H*c_2*c_3*v + 2*pow(F,2)*H*c_2*c_4*v + pow(F,2)*H*c_3*c_4*v);

		s_0 = camera_load;
		s_1 = s_0 + cooperatorList[0].load;
		s_2 = s_1 + cooperatorList[1].load;
		s_3 = s_2 + cooperatorList[2].load;
		s_4 = s_3 + cooperatorList[3].load;

		cout<<"image size: " << image.rows << "x" << image.cols << endl;
		cout<<"a0: " << camera_load << ". cols from " << 0 << " to " << ceil((s_0+ovl)*image.cols) << endl;
		cout<<"a1: " << cooperatorList[0].load << ". cols from " << ceil(s_0*image.cols) << " to " << ceil((s_1+ovl)*image.cols) << endl;
		cout<<"a2: " << cooperatorList[1].load << ". cols from " << ceil(s_1*image.cols) << " to " << ceil((s_2+ovl)*image.cols) << endl;
		cout<<"a3: " << cooperatorList[2].load << ". cols from " << ceil(s_2*image.cols) << " to " << ceil((s_3+ovl)*image.cols) << endl;
		cout<<"a4: " << cooperatorList[3].load << ". cols from " << ceil(s_3*image.cols) << " to " << image.cols << endl;


		myLoad = Mat(image, Range(0, image.rows), Range(0, ceil((s_0+ovl)*image.cols))).clone();
		cooperatorList[0].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_0*image.cols),ceil((s_1+ovl)*image.cols))).clone();
		cooperatorList[0].col_offset = ceil(s_0*image.cols);
		cooperatorList[1].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_1*image.cols),ceil((s_2+ovl)*image.cols))).clone();
		cooperatorList[1].col_offset = ceil(s_1*image.cols);
		cooperatorList[2].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_2*image.cols),ceil((s_3+ovl)*image.cols))).clone();
		cooperatorList[2].col_offset = ceil(s_2*image.cols);
		cooperatorList[3].image_slice = Mat(image, Range(0, image.rows), Range(ceil(s_3*image.cols),image.cols)).clone();
		cooperatorList[3].col_offset = ceil(s_3*image.cols);
		break;
	}

	return myLoad;
}


void OffloadingManager::transmitLoads(){

	vector<uchar> bitstream;
	vector<int> param = vector<int>(2);
	param[0] = CV_IMWRITE_JPEG_QUALITY;
	param[1] = 100;

	for(int i=0;i<cooperators_to_use;i++){
		cv::imencode(".jpg",cooperatorList[i].image_slice,bitstream,param);
		Coordinate_t top_left;
		top_left.xCoordinate = cooperatorList[i].col_offset;
		top_left.yCoordinate = 0;
		DataCTAMsg *msg = new DataCTAMsg(0,i,top_left,bitstream.size(),bitstream);

		cooperatorList[i].connection->writeMsg(msg);
	}
}
