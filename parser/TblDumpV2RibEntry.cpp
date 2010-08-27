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

#include "TblDumpV2RibEntry.h"

TblDumpV2RibEntry::TblDumpV2RibEntry(void) {
	attributes = new list<BGPAttribute>();
}

TblDumpV2RibEntry::TblDumpV2RibEntry(const TblDumpV2RibEntry& val) {
	peerIndex = val.peerIndex;
	originatedTime = val.originatedTime;
	attributeLength = val.attributeLength;
	
	attributes = new list<BGPAttribute>();
	
	list<BGPAttribute>::iterator iter;
	iter = val.attributes->begin();
	
	for (uint32_t i=0; i < val.attributes->size(); i++, iter++) {
		attributes->push_back(*iter);
	}
}

TblDumpV2RibEntry::~TblDumpV2RibEntry(void)
{
	delete attributes;
	attributes = NULL;
}

// TODO: Can we print from this abstract of a level?
//       Seems like we don't know AFI, SAFI...
void TblDumpV2RibEntry::printMe(MRTTblDumpV2PeerIndexTbl* peerIndexTbl) { 
	cout << "TblDumpV2RibEntry with Peer Index Table" << endl; 
	cout << "Peer Index: " << peerIndex;
	fflush(stdout);
	list<struct _MRTTblDumpV2PeerIndexTblPeerEntry>::iterator iter;
	
	int i=0;
	for (iter = peerIndexTbl->getPeerEntries()->begin(); i < peerIndex; iter++, i++)
	{ /* Do nothing -- looking up the correct index */ }
	cout << "iter should be at " << i;
}	

void TblDumpV2RibEntry::printMe() { 
	struct tm  *ts;
	ts = gmtime(&originatedTime);
	char buf[80];
	strftime(buf, sizeof(buf), "%m/%d/%Y %H:%M:%S", ts);
	cout << "  ORIGINATED: " << buf;
	// Print out the BGP Attributes
	list<BGPAttribute>::iterator iter;
	for (iter = attributes->begin(); iter != attributes->end(); iter++) {
		cout << endl << "  ";
		(*iter).printMe();		
	}
}	

void TblDumpV2RibEntry::printMeCompact() {
	list<BGPAttribute>::iterator iter;

	cout << "ATTR_CNT: " << attributes->size() << "|";
	for (iter = attributes->begin(); iter != attributes->end(); iter++) {
		iter->printMeCompact();
		cout << "|";
	}
}

void TblDumpV2RibEntry::printMeCompact(MRTTblDumpV2PeerIndexTbl *tbl) {
	/* nothing */
}


