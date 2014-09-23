/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "VSNTestBed"
 * 	found in "Messages.asn1"
 * 	`asn1c -S/skeletons -fnative-types`
 */

#ifndef	_OffloadingChoices_H_
#define	_OffloadingChoices_H_


#include <asn_application.h>

/* Including external dependencies */
#include <NativeEnumerated.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum OffloadingChoices {
	OffloadingChoices_polimi	= 0,
	OffloadingChoices_kth	= 1
} e_OffloadingChoices;

/* OffloadingChoices */
typedef long	 OffloadingChoices_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_OffloadingChoices;
asn_struct_free_f OffloadingChoices_free;
asn_struct_print_f OffloadingChoices_print;
asn_constr_check_f OffloadingChoices_constraint;
ber_type_decoder_f OffloadingChoices_decode_ber;
der_type_encoder_f OffloadingChoices_encode_der;
xer_type_decoder_f OffloadingChoices_decode_xer;
xer_type_encoder_f OffloadingChoices_encode_xer;
per_type_decoder_f OffloadingChoices_decode_uper;
per_type_encoder_f OffloadingChoices_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _OffloadingChoices_H_ */
