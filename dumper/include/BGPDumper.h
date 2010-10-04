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

class BGPDumper;
typedef boost::shared_ptr<BGPDumper> BGPDumperPtr;

/* Common Dumper */
class BGPDumper : public Dumper
{
public:
	virtual ~BGPDumper();
	
	// Factory method for creating a BGP dumper
	static BGPDumperPtr newDumper( const BGPMessagePtr &msg );

    // XML output
	virtual xmlNodePtr genXml();
	xmlNodePtr genXmlAsciiMsg();
	xmlNodePtr genXmlOctetMsg();
	virtual xmlNodePtr genXmlAsciiNode();

    // Ascii output
	virtual std::string genAscii();
    //virtual list<string> genAsciiMsg(); 
    virtual std::list<std::string> genAsciiMsg(string peer_addr, string peer_as, bool is_tabledump);

protected:
	BGPDumper( const BGPMessagePtr &msg );

private:
    BGPDumper( ) { }

protected:
    BGPMessagePtr bgp_msg;

private:
    static log4cxx::LoggerPtr Logger;
};

/* Open */
class BGPOpenDumper : public BGPDumper 
{
public:
	BGPOpenDumper(const BGPMessagePtr &msg);
	virtual ~BGPOpenDumper();
protected:
};

/* Update */
class BGPUpdateDumper : public BGPDumper 
{
public:
	BGPUpdateDumper(const BGPMessagePtr &msg);
	virtual ~BGPUpdateDumper();

    // XML output
	xmlNodePtr genUpdateWithdrawnNode(const list<RoutePtr>& Routes);
	xmlNodePtr genUpdateNlriNode(const list<RoutePtr>& Routes);
	xmlNodePtr genPrefixNode( const RoutePtr &route, uint32_t afi, uint32_t safi);
	xmlNodePtr genUpdateAttributesNode(const std::list<BGPAttributePtr>& pathAttributes);
	virtual xmlNodePtr genXmlAsciiNode();

    // Ascii output
    //virtual list<string> genAsciiMsg(); 
    virtual list<string> genAsciiMsg(string peer_addr, string peer_as, bool is_table_dump); 
protected:
};

/* Keepalive */
class BGPKeepAliveDumper : public BGPDumper 
{
public:
	BGPKeepAliveDumper(const BGPMessagePtr &msg);
	virtual ~BGPKeepAliveDumper();
protected:
};

/* Notification */
class BGPNotificationDumper : public BGPDumper 
{
public:
	BGPNotificationDumper(const BGPMessagePtr &msg);
	virtual ~BGPNotificationDumper();
protected:
};

/* Route Refresh */
class BGPRouteRefreshDumper : public BGPDumper 
{
public:
	BGPRouteRefreshDumper(const BGPMessagePtr &msg);
	virtual ~BGPRouteRefreshDumper();
protected:
};

#endif /* __BGPDUMPER_H_ */

// vim: sw=4 ts=4 sts=4 expandtab
