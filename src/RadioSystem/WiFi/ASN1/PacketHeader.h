/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "VSNTestBed"
 * 	found in "WiFiUDP_asn1defs.asn1"
 * 	`asn1c -fnative-types`
 */

#ifndef	_PacketHeader_H_
#define	_PacketHeader_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Address.h"
#include <NativeInteger.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PacketHeader */
typedef struct PacketHeader {
	Address_t	 sourceAddress;
	Address_t	 destinationAddress;
	Address_t	 destinationAddress2;
	long	 messageID;
	long	 messageType;
	long	 numberOfPackets;
	long	 packetSeqNumber;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PacketHeader_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PacketHeader;

#ifdef __cplusplus
}
#endif

#endif	/* _PacketHeader_H_ */
#include <asn_internal.h>
