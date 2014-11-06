/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "VSNTestBed"
 * 	found in "WiFiUDP_asn1defs.asn1"
 * 	`asn1c -fnative-types`
 */

#ifndef	_ReceptionReportMessage_H_
#define	_ReceptionReportMessage_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ReceptionReportModes.h"
#include <NativeInteger.h>
#include "asn_SEQUENCE_OF.h"
#include "constr_SEQUENCE_OF.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ReceptionReportMessage */
typedef struct ReceptionReportMessage {
	ReceptionReportModes_t	 mode;
	struct droppedPackets {
		A_SEQUENCE_OF(long) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} droppedPackets;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ReceptionReportMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReceptionReportMessage;

#ifdef __cplusplus
}
#endif

#endif	/* _ReceptionReportMessage_H_ */
#include <asn_internal.h>