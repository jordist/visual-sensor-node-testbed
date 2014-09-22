/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "VSNTestBed"
 * 	found in "Messages.asn1"
 * 	`asn1c -S/skeletons -fnative-types`
 */

#ifndef	_CoopStatus_H_
#define	_CoopStatus_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CoopStatus {
	CoopStatus_online	= 0,
	CoopStatus_offline	= 1
} e_CoopStatus;

/* CoopStatus */
typedef long	 CoopStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CoopStatus;
asn_struct_free_f CoopStatus_free;
asn_struct_print_f CoopStatus_print;
asn_constr_check_f CoopStatus_constraint;
ber_type_decoder_f CoopStatus_decode_ber;
der_type_encoder_f CoopStatus_encode_der;
xer_type_decoder_f CoopStatus_decode_xer;
xer_type_encoder_f CoopStatus_encode_xer;
per_type_decoder_f CoopStatus_decode_uper;
per_type_encoder_f CoopStatus_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _CoopStatus_H_ */