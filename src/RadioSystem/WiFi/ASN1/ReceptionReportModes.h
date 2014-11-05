/*
 * Generated by asn1c-0.9.24 (http://lionet.info/asn1c)
 * From ASN.1 module "VSNTestBed"
 * 	found in "WiFiUDP_asn1defs.asn1"
 * 	`asn1c -fnative-types`
 */

#ifndef	_ReceptionReportModes_H_
#define	_ReceptionReportModes_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ReceptionReportModes {
	ReceptionReportModes_specifyEach	= 0,
	ReceptionReportModes_specifyIntervals	= 1
} e_ReceptionReportModes;

/* ReceptionReportModes */
typedef long	 ReceptionReportModes_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ReceptionReportModes;
asn_struct_free_f ReceptionReportModes_free;
asn_struct_print_f ReceptionReportModes_print;
asn_constr_check_f ReceptionReportModes_constraint;
ber_type_decoder_f ReceptionReportModes_decode_ber;
der_type_encoder_f ReceptionReportModes_encode_der;
xer_type_decoder_f ReceptionReportModes_decode_xer;
xer_type_encoder_f ReceptionReportModes_encode_xer;
per_type_decoder_f ReceptionReportModes_decode_uper;
per_type_encoder_f ReceptionReportModes_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ReceptionReportModes_H_ */
#include <asn_internal.h>
