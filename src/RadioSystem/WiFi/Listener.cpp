#include "Listener.h"
//#include <pthread>
	#include <iostream>

Listener::Listener(short listenPort, Receiver* receiver){
	listenPort_=listenPort;
	buflen_=1500;
	receiver_=receiver;
	so_=socket(PF_INET,SOCK_DGRAM,0);
}

Listener::~Listener() {
	free(buffer_);
	close(so_);
}

void Listener::listen(){
	int i;
	struct sockaddr_in addrsrv, addrcli;
	socklen_t addrcli_size;
//	uint8_t *buffer;
	buffer_=(uint8_t*)malloc(buflen_);

//	so=socket(PF_INET,SOCK_DGRAM,0);

	memset(&addrsrv,0,sizeof(addrsrv));
	memset(&addrcli,0,sizeof(addrcli));
	addrsrv.sin_family=AF_INET;
	addrsrv.sin_port=htons(listenPort_);
	addrsrv.sin_addr.s_addr=htonl(INADDR_ANY);

	if(bind(so_,(struct sockaddr *) &addrsrv, sizeof(addrsrv))!=0){
		fprintf(stderr, "Bind failed\n");
	}
	addrcli_size=sizeof(addrcli);

	Packet_t *pkt;
	debug(printf("Listening........\n"));
	while(1){
		i=recvfrom(so_, (void *)buffer_, buflen_, 0, (struct sockaddr *) &addrcli, &addrcli_size);
		pkt=decode_buffer(buffer_, i);
		//debug(std::cout << "Listener: new packet, messageType=" << pkt->header.messageType << "\n");
//		std::cerr << " Listener: new packet, messageType=" << pkt->header.messageType << "\n";
		if(pkt->header.messageType==MessageTypes_connectMsg){
			receiver_->notifyConnectMsg(addrcli, pkt->header.sourceAddress, pkt->header.destinationAddress);
		}
		else{
			receiver_->AddToQue(pkt);
		}
	}
//	close(so);
//	free(buffer_);
}

inline Packet_t* Listener::decode_buffer(uint8_t *buffer, int buflen){
	asn_dec_rval_t dc;
	Packet_t* decoded_packet=0;
	dc=uper_decode(0,&asn_DEF_Packet,(void**) &decoded_packet, buffer, buflen, 0,0);
//	std::cout << dc.consumed << "\n";
	return decoded_packet;
}

void Listener::ThreadEntry() {
//    pthread_cleanup_push(release_mutex, &que_mutex_);

    //while (!get_should_stop()) {
    while (true) {
      pthread_testcancel();
      listen();
    }
    //debug(printf("Exiting from ThreadEntry()!\n"));
//  pthread_cleanup_pop(0);
}

/*void Listener::release_mutex(void* mutex) {
  //debug(printf("Before release in release_mutex()\n"));
  pthread_mutex_unlock((pthread_mutex_t*) mutex);
  //debug(printf("After release in  release_mutex()\n"));
}*/
