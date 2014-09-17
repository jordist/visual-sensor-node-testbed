#include <stdlib.h>
#include <iostream>
#include "NodeManager/NodeManager.h"
#include "Tasks/Tasks.h"
#include "Messages/DataATCMsg.h"

using namespace std;

//todo: deadline timer for keeping the system active even if the start/stop message is not received


NodeManager::NodeManager(NodeType nt){
	node_type = nt;
	cout << "Node Type is " << node_type << endl;
	switch(node_type){
	case SINK:{
		break;
	}
	case CAMERA:{
		imgAcq = new ImageAcquisition(0);

		BRISK_detParams detPrms(60,4);

		BRISK_descParams dscPrms;
		extractor = new VisualFeatureExtraction();
		extractor->setDetector("BRISK", &detPrms);
		extractor->setDescriptor("BRISK",&dscPrms);

		encoder = new VisualFeatureEncoding();

		//boost::thread m_thread;
		//m_thread = boost::thread(&NodeManager::ATC_processing_thread, this, 0);

		break;
	}
	case COOPERATOR:{
		BRISK_detParams detPrms(60,4);

		BRISK_descParams dscPrms;
		extractor = new VisualFeatureExtraction();
		extractor->setDetector("BRISK", &detPrms);
		extractor->setDescriptor("BRISK",&dscPrms);

		encoder = new VisualFeatureEncoding();
		break;
	}
	default:
		break;
	}
	received_notifications = 0;
	outgoing_msg_seq_num = 255;
}

NodeType NodeManager::getNodeType(){
	return node_type;
}

void NodeManager::set_radioSystem(RadioSystem *rs){
	radioSystem_ptr = rs;
}
void NodeManager::set_taskManager(TaskManager *tm){
	taskManager_ptr = tm;
}

void NodeManager::set_s2gInterface(S2GInterface *s2g){
	s2gInterface_ptr = s2g;
}

void NodeManager::deleteMsg(Message *msg){
	delete(msg);
}

void NodeManager::notify_msg(Message *msg){
	cout << "NM: a message notification has been received:" << msg->getMessageType() << endl;

	boost::thread m_thread;
	switch(msg->getMessageType()){
	case START_CTA_MESSAGE:
	{
		switch(node_type){
		case SINK:
		{
			//just forward it (sendTestPacket will take care of deleting the msg)
			sendMessage(msg);
			break;
		}
		case CAMERA:
		{
			//update system state and start cta processing
			cta_param.quality_factor = ((StartCTAMsg*)msg)->getQualityFactor();
			cta_param.num_slices = ((StartCTAMsg*)msg)->getNumSlices();
			cur_state = ACTIVE;

			//todo: run only if there's not another CTA or ATC thread running!
			m_thread = boost::thread(&NodeManager::CTA_processing_thread, this);
			delete(msg);
			break;
		}
		default:
			break;
		}

		break;
	}

	case START_ATC_MESSAGE:
	{
		//if sink, forward to destination camera
		switch(node_type){
		case SINK:
		{
			//just forward it (sendTestPacket will take care of deleting the msg)
			sendMessage(msg);
			break;
		}
		case CAMERA:
		{
			// copy ATC params
			atc_param.max_features =  ((StartATCMsg*)msg)->getMaxNumFeat();
			atc_param.det = ((StartATCMsg*)msg)->getDetectorType();
			atc_param.detection_threshold = ((StartATCMsg*)msg)->getDetectorThreshold();
			atc_param.desc = ((StartATCMsg*)msg)->getDescriptorType();
			atc_param.desc_length = ((StartATCMsg*)msg)->getDescriptorLength();

			atc_param.coding = ((StartATCMsg*)msg)->getCoding();
			atc_param.transmit_keypoints = ((StartATCMsg*)msg)->getTransferKpt();
			atc_param.transmit_orientation = ((StartATCMsg*)msg)->getTransferOrientation();
			atc_param.transmit_scale = ((StartATCMsg*)msg)->getTransferScale();

			atc_param.num_blocks = ((StartATCMsg*)msg)->getNumBlocks();
			cur_state = ACTIVE;

			//if camera, start ATC processing with ATC params
			m_thread = boost::thread(&NodeManager::ATC_processing_thread, this);
			delete(msg);
			break;
		}
		default:
			break;
		}

		break;
	}
	case DATA_CTA_MESSAGE:
	{
		cout << "forwarding to gui" << endl;
		s2gInterface_ptr->writeMsg(msg);
		break;
	}
	case DATA_ATC_MESSAGE:
	{
		cout << "forwarding to gui" << endl;
		s2gInterface_ptr->writeMsg(msg);
		break;
	}

	case STOP_MESSAGE:
	{
		switch(node_type){
		case SINK:
		{
			//just forward it (sendTestPacket will take care of deleting the msg)
			sendMessage(msg);
			break;
		}
		case CAMERA:
		{
			cout << "stopping..." << endl;
			cur_state = IDLE;
			break;
		}
		}
	}
	default:
		break;
	}

}


int NodeManager::notify_endTask(){
	cout << "NM: a end-task notification has been received" << endl;
	cur_task_finished.notify_one();
	return 0;
}

void NodeManager::sendMessage(Message* msg){
	Task *cur_task;
	outgoing_msg_seq_num++;
	msg->setSeqNum(outgoing_msg_seq_num);
	cur_task = new SendMessageTask(msg,radioSystem_ptr->getTelosb());
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the send_message_task" << endl;
	{
		boost::mutex::scoped_lock lk(cur_task->task_monitor);
		while(!cur_task->completed){
			cur_task_finished.wait(lk);
		}
	}
	cout << "NM: exiting the tx thread" << endl;
	delete((SendMessageTask*)cur_task);
}


/*void NodeManager::sendTestPacket(Message* msg){
	boost::thread m_thread;
	m_thread = boost::thread(&NodeManager::transmission_thread, this, msg);
}

void NodeManager::transmission_thread(Message* msg){
	//create the SendMessageTask that sends the message
	cout << "NM: I'm entering the transmission thread" << endl;
	Task *cur_task;
	outgoing_msg_seq_num++;
	msg->setSeqNum(outgoing_msg_seq_num);
	cur_task = new SendMessageTask(msg,radioSystem_ptr->getTelosb());
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the send_message_task" << endl;
	{
		boost::mutex::scoped_lock lk(cur_task->task_monitor);
		while(!cur_task->completed){
			cur_task_finished.wait(lk);
		}
	}
	cout << "NM: exiting the tx thread" << endl;
	delete((SendMessageTask*)cur_task);
	//the SendMessageTask will packetize the message according to its address and
	//send each packet to the proper radio interface
	//packetization should be done by another specified component
	//while loop that sends packets check a thread-safe variable to understand if continuing
	//transmission or not. thread shouldn't be killed.


}*/

void NodeManager::CTA_processing_thread(){

	cout << "NM: I'm entering the CTA_processing thread" << endl;
	Task *cur_task;

	// Acquire the image
	cur_task = new AcquireImageTask(imgAcq);
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the acquire_image_task" << cur_task << endl;
	{
		boost::mutex::scoped_lock lk(cur_task->task_monitor);
		while(!cur_task->completed){
			//cout << "NM: entering in wait state. Status " << cur_task << " " << cur_task->completed << endl;
			cur_task_finished.wait(lk);
			received_notifications++;
		}
	}
	cout << "NM: ended acquire_image_task" << endl;
	Mat image = ((AcquireImageTask*)cur_task)->getImage();
	delete((AcquireImageTask*)cur_task);


	// Convert to gray-scale
	cout << "NM: Creating the convert_colorspace_task"  << endl;
	cur_task = new ConvertColorspaceTask(image,0);
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the convert_colorspace_task " << cur_task << endl;
	{
		boost::mutex::scoped_lock lk(cur_task->task_monitor);
		while(!cur_task->completed){
			//cout << "NM: entering in wait state. Status " << cur_task << " " << cur_task->completed << endl;
			cur_task_finished.wait(lk);
			received_notifications++;
		}
	}
	cout << "NM: ended convert_colorspace_task" << endl;
	image = ((ConvertColorspaceTask*)cur_task)->getConvertedImage();
	delete((ConvertColorspaceTask*)cur_task);

	for(int i=0;i<cta_param.num_slices;i++){
		cout << "NM: Creating the slice_image_task" << endl;
		cur_task = new SliceImageTask(image,Point(0,i*480/cta_param.num_slices),Point(640,(i+1)*480/cta_param.num_slices));
		taskManager_ptr->addTask(cur_task);
		cout << "NM: Waiting the end of the slice_image_task" << cur_task << endl;
		{
			boost::mutex::scoped_lock lk(cur_task->task_monitor);
			while(!cur_task->completed){
				//cout << "NM: entering in wait state. Status " << cur_task << " " << cur_task->completed << endl;
				cur_task_finished.wait(lk);
				received_notifications++;
			}
		}
		cout << "NM: ended slice_image_task" << endl;
		Mat slice = ((SliceImageTask*)cur_task)->getSlice();
		delete((SliceImageTask*)cur_task);


		//Encode with jpeg
		cout << "NM: encoding the slice with jpeg" << endl;
		cur_task = new EncodeSliceTask(slice,cta_param.quality_factor);
		taskManager_ptr->addTask(cur_task);
		cout << "NM: Waiting the end of the encode_image_task" << endl;
		{
			boost::mutex::scoped_lock lk(cur_task->task_monitor);
			while(!cur_task->completed){
				//cout << "NM: entering in wait state" << endl;
				cur_task_finished.wait(lk);
				received_notifications++;
			}
		}
		cout << "NM: ended encode_slice_task" << endl;
		vector<uchar> slice_bitstream;
		slice_bitstream = ((EncodeSliceTask*)cur_task)->getJpegBitstream();
		//imshow("rx_image",image1);
		//waitKey(50);

		delete((EncodeSliceTask*)cur_task);
		cout << "bitstream size of slice is " << slice_bitstream.size() << endl;


		Coordinate_t top_left;
		top_left.xCoordinate = 0;
		top_left.yCoordinate = (480/cta_param.num_slices)*i;
		DataCTAMsg *msg = new DataCTAMsg(0,i,top_left,slice_bitstream.size(),slice_bitstream);
		msg->setSource(1);
		msg->setDestination(0);
		//boost::thread tx_thread;
		//tx_thread = boost::thread(&NodeManager::transmission_thread, this, msg);
		//tx_thread.join();

		sendMessage(msg);

		//		//		cout << "NM: I'm entering the transmission thread" << endl;
		//		outgoing_msg_seq_num++;
		//		msg->setSeqNum(outgoing_msg_seq_num);
		//		cur_task = new SendMessageTask(msg,radioSystem_ptr->getTelosb());
		//		taskManager_ptr->addTask(cur_task);
		//		cout << "NM: Waiting the end of the send_message_task" << endl;
		//		{
		//			boost::mutex::scoped_lock lk(cur_task->task_monitor);
		//			while(!cur_task->completed){
		//				cur_task_finished.wait(lk);
		//			}
		//		}
		//		delete((SendMessageTask*)cur_task);
	}
	cout << "NM: exiting the CTA thread TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << endl;
	cout << "Rec notifications: " << received_notifications << ", Sent notifications: " << taskManager_ptr->sent_notifications << endl;

}

void NodeManager::ATC_processing_thread(){


	cout << "NM: I'm entering the ATC_processing thread " << endl;

	boost::mutex monitor;
	boost::mutex::scoped_lock lk(monitor);
	Task *cur_task;

	// Acquire the image
	cur_task = new AcquireImageTask(imgAcq);
	taskManager_ptr->addTask(cur_task);
	//cout << "NM: Waiting the end of the acquire_image_task" << endl;
	while(!cur_task->completed){
		cur_task_finished.wait(lk);
	}
	cout << "NM: ended acquire_image_task" << endl;
	Mat image = ((AcquireImageTask*)cur_task)->getImage();
	delete((AcquireImageTask*)cur_task);

	// Convert to gray-scale
	cur_task = new ConvertColorspaceTask(image,0);
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the convert_colorspace_task" << endl;
	while(!cur_task->completed){
		cur_task_finished.wait(lk);
	}
	cout << "NM: ended convert_colorspace_task" << endl;
	image = ((ConvertColorspaceTask*)cur_task)->getConvertedImage();
	delete((ConvertColorspaceTask*)cur_task);

	// Extract the keypoints
	cur_task = new ExtractKeypointsTask(extractor,image,atc_param.detection_threshold);
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the extract_keypoints_task" << endl;
	while(!cur_task->completed){
		cur_task_finished.wait(lk);
	}
	cout << "NM: ended extract_keypoints_task" << endl;

	vector<KeyPoint> kpts = ((ExtractKeypointsTask*)cur_task)->getKeypoints();
	cout << "extracted " << (int)kpts.size() << "keypoints" << endl;

	delete((ExtractKeypointsTask*)cur_task);

	//Extract features
	std::cout<<std::dec;
	cur_task = new ExtractFeaturesTask(extractor,image,kpts,atc_param.max_features);
	taskManager_ptr->addTask(cur_task);
	cout << "NM: Waiting the end of the extract_features_task" << endl;
	while(!cur_task->completed){
		cur_task_finished.wait(lk);
	}
	cout << "NM: ended extract_features_task" << endl;
	Mat features = ((ExtractFeaturesTask*)cur_task)->getFeatures();
	kpts = ((ExtractFeaturesTask*)cur_task)->getKeypoints();
	cout << "now extracted " << (int)kpts.size() << "keypoints" << endl;
	delete((ExtractFeaturesTask*)cur_task);

	if(kpts.size()>0){

		int feat_per_block = ceil((float)kpts.size() / (float)atc_param.num_blocks);
		int beg,end;

		cout << "features per block: " << (int)feat_per_block << endl;

		for(int i=0; i<atc_param.num_blocks; i++){

			vector<uchar> block_ft_bitstream;
			vector<uchar> block_kp_bitstream;

			beg = i*atc_param.num_blocks;
			end = min((int)(unsigned int)kpts.size(), (int)(beg+feat_per_block));

			Mat features_sub = features.rowRange(beg,end);

			if(atc_param.coding == CodingChoices_none){
				cur_task = new EncodeFeaturesTask(encoder,"BRISK",features_sub,0);
				taskManager_ptr->addTask(cur_task);
				//cout << "NM: Waiting the end of the encode_features_task" << endl;
				while(!cur_task->completed){
					cur_task_finished.wait(lk);
				}
				//cout << "NM: ended encode_features_task" << endl;
				block_ft_bitstream = ((EncodeFeaturesTask*)cur_task)->getFeatsBitstream();
				delete((EncodeFeaturesTask*)cur_task);
			}
			if(atc_param.coding == CodingChoices_entropyCoding){
				cur_task = new EncodeFeaturesTask(encoder,"BRISK",features_sub,1);
				taskManager_ptr->addTask(cur_task);
				//cout << "NM: Waiting the end of the encode_features_task" << endl;
				while(!cur_task->completed){
					cur_task_finished.wait(lk);
				}
				//cout << "NM: ended encode_features_task" << endl;
				block_ft_bitstream = ((EncodeFeaturesTask*)cur_task)->getFeatsBitstream();
				delete((EncodeFeaturesTask*)cur_task);
			}

			vector<KeyPoint> sub_kpts(&kpts[beg],&kpts[end]);
			cur_task = new EncodeKeypointsTask(encoder,sub_kpts,640,480,true);
			taskManager_ptr->addTask(cur_task);
			//cout << "NM: Waiting the end of the encode_kpts_task" << endl;
			while(!cur_task->completed){
				cur_task_finished.wait(lk);
			}
			//cout << "NM: ended encode_kpts_task" << endl;
			block_kp_bitstream = ((EncodeKeypointsTask*)cur_task)->getKptsBitstream();
			delete((EncodeKeypointsTask*)cur_task);
			cout << "sending " << (int)(sub_kpts.size()) << "keypoints" << endl;
			cout << "and " << (int)(features_sub.rows) << "features" << endl;


			DataATCMsg *msg = new DataATCMsg(0, i, block_ft_bitstream, block_kp_bitstream);
			msg->setSource(1);
			msg->setDestination(0);

			sendMessage(msg);

			cout << "first keypoint: " << sub_kpts[0].pt.x << " " << sub_kpts[0].pt.y << endl;

		}
	}

	//	if(atc_param.coding == CodingChoices_none){
	//		cout << "should do something here...." << endl;
	//		cur_task = new EncodeFeaturesTask(encoder,"BRISK",features,0);
	//		taskManager_ptr->addTask(cur_task);
	//		cout << "NM: Waiting the end of the encode_features_task" << endl;
	//		while(!cur_task->completed){
	//			cur_task_finished.wait(lk);
	//		}
	//		cout << "NM: ended encode_features_task" << endl;
	//		block_ft_bitstream = ((EncodeFeaturesTask*)cur_task)->getFeatsBitstream();
	//		delete((EncodeFeaturesTask*)cur_task);
	//	}
	//	if(atc_param.coding == CodingChoices_entropyCoding){
	//		cur_task = new EncodeFeaturesTask(encoder,"BRISK",features,1);
	//		taskManager_ptr->addTask(cur_task);
	//		cout << "NM: Waiting the end of the encode_features_task" << endl;
	//		while(!cur_task->completed){
	//			cur_task_finished.wait(lk);
	//		}
	//		cout << "NM: ended encode_features_task" << endl;
	//		block_ft_bitstream = ((EncodeFeaturesTask*)cur_task)->getFeatsBitstream();
	//		delete((EncodeFeaturesTask*)cur_task);
	//	}

	//	cur_task = new EncodeKeypointsTask(encoder,kpts,640,480,true);
	//	taskManager_ptr->addTask(cur_task);
	//	cout << "NM: Waiting the end of the encode_kpts_task" << endl;
	//	while(!cur_task->completed){
	//		cur_task_finished.wait(lk);
	//	}
	//	cout << "NM: ended encode_kpts_task" << endl;
	//	block_kp_bitstream = ((EncodeKeypointsTask*)cur_task)->getKptsBitstream();
	//	delete((EncodeKeypointsTask*)cur_task);
	//	cout << "sending " << (int)(kpts.size()) << "keypoints" << endl;
	//	cout << "and " << (int)(features.rows) << "features" << endl;


	//	DataATCMsg *msg = new DataATCMsg(0, 0, block_ft_bitstream, block_kp_bitstream);
	//	msg->setSource(1);
	//	msg->setDestination(0);
	//
	//	sendMessage(msg);

	//	//		cout << "NM: I'm entering the transmission thread" << endl;
	//	outgoing_msg_seq_num++;
	//	msg->setSeqNum(outgoing_msg_seq_num);
	//	cur_task = new SendMessageTask(msg,radioSystem_ptr->getTelosb());
	//	taskManager_ptr->addTask(cur_task);
	//	cout << "NM: Waiting the end of the send_message_task" << endl;
	//	{
	//		boost::mutex::scoped_lock lk(cur_task->task_monitor);
	//		while(!cur_task->completed){
	//			cur_task_finished.wait(lk);
	//		}
	//	}
	//	delete((SendMessageTask*)cur_task);

	cout << "NM: exiting the ATC thread QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ" << endl;


}
