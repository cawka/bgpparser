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

#include "BGPNotification.h"

#include <boost/iostreams/read.hpp>
#include <boost/iostreams/skip.hpp>
namespace io = boost::iostreams;

using namespace std;

log4cxx::LoggerPtr BGPNotification::Logger = log4cxx::Logger::getLogger( "bgpparser.BGPNotification" );

/*
      0                   1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
      | Error code    | Error subcode |   Data (variable)             |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

 Note that the length of the Data field can be determined from
 the message Length field by the formula:

		  Message Length = 21 + Data Length

 */

BGPNotification::BGPNotification( BGPCommonHeader &header, istream &input )
: BGPCommonHeader( header )
{
	io::read( input, reinterpret_cast<char*>(&errorCode),    sizeof(uint8_t) );
	io::read( input, reinterpret_cast<char*>(&subErrorCode), sizeof(uint8_t) );

	/* calculate the data length */
	uint32_t dataLength = getLength() - 21;

	if (dataLength > 0)
	{
		/* dynamically allocated memory for the data */
		data = boost::shared_ptr<uint8_t>( new uint8_t[dataLength] );

		/* copy the data from the notification to the */
		/* dynamically allocated memory */
		io::read( input, reinterpret_cast<char*>(data.get()), dataLength );
	}

	switch (errorCode) {
	case BGP_NOTIFICATION_MSG_HEADER_ERROR: {
			switch (subErrorCode) {
			case UNSPECIFIC:
				break;
			case CONNECTION_NOT_SYNCHRONIZED:
				break;
			case BAD_MESSAGE_LENGTH: {
					uint16_t temp;
					/* data should store the length value - a 2 octet short value */
					memcpy(&temp, data.get(), sizeof(uint16_t));
					temp = ntohs(temp);
					memcpy(data.get(), &temp, sizeof(uint16_t));
				}
				break;
			case BAD_MESSAGE_TYPE: {
					/* data field should store the bad type field - don't know how */
					/* this is interpreted*/
				}
				break;
			default:
				LOG4CXX_ERROR( Logger,"unknown sub-error code ["<< subErrorCode <<"] in BGP Notification" );
				break;
			}
		}
		break;
	case BGP_NOTIFICATION_OPEN_MSG_ERROR: {
			switch (subErrorCode) {
			case UNSPECIFIC:
				break;
			case UNSUPPORTED_VERSION_NUMBER: {
					uint16_t temp;
					/* data should store the version number supported */
					memcpy(&temp, data.get(), sizeof(uint16_t));
					temp = ntohs(temp);
					memcpy(data.get(), &temp, sizeof(uint16_t));
				}
				break;
			case BAD_PEER_AS:
				break;
			case BAD_BGP_IDENTIFIER:
				break;
			case UNSUPPORTED_OPTIONAL_PARAMETER:
				break;
			case AUTHENTICATION_FAILURE:
				break;
			case UNACCEPTABLE_HOLD_TIME:
				break;
			default:
				LOG4CXX_ERROR( Logger,"unknown sub-error code ["<< subErrorCode <<"] in BGP Open" );
				break;
			}
		}
		break;
	case BGP_NOTIFICATION_UPDATE_MSG_ERROR: {
			switch (subErrorCode) {
			case UNSPECIFIC:
				break;
			case MALFORMED_ATTRIBUTE_LIST:
				break;
			case UNRECOGNIZED_WELL_KNOWN_ATTRIBUTE:
				break;
			case MISSING_WELL_KNOWN_ATTRIBUTE:
				break;
			case ATTRIBUTE_FLAGS_ERROR:
				break;
			case ATTRIBUTE_LENGTH_ERROR:
				break;
			case INVALID_ORIGIN_ATTRIBUTE:
				break;
			case AS_ROUTING_LOOP:
				break;
			case INVALID_NEXT_HOP_ATTRIBUTE:
				break;
			case OPTIONAL_ATTRIBUTE_ERROR:
				break;
			case INVALID_NETWORK_FIELD:
				break;
			case MALFORMED_AS_PATH:
				break;
			default:
				break;
			}
		}
		break;
	case BGP_NOTIFICATION_HOLD_TIMER_EXPIRED:
		break;
	case BGP_NOTIFICATION_FINITE_STATE_ERROR:
		break;
	case BGP_NOTIFICATION_CEASE:
		break;
	default:
		LOG4CXX_ERROR( Logger,"unknown error code ["<< errorCode <<"] in BGP Notification" );
		break;
	}
}

BGPNotification::~BGPNotification(void) {
}

void BGPNotification::printMe() {
	/* nothing */
}

void BGPNotification::printMeCompact() {
	std::cout << errorCode << "|" << subErrorCode;
}
