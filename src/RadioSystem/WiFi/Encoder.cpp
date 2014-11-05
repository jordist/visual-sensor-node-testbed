/*
 * Encoder.cpp
 *
 *  Created on: Oct 28, 2014
 *      Author: jordi
 */

#include "Encoder.h"

int Encoder::Encode(ReceptionReportMessage_t* s, uint8_t* buffer,
		size_t bufsize) {
	asn_enc_rval_t enc_rval;
	enc_rval=uper_encode_to_buffer(&asn_DEF_ReceptionReportMessage, s, buffer, bufsize);
	return ((enc_rval.encoded + 7) / 8); //Number of bytes encoded
}

int Encoder::Encode(ReceptionReportRequestMessage_t* s, uint8_t* buffer,
		size_t bufsize) {
	asn_enc_rval_t enc_rval;
	enc_rval=uper_encode_to_buffer(&asn_DEF_ReceptionReportRequestMessage, s, buffer, bufsize);
	return ((enc_rval.encoded + 7) / 8); //Number of bytes encoded
}

int Encoder::Encode(Packet_t* s, uint8_t* buffer, size_t bufsize) {
	asn_enc_rval_t enc_rval;
	enc_rval=uper_encode_to_buffer(&asn_DEF_Packet, s, buffer, bufsize);
	return ((enc_rval.encoded + 7) / 8); //Number of bytes encoded
}
