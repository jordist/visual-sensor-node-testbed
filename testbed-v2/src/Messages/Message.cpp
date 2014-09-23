

#include "Message.h"
#include <math.h>

////START CTA
//void StartCTAMsg::setFramesPerSecond(int fps){
//	//internal_msg->choice.startctamessage.framesPerSecond = fps;
//	internal_msg->framesPerSecond = fps;
//}
//void StartCTAMsg::setQualityFactor(int qf){
//	//internal_msg->choice.startctamessage.qualityFactor = qf;
//	internal_msg->qualityFactor = qf;
//}
//void StartCTAMsg::setFrameWidth(int w){
//	//internal_msg->choice.startctamessage.frameWidth = w;
//	internal_msg->frameWidth = w;
//}
//void StartCTAMsg::setFrameHeight(int h){
//	//internal_msg->choice.startctamessage.frameHeight = h;
//	internal_msg->frameHeight = h;
//}
//int StartCTAMsg::getFramesPerSecond(){
//	//return internal_msg->choice.startctamessage.framesPerSecond;
//	return internal_msg->framesPerSecond;
//}
//int StartCTAMsg::getQualityFactor(){
//	//return internal_msg->choice.startctamessage.qualityFactor;
//	return internal_msg->qualityFactor;
//}
//int StartCTAMsg::getFrameHeight(){
//	//return internal_msg->choice.startctamessage.frameHeight;
//	return internal_msg->frameHeight;
//}
//int StartCTAMsg::getFrameWidth(){
//	//return internal_msg->choice.startctamessage.frameWidth;
//	return internal_msg->frameWidth;
//}
//int StartCTAMsg::getBitStream(vector<char>& bitstream){
//	char buf[1024];         /* Temporary buffer - 1K*/
//	asn_enc_rval_t ec;
//
//	ec = uper_encode_to_buffer(&asn_DEF_StartCTAMessage,internal_msg, buf, 1024);
//	for(int i=0;i<ceil(double(ec.encoded)/8);i++){
//		bitstream.push_back(buf[i]);
//	}
//
//
//	//
//	asn_dec_rval_t rval;
//	rval = uper_decode_complete(0, &asn_DEF_StartCTAMessage,(void **)&internal_msg, buf, 1024);
//	StartCTAMsg* msg1 = new StartCTAMsg(internal_msg);
//
//	if(rval.code != RC_OK) {
//		fprintf(stderr,
//				"Broken message encoding at byte %ld\n",
//				(long)rval.consumed);
//		exit(65); /* better, EX_DATAERR */
//	} else {
//		fprintf(stdout,"Printing Rectangle as XML...\n");
//		xer_fprint(stdout, &asn_DEF_StartCTAMessage, internal_msg);
//	}
//	cout << "fps: " << msg1->getFramesPerSecond() << endl;
//	cout << "qf: " << msg1->getQualityFactor() << endl;
//	cout << "w: " << msg1->getFrameWidth() << endl;
//	cout << "h: " << msg1->getFrameHeight() << endl;
//
//	return ec.encoded;
//}

