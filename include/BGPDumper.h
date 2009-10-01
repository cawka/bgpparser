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

// BGP Message Dumper
#ifndef __BGPDUMPER_H_
#define __BGPDUMPER_H_

#include "Dumper.h"
#include "BGPCommonHeader.h"
#include "BGPAttribute.h"
#include <list>

using namespace std;

/* Common Dumper */
class BGPDumper : public Dumper
{
public:
	BGPDumper(BGPMessage*);
	virtual ~BGPDumper();
	
	// Factory method for creating a BGP dumper
	static class BGPDumper* newDumper(BGPMessage*);

    // XML output
	virtual xmlNodePtr genXml();
	xmlNodePtr genXmlAsciiMsg();
	xmlNodePtr genXmlOctetMsg();
	virtual xmlNodePtr genXmlAsciiNode();

    // Ascii output
	virtual string genAscii();
    //virtual list<string> genAsciiMsg(); 
    virtual list<string> genAsciiMsg(string peer_addr, string peer_as); 

protected:
    BGPMessage* bgp_msg;
};

/* Open */
class BGPOpenDumper : public BGPDumper 
{
public:
	BGPOpenDumper(BGPMessage*);
	virtual ~BGPOpenDumper();
protected:
};

/* Update */
class BGPUpdateDumper : public BGPDumper 
{
public:
	BGPUpdateDumper(BGPMessage*);
	virtual ~BGPUpdateDumper();

    // XML output
	xmlNodePtr genUpdateWithdrawnNode(list<Route>* Routes);
	xmlNodePtr genUpdateNlriNode(list<Route>* Routes);
	xmlNodePtr genPrefixNode(Route, uint32_t afi, uint32_t safi);
	xmlNodePtr genUpdateAttributesNode(list<BGPAttribute>* pathAttributes);
	virtual xmlNodePtr genXmlAsciiNode();

    // Ascii output
    //virtual list<string> genAsciiMsg(); 
    virtual list<string> genAsciiMsg(string peer_addr, string peer_as); 
protected:
};

/* Keepalive */
class BGPKeepAliveDumper : public BGPDumper 
{
public:
	BGPKeepAliveDumper(BGPMessage*);
	virtual ~BGPKeepAliveDumper();
protected:
};

/* Notification */
class BGPNotificationDumper : public BGPDumper 
{
public:
	BGPNotificationDumper(BGPMessage*);
	virtual ~BGPNotificationDumper();
protected:
};

/* Route Refresh */
class BGPRouteRefreshDumper : public BGPDumper 
{
public:
	BGPRouteRefreshDumper(BGPMessage*);
	virtual ~BGPRouteRefreshDumper();
protected:
};

#endif /* __BGPDUMPER_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
