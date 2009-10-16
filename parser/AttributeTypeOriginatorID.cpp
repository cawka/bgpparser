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

// Author: Jonathan Park
#include "AttributeTypeOriginatorID.h"

AttributeTypeOriginatorID::AttributeTypeOriginatorID(void) {
	/* nothing */
}

AttributeTypeOriginatorID::AttributeTypeOriginatorID(uint16_t len, uint8_t* msg)
					: AttributeType(len, msg) {
	PRINT_DBG("AttributeTypeOriginatorID::AttributeTypeOriginatorID()");
	memcpy(&originator_id,msg,sizeof(uint32_t));
}

AttributeTypeOriginatorID::AttributeTypeOriginatorID(uint16_t len, uint32_t oid, uint8_t* msg)
					: AttributeType(len, msg) {
	originator_id = oid;
}

AttributeTypeOriginatorID::AttributeTypeOriginatorID(const AttributeTypeOriginatorID& attr) {
	length = attr.length;
	originator_id = attr.originator_id;
	if (attr.value) {
		value = (uint8_t*)malloc(length);
		memcpy(value, attr.value, length);
	} else {
		value = NULL;
	}
}


AttributeTypeOriginatorID::~AttributeTypeOriginatorID(void) {
	/* nothing */
}

void AttributeTypeOriginatorID::printMe() { 
	IPAddress addr;
	memcpy(&addr,&originator_id,sizeof(uint32_t));
	cout << "ORIGINATOR-ID: ";
	PRINT_IP_ADDR(addr.ipv4);
}

void AttributeTypeOriginatorID::printMeCompact() { 
	IPAddress addr;
	memcpy(&addr,&originator_id,sizeof(uint32_t));
	cout << "ORIGINATOR-ID: ";
	PRINT_IP_ADDR(addr.ipv4);
}
