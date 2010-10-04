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

/* BGPUpdate */
// Author: Jason Ryder
#ifndef __BGPUPDATE_H_
#define __BGPUPDATE_H_

#include "BGPCommonHeader.h"
#include "BGPStructure.h"

class BGPUpdate: public BGPCommonHeader
{
public:
	BGPUpdate(BGPCommonHeader &header, std::istream &input, bool isAS4);
	virtual ~BGPUpdate();
	
	virtual BGP_MESSAGE_TYPE Type()    { return UPDATE;   }
	virtual std::string      TypeStr() { return "UPDATE"; };
	
	inline uint16_t getWithdrawnRoutesLength() { return withdrawnRoutesLength; }
	inline uint16_t getPathAttributesLength()  { return pathAttributesLength; }
	inline uint16_t getNlriLength() 		   { return nlriLength; }
	
	const std::list<BGPAttributePtr>& getPathAttributes()  { return pathAttributes; }
	const std::list<RoutePtr>& 		  getWithdrawnRoutes() { return withdrawnRoutes; }
	const std::list<RoutePtr>& 		  getNlriRoutes()      { return announcedRoutes; }
	
	virtual void printMe();
	virtual void printMeCompact();
	
	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

protected:
	BGPUpdate( );

protected:
	uint16_t withdrawnRoutesLength;
	std::list<RoutePtr> withdrawnRoutes;
	
	uint16_t pathAttributesLength;
	std::list<BGPAttributePtr> pathAttributes;
	
	uint16_t nlriLength; 	// = length - sizeof(withdrawnRoutesLength) - sizeof(pathAttributesLength)
							//			- withdrawnRoutesLength - pathAttributesLength
	std::list<RoutePtr> announcedRoutes;

	static log4cxx::LoggerPtr Logger;
};

typedef boost::shared_ptr<BGPUpdate> BGPUpdatePtr;

#endif /* __BGPUPDATE_H_ */
