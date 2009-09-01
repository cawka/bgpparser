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
#include "BGPAttribute.h"

BGPAttribute::BGPAttribute(void) {
	value = NULL;
}

BGPAttribute::BGPAttribute(uint8_t* msg, bool isAS4, uint8_t *maxptr) {
	attributeFlags = *msg;
	msg += sizeof(attributeFlags);
	attributeTypeCode = *(msg);
	msg += sizeof(attributeTypeCode);
	uint16_t len = 0;
	uint8_t len_8;
	// This methods handles endianess conversion in a generic way. If they were to
	//  change network byte order to little endian, this code would still be semantically
	//  correct.
	if (isExtendedLength()) {
		memcpy(&len, msg, sizeof(uint16_t));
		len = ntohs(len);
		msg += 2;
	}	else {
		memcpy(&len_8, msg, sizeof(uint8_t));
		len = len_8;
		msg += 1;
	}

	setAttributeLength(len);
	
	// TODO: Set attribute value
	AttributeType::setEndMsg(maxptr);
	value = AttributeType::newAttribute(attributeTypeCode, len, msg, isAS4);
}

// Copy constructor
BGPAttribute::BGPAttribute(const BGPAttribute& bgpA) {
	attributeFlags = bgpA.attributeFlags;
	attributeTypeCode = bgpA.attributeTypeCode;
	attributeLength = bgpA.attributeLength;

	/* define an attribute value class here */
	PRINT_DBG("  Attempting copy value");
	//AttributeType* val = bgpA.value; // Hopefully uses copy constructor
	//value = val; // Uses assignment, not copy constructor
	//value = new AttributeType(*bgpA.value);
	//value = bgpA.value;
	if (bgpA.value != NULL) {
		value = bgpA.value->clone();
	} else {
		value = NULL;
	}
}

BGPAttribute::~BGPAttribute(void) {
	if (value != NULL) {
		delete value;
		value = NULL;
	}
}


