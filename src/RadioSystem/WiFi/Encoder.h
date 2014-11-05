/*
 * Encoder.h
 *
 *  Created on: Oct 28, 2014
 *      Author: jordi
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "ASN1/ReceptionReportMessage.h"
#include "ASN1/ReceptionReportRequestMessage.h"
#include "ASN1/Packet.h"

class Encoder {
public:
	int Encode(ReceptionReportMessage_t *s, uint8_t *buffer, size_t bufsize);
	int Encode(ReceptionReportRequestMessage_t *s, uint8_t *buffer, size_t bufsize);
	int Encode(Packet_t *s, uint8_t *buffer, size_t bufsize);
};

#endif /* ENCODER_H_ */
