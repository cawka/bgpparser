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

// Modified: Jonathan Park (jpark@cs.ucla.edu) 
#include <bgpparser.h>

#include "MRTBgp4MPSnapshot.h"
using namespace std;

MRTBgp4MPSnapshot::MRTBgp4MPSnapshot(void) {
	/* nothing */
}

MRTBgp4MPSnapshot::MRTBgp4MPSnapshot(uint8_t **ptr) 
: MRTCommonHeader((const uint8_t **)ptr) 
{
	uint8_t *p;

	/* add sizeof(MRTCommonHeaderPacket to ptr since ptr points to base of message */
	p = const_cast<uint8_t *>(*ptr) + sizeof(MRTCommonHeaderPacket);

	memcpy(&viewNumber, p, sizeof(uint16_t));
	p += sizeof(uint16_t);
	viewNumber = ntohs(viewNumber);

	/* clear out the file name */
	fileName.clear();

	/* scan input packet byte by byte and append to string */
	/* file name will be terminated by a NULL {0} character */
	while (*p != '\0') {
		fileName.append(1, *p++);
	}

	/* add an ending NULL {0} character to file name string for safety */
	fileName.append(1, '\0');

	/* TODO: increment the pointer to the new location in the file stream */
	*ptr = p;
}

MRTBgp4MPSnapshot::~MRTBgp4MPSnapshot(void) {
	/* nothing */
}

uint16_t MRTBgp4MPSnapshot::getViewNumber(void) const {
	return viewNumber;
}

string MRTBgp4MPSnapshot::getFileName(void) const {
	return fileName;
}
