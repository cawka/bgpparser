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

// Author: Jason Ryder
// Author: Paul Wang

// BGP Message Common Header
#ifndef __BGPCOMMOMHEADER_H_
#define __BGPCOMMOMHEADER_H_

#include "BGPAttribute.h"

/*
 *
	The maximum message size is 4096 octets.  
	The smallest message that may be sent consists of a BGP header without a data portion (19 octets).
	All multi-octet fields are in network byte order.
	
	Each message has a fixed-size header.  There may or may not be a data
   portion following the header, depending on the message type.  The
   layout of these fields is shown below:

      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |                                                               |
      +                                                               +
      |                                                               |
      +                                                               +
      |                           Marker                              |
      +                                                               +
      |                                                               |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      |          Length               |      Type     |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

      Marker:

         This 16-octet field is included for compatibility; it MUST be
         set to all ones.

      Length:

         This 2-octet uint32_teger indicates the total length of the
         message, including the header in octets.  Thus, it allows one
         to locate the (Marker field of the) next message in the TCP
         stream.  The value of the Length field MUST always be at least
         19 and no greater than 4096, and MAY be further constrained,
         depending on the message type.  "padding" of extra data after
         the message is not allowed.  Therefore, the Length field MUST
         have the smallest value required, given the rest of the
         message.

      Type:

         This 1-octet uint32_teger indicates the type code of the
         message.  This document defines the following type codes:

                              1 - OPEN
                              2 - UPDATE
                              3 - NOTIFICATION
                              4 - KEEPALIVE

         [RFC2918] defines one more type code.
                              5 - ROUTE-REFRESH
 *
 */

#define MESSAGE_HEADER_SIZE			19
#define MAX_MESSAGE_SIZE 			4096
#define MIN_MESSAGE_SIZE 			MESSAGE_HEADER_SIZE

/*
#define OPEN 							1
#define UPDATE 						2
#define NOTIFICATION 			3
#define KEEPALIVE					4
#define ROUTE-REFRESH 		5
*/


/*
 *
	OPEN Message Format

   After a TCP connection is established, the first message sent by each
   side is an OPEN message.  If the OPEN message is acceptable, a
   KEEPALIVE message confirming the OPEN is sent back.

   In addition to the fixed-size BGP header, the OPEN message contains
   the following fields:

       0                   1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+
       |    Version    |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |     My Autonomous System      |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |           Hold Time           |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                         BGP Identifier                        |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       | Opt Parm Len  |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                                                               |
       |             Optional Parameters (variable)                    |
       |                                                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 *
 */ 


//typedef struct BGPMessage_t
//{
//	// NOTE: The first 4 fields come from MRT header
//	long time;		// epoch time
//	std::string* type;	// update, withdraw, RIB
//	std::string* peerIP;	// of the router sending the update
//	std::string* peerAS;	// AS number of router sending updating
//	std::string* prefix;	// prefix being announced
//	std::set<uint32_t>* sASes;	// all ASes present in AS path including AS_SET
//	std::vector<std::string>* vASPath;	// ASPathe w/out prepending
//	std::set<std::string>   * sLinks;			// set of links in AS path, in A {B, C}, assume A-B, A-C and B-C
//	std::string* nexthopIP;				// next HOP IP
//	std::set<uint32_t>* sOrigins;	// need a set here because of AS_SET
//	uint32_t nexthopAS;
//	std::string* line;							// contains the ENTIRE raw line
//} BGPMessage_t;

class BGPCommonHeader;
typedef BGPCommonHeader BGPMessage;
typedef boost::shared_ptr<BGPMessage> BGPMessagePtr;

class BGPCommonHeader /* AKA BGPMessage */ : public Node
{
public:
	enum BGP_MESSAGE_TYPE
	{
		UNKNOWN, // Not a defined BGP message type
		OPEN,
		UPDATE,
		NOTIFICATION,	
		KEEPALIVE,
		ROUTE_REFRESH
	};


	BGPCommonHeader( std::istream &input );
	virtual ~BGPCommonHeader();
	
	virtual BGP_MESSAGE_TYPE Type()    { return UNKNOWN;   };
	virtual std::string      TypeStr() { return "UNKNOWN"; };
	
//	// Getters and Setters
	const uint8_t* getMarker() const { return marker; }
	uint16_t getLength() const { return length; }
	uint8_t  getType()   const { return type; }

	const boost::shared_ptr<char> &getData() const { return data; }
//	uint8_t hasError() { return error; }
	
//	BGPMessage_t& getBgpData() { return bgpData; }
	
	// Factory method for creating a BGP message instance.
	static BGPMessagePtr newMessage( std::istream &input, bool isAS4 );

	virtual void accept( Visitor &v ) 							{ v.visit( *this ); }
	virtual void accept( GJVoidVisitor &v, boost::any param )   { v.visit( *this, param ); }
	virtual boost::any accept( GJNoArguVisitor &v ) 		    { return v.visit( *this ); }
	virtual boost::any accept( GJVisitor &v, boost::any param ) { return v.visit( *this, param ); }

protected:
	BGPCommonHeader( uint8_t type );

protected:
	// There is a 16-byte marker that is all 1s
	uint8_t marker[16];
	uint16_t length;
	uint8_t type;
	boost::shared_ptr<char> data;

private:
	static log4cxx::LoggerPtr Logger;
};


#endif /* __BGPCOMMOMHEADER_H_ */
