/*
 * Copyright (c) 2008,2009, University of California, Los Angeles All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of NLnetLabs nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MRTSTRUCTURE_H_
#define _MRTSTRUCTURE_H_

/* Address Families... consider moving to common code */
enum {
	AFI_IPv4 = 1,
	AFI_IPv6 = 2
};


/* MULTICAST/UNICAST */
enum {
	SAFI_UNICAST = 1,
	SAFI_MULTICAST = 2
};


/* MRT types */
enum {
	/* NULL = 0, */
	START = 1,
	DIE = 2,	/* deprecated */
	I_AM_DEAD = 3,
	PEER_DOWN = 4,	/* deprecated */
	BGP = 5,	/* deprecated */
	RIP = 6,	/* deprecated */
	IDRP = 7,	/* deprecated */
	RIPNG = 8,	/* deprecated */
	BGP4PLUS = 9,	/* deprecated */
	BGP4PLUS_01 = 10,	/* deprecated */
	OSPF = 11,
	TABLE_DUMP = 12,
	TABLE_DUMP_V2 = 13,
	BGP4MP = 16,
	BGP4MP_ET = 17,
	ISIS = 32,
	ISIS_ET = 33,
	OSPFv3 = 48,
	OSPFv3_ET = 49,
	OSPF_ET = 64	/* deprecated */
};


/* BGP4MP Subtypes */
enum {
	BGP4MP_STATE_CHANGE = 0,
	BGP4MP_MESSAGE = 1,
	BGP4MP_ENTRY = 2,			/* not implemented */
	BGP4MP_SNAPSHOT = 3,		/* not implemented */
	BGP4MP_MESSAGE_AS4 = 4,
	BGP4MP_STATE_CHANGE_AS4 = 5,
};


/* TABLE_DUMP_V2 Subtypes */
enum {
	PEER_INDEX_TABLE = 1,
	RIB_IPV4_UNICAST = 2,
	RIB_IPV4_MULTICAST = 3,
	RIB_IPV6_UNICAST = 4,
	RIB_IPV6_MULTICAST = 5,
	RIB_GENERIC = 6
};


/* BGP4MP state change message states */
enum {
	IDLE = 1,
	CONNECT = 2,
	ACTIVE = 3,
	OPENSENT = 4,
	OPENCONFIRM = 5,
	ESTABLISHED = 6
};


/* BGP attribute types */
enum {
	ATTR_TYPE_ORIGIN = 1,
	ATTR_TYPE_ASPATH = 2,
	ATTR_TYPE_NEXTHOP = 3, 
	ATTR_TYPE_MULTIEXITDISC = 4,
	ATTR_TYPE_LOCALPREF= 5,
	ATTR_TYPE_ATOMICAGGREGATE = 6,
	ATTR_TYPE_AGGREGATOR = 7,
	ATTR_TYPE_COMMUNITIES = 8,
	ATTR_TYPE_ORIGINATOR_ID = 9,	/* not supported */
	ATTR_TYPE_CLUSTER_LIST = 10,	/* not supported */
	ATTR_TYPE_DPA = 11,				/* not supported */
	ATTR_TYPE_ADVERTISER = 12,		/* not supported */
	ATTR_TYPE_RCID_PATH = 13,		/* not supported */
	ATTR_TYPE_MP_REACH_NLRI = 14,
	ATTR_TYPE_MP_UNREACH_NLRI = 15,
	ATTR_TYPE_EXT_COMMUNITIES = 16,	/* not supported */
	ATTR_TYPE_NEW_AS_PATH = 17,
	ATTR_TYPE_NEW_AGGREGATOR = 18
};


/* BGP ORIGIN attribute values */
enum {
	ATTR_TYPE_ORIGIN_IGP = 0,
	ATTR_TYPE_ORIGIN_EGP = 1,
	ATTR_TYPE_ORIGIN_INCOMPLETE = 2
};


/* BGP AS_PATH attribute segment types */
enum {
	ATTR_TYPE_AS_PATH_SET = 1,
	ATTR_TYPE_AS_PATH_SEQUENCE = 2
	// TODO: Add two additional types -- confederation ?
};


/* BGP COMMUNITIES attribute well known communities */
enum {
	ATTR_TYPE_COMMUNITIES_NOEXPORT = 0xFFFFFF01,
	ATTR_TYPE_COMMUNITIES_NOADVERTISE = 0xFFFFFF02,
	ATTR_TYPE_COMMUNITIES_SUBCONFED = 0xFFFFFF03
};


/* BGP Notification error codes */
enum {
	BGP_NOTIFICATION_MSG_HEADER_ERROR = 1,
	BGP_NOTIFICATION_OPEN_MSG_ERROR = 2,
	BGP_NOTIFICATION_UPDATE_MSG_ERROR = 3,
	BGP_NOTIFICATION_HOLD_TIMER_EXPIRED = 4,
	BGP_NOTIFICATION_FINITE_STATE_ERROR = 5,
	BGP_NOTIFICATION_CEASE = 6
};


/* BGP Notification unspecific sub-error code */
#define UNSPECIFIC	0


/* BGP Notification message header error sub-error codes */
enum {
	CONNECTION_NOT_SYNCHRONIZED = 1,
	BAD_MESSAGE_LENGTH = 2,
	BAD_MESSAGE_TYPE = 3
};


/* BGP Notification open message error sub-error codes */
enum {
	UNSUPPORTED_VERSION_NUMBER = 1,
	BAD_PEER_AS = 2,
	BAD_BGP_IDENTIFIER = 3,
	UNSUPPORTED_OPTIONAL_PARAMETER = 4,
	AUTHENTICATION_FAILURE = 5,			/* deprecated */
	UNACCEPTABLE_HOLD_TIME = 6
};


/* BGP Notification update message error sub-error codes */
enum {
	MALFORMED_ATTRIBUTE_LIST = 1,
	UNRECOGNIZED_WELL_KNOWN_ATTRIBUTE = 2,
	MISSING_WELL_KNOWN_ATTRIBUTE = 3,
	ATTRIBUTE_FLAGS_ERROR = 4,
	ATTRIBUTE_LENGTH_ERROR = 5,
	INVALID_ORIGIN_ATTRIBUTE = 6,
	AS_ROUTING_LOOP = 7,				/* deprecated */
	INVALID_NEXT_HOP_ATTRIBUTE = 8,
	OPTIONAL_ATTRIBUTE_ERROR = 9,
	INVALID_NETWORK_FIELD = 10,
	MALFORMED_AS_PATH = 11
};

/* IP addresses... consider moving to common code */
typedef union _IPAddress {
	in_addr ipv4;
	in6_addr ipv6;
} IPAddress;


/* Network Layer Reachability Information... moved to BGPStructure.h */
/*
typedef struct _NLRI {
	uint8_t prefixLength;
	IPAddress prefix;
} NLRI;
*/


/* Common header of MRT packet structure */
typedef struct _MRTCommonHeaderPacket {
	uint32_t timestamp;
	uint16_t type;
	uint16_t subtype;
	uint32_t length;
} MRTCommonHeaderPacket;


/* Peer entry for a TABLE_DUMP_V2 PEER_INDEXT_TABLE subtype */
typedef struct _MRTTblDumpV2PeerIndexTblPeerEntry {
	uint16_t IPType;
	bool isAS4;
	uint32_t peerBGPId;
	IPAddress peerIP;
	uint32_t peerAS;
} MRTTblDumpV2PeerIndexTblPeerEntry;


/* RIB entry for a TABLE_DUMP_V2 RIB_IPV4_UNICAST subtype */
/*
typedef struct _MRTTblDumpV2RibEntry {
	uint16_t peerIndex;
	time_t originatedTime;
	uint16_t attributeLength;

	//struct _BGPAttribute1 *attributes;
} MRTTblDumpV2RibEntry;
*/


/* BGP attribute structure */
/*
typedef struct _BGPAttribute {
	// TODO: add BGP attribute properties here
	union {
		struct {
			uint8_t attributeFlags;
			uint8_t attributeTypeCode;
		} fields;
		struct {
			uint16_t attr;
		} attribute;
	} attributeType;
	uint16_t attributeLength;
	uint8_t *attributeValue;
} BGPAttribute1;
*/


/* Addon header data for MRT when type is BGP4MP and subtype is BGP4MP_MESSAGE */
typedef struct _MRTBgp4MPMessagePacket {
	uint16_t peerAS;
	uint16_t localAS;
	uint16_t interfaceIndex;
	uint16_t addressFamily;
} MRTBgp4MPMessagePacket;

/* Addon header data for MRT when type is BGP4MP and subtype is BGP4MP_MESSAGE_32BIT_AS */
typedef struct _MRTBgp4MPMessageAS4Packet {
	uint32_t peerAS;
	uint32_t localAS;
	uint16_t interfaceIndex;
	uint16_t addressFamily;
} MRTBgp4MPMessageAS4Packet;


/* Addon header data for MRT when type is BGP4MP and subtype is BGP4MP_STATE_CHANGE */
typedef struct _MRTBgp4MPStateChangePacket {
	uint16_t peerAS;
	uint16_t localAS;
	uint16_t interfaceIndex;
	uint16_t addressFamily;
} MRTBgp4MPStateChangePacket;


/* Addon header data for MRT when type is BGP4MP and subtype is BGP4MP_STATE_CHANGE_AS4 */
typedef struct _MRTBgp4MPStateChangeAS4Packet {
	uint32_t peerAS;
	uint32_t localAS;
	uint16_t interfaceIndex;
	uint16_t addressFamily;
} MRTBgp4MPStateChangeAS4Packet;


/* Addon header data for MRT when type is BGP4MP and subtype is BGP4MP_ENTRY */
typedef struct _MRTBgp4MPEntryPacket {
	uint16_t viewNumber;
	uint16_t status; 
	uint32_t timeLastChange;
	uint16_t addressFamily;
	uint8_t safi;
	uint8_t nextHopLength;
} MRTBgp4MPEntryPacket;


#endif	/* _MRTSTRUCTURE_H_ */
