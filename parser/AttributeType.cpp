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

// Author: Jason Ryder, Paul Wang
// Modified: Jonathan Park (jpark@cs.ucla.edu)
#include "AttributeType.h"

#include "AttributeTypeAggregator.h"
#include "AttributeTypeAS4Aggregator.h"
#include "AttributeTypeASPath.h"
#include "AttributeTypeAS4Path.h"
#include "AttributeTypeAtomicAggregate.h"
#include "AttributeTypeCommunities.h"
#include "AttributeTypeOriginatorID.h"
#include "AttributeTypeClusterList.h"
#include "AttributeTypeExtCommunities.h"
#include "AttributeTypeLocalPref.h"
#include "AttributeTypeMultiExitDisc.h"
#include "AttributeTypeNextHop.h"
#include "AttributeTypeOrigin.h"
#include "AttributeTypeMPReachNLRI.h"
#include "AttributeTypeMPUnreachNLRI.h"

uint8_t *AttributeType::endMsg = NULL;

AttributeType::AttributeType(void) {
	length = 0;
	value = NULL;
    error = 0;
}

AttributeType::AttributeType(uint16_t len, uint8_t* msg, bool isAS4) {
	PRINT_DBG("AttributeType::AttributeType()");
	PRINT_DBG2("  length = ", len);
	length = len;
	value = (uint8_t*)malloc(length);
	memcpy(value, msg, length);
    this->isAS4 = isAS4;
    error = 0;
}

AttributeType::AttributeType(uint16_t len, uint8_t* msg) {
	PRINT_DBG("AttributeType::AttributeType()");
	PRINT_DBG2("  length = ", len);
	length = len;
	value = (uint8_t*)malloc(length);
	memcpy(value, msg, length);
    isAS4 = false;
    error = 0;
}

AttributeType::AttributeType(const AttributeType& attr) {
	length = attr.length;
    isAS4  = attr.isAS4;
	if (attr.value)
	{
		value = (uint8_t*)malloc(length);
		PRINT_DBG("  Creating AttributeType Copy");
		memcpy(value, attr.value, length);
	} else {
		value = NULL;
    }
    error = 0;
}

AttributeType::~AttributeType(void) {
	if (value) {
		free(value);	
		value = NULL;
	}
}

AttributeType* AttributeType::newAttribute(uint8_t attrType, uint16_t len, uint8_t* msg, bool isAS4) {
    AttributeType* attr = NULL;

	switch(attrType) {
        PRINT_DBG("  Creating new Attribute(len, msg);");
		case ORIGIN:           { attr =  new AttributeTypeOrigin(len, msg);           break; }
		case AS_PATH:          { attr =  new AttributeTypeASPath(len, msg, isAS4);    break; }
		case NEXT_HOP:         { attr =  new AttributeTypeNextHop(len, msg);          break; }
		case MULTI_EXIT_DISC:  { attr =  new AttributeTypeMultiExitDisc(len, msg);    break; }
		case LOCAL_PREF:       { attr =  new AttributeTypeLocalPref(len, msg);        break; }
		case ATOMIC_AGGREGATE: { attr =  new AttributeTypeAtomicAggregate();          break; }
		case AGGREGATOR:       { attr =  new AttributeTypeAggregator(len, msg);       break; }
		case COMMUNITIES:      { attr =  new AttributeTypeCommunities(len, msg);      break; }
		case ORIGINATOR_ID:    { attr =  new AttributeTypeOriginatorID(len, msg);     break; }
		case CLUSTER_LIST:     { attr =  new AttributeTypeClusterList(len, msg);      break; }
		case EXT_COMMUNITIES:  { attr =  new AttributeTypeExtCommunities(len, msg);   break; }
		case MP_REACH_NLRI:    { attr =  new AttributeTypeMPReachNLRI(len, msg);      break; }
		case MP_UNREACH_NLRI:  { attr =  new AttributeTypeMPUnreachNLRI(len, msg);    break; }
		case NEW_AS_PATH:      { attr =  new AttributeTypeAS4Path(len, msg);          break; }
		case NEW_AGGREGATOR:   { attr =  new AttributeTypeAS4Aggregator(len, msg);    break; }
		// ADD MORE ATTRIBUTES HERE

		default: {
			PRINT_INFO("  Unhandled attribute type code");
			attr =  new AttributeType(len, msg, isAS4);
			break;
		}
	}
    return attr;
}

string AttributeType::getTypeStr(uint8_t attrType) {
	switch(attrType) {
		case ORIGIN:           { return "ORIGIN";           break; }
		case AS_PATH:          { return "AS_PATH";          break; }
		case NEXT_HOP:         { return "NEXT_HOP";         break; }
		case MULTI_EXIT_DISC:  { return "MULTI_EXIT_DISC";  break; }
		case LOCAL_PREF:       { return "LOCAL_PREF";       break; }
		case ATOMIC_AGGREGATE: { return "ATOMIC_AGGREGATE"; break; }
		case AGGREGATOR:       { return "AGGREGATOR";       break; }
		case COMMUNITIES:      { return "COMMUNITIES";      break; }
		case ORIGINATOR_ID:    { return "ORIGINATOR_ID";    break; }
		case CLUSTER_LIST:     { return "CLUSTER_LIST";     break; }
		case MP_REACH_NLRI:    { return "MP_REACH_NLRI";    break; }
		case MP_UNREACH_NLRI:  { return "MP_UNREACH_NLRI";  break; }
		case NEW_AS_PATH:      { return "AS4_PATH";         break; }
		case NEW_AGGREGATOR:   { return "AS4_AGGREGATOR";   break; }
		// ADD MORE ATTRIBUTES HERE

		default: { 
			return "OTHER";
		}
	}
	return "";
}

// vim: sw=4 ts=4 sts=4 expandtab
