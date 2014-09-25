/*
 * Generated by asn1c-0.9.22 (http://lionet.info/asn1c)
 * From ASN.1 module "VSNTestBed"
 * 	found in "Messages.asn1"
 * 	`asn1c -S/skeletons -fnative-types`
 */

#include <asn_internal.h>

#include "StartDATCMessage.h"

static asn_TYPE_member_t asn_MBR_StartDATCMessage_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, framesPerSecond),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"framesPerSecond"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, detectorType),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DetectorTypes,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"detectorType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, detectorThreshold),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeReal,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"detectorThreshold"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, descriptorType),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_DescriptorTypes,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"descriptorType"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, descriptorLength),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"descriptorLength"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, maxNumberOfFeatures),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"maxNumberOfFeatures"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, rotationInvariant),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"rotationInvariant"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, coding),
		(ASN_TAG_CLASS_CONTEXT | (7 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_CodingChoices,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"coding"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, transferCoordinates),
		(ASN_TAG_CLASS_CONTEXT | (8 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"transferCoordinates"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, transferScale),
		(ASN_TAG_CLASS_CONTEXT | (9 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"transferScale"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, transferOrientation),
		(ASN_TAG_CLASS_CONTEXT | (10 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_BOOLEAN,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"transferOrientation"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, numFeaturesPerBlock),
		(ASN_TAG_CLASS_CONTEXT | (11 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"numFeaturesPerBlock"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, numCooperators),
		(ASN_TAG_CLASS_CONTEXT | (12 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_NativeInteger,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"numCooperators"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct StartDATCMessage, offloading),
		(ASN_TAG_CLASS_CONTEXT | (13 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_OffloadingChoices,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"offloading"
		},
};
static ber_tlv_tag_t asn_DEF_StartDATCMessage_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static asn_TYPE_tag2member_t asn_MAP_StartDATCMessage_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* framesPerSecond at 68 */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* detectorType at 71 */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* detectorThreshold at 72 */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* descriptorType at 73 */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* descriptorLength at 74 */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* maxNumberOfFeatures at 75 */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 }, /* rotationInvariant at 77 */
    { (ASN_TAG_CLASS_CONTEXT | (7 << 2)), 7, 0, 0 }, /* coding at 79 */
    { (ASN_TAG_CLASS_CONTEXT | (8 << 2)), 8, 0, 0 }, /* transferCoordinates at 81 */
    { (ASN_TAG_CLASS_CONTEXT | (9 << 2)), 9, 0, 0 }, /* transferScale at 82 */
    { (ASN_TAG_CLASS_CONTEXT | (10 << 2)), 10, 0, 0 }, /* transferOrientation at 83 */
    { (ASN_TAG_CLASS_CONTEXT | (11 << 2)), 11, 0, 0 }, /* numFeaturesPerBlock at 84 */
    { (ASN_TAG_CLASS_CONTEXT | (12 << 2)), 12, 0, 0 }, /* numCooperators at 85 */
    { (ASN_TAG_CLASS_CONTEXT | (13 << 2)), 13, 0, 0 } /* offloading at 87 */
};
static asn_SEQUENCE_specifics_t asn_SPC_StartDATCMessage_specs_1 = {
	sizeof(struct StartDATCMessage),
	offsetof(struct StartDATCMessage, _asn_ctx),
	asn_MAP_StartDATCMessage_tag2el_1,
	14,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_StartDATCMessage = {
	"StartDATCMessage",
	"StartDATCMessage",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_StartDATCMessage_tags_1,
	sizeof(asn_DEF_StartDATCMessage_tags_1)
		/sizeof(asn_DEF_StartDATCMessage_tags_1[0]), /* 1 */
	asn_DEF_StartDATCMessage_tags_1,	/* Same as above */
	sizeof(asn_DEF_StartDATCMessage_tags_1)
		/sizeof(asn_DEF_StartDATCMessage_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_StartDATCMessage_1,
	14,	/* Elements count */
	&asn_SPC_StartDATCMessage_specs_1	/* Additional specs */
};

