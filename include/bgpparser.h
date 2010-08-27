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

#ifndef _BGPPARSER_H_
#define _BGPPARSER_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <log4cxx/logger.h>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>

#ifdef WIN32
/* enable 32-bit time_t structure */
#define _USE_32BIT_TIME_T
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
#include <ctime>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif	/* WIN32 */


#define BITMASK_8 0x000000FF
#define BITMASK_16 0x0000FFFF


#ifdef DEBUG
#	define _PRINT_HEX_VALL(l,x,name) { if( l>=DBG_LEVEL ) { printf("  %s value: 0x%02x%02x%02x%02x\n", name, \
															 *x & BITMASK_8, \
															 *(x+1) & BITMASK_8, \
															 *(x+2) & BITMASK_8, \
															 *(x+3) & BITMASK_8 ); } }
#	define _PRINT_IP_ADDR_DBGL(l,x) { if( l>=DBG_LEVEL ) { fprintf(stderr,"%01i.%01i.%01i.%01i", *(uint8_t*)&x & BITMASK_8, \
															*(((uint8_t*)&x)+1) & BITMASK_8, \
															*(((uint8_t*)&x)+2) & BITMASK_8, \
															*(((uint8_t*)&x)+3) & BITMASK_8 ); } }
#	define _PRINT_INC_IP_ADDR_DBGL(l,x,numOctets) { if( l>=DBG_LEVEL ) { \
			numOctets > 0 ? fprintf(stderr,"%01i", *(uint8_t*)&x & BITMASK_8); \
			for (int i=1; i < numOctets; i++) fprintf(stderr,".%01i", *(((uint8_t*)&x)+i) & BITMASK_8 ); } }
#	define _PRINT_IPv6_ADDR_DBGL(l,x) \
			{ if( l>=DBG_LEVEL ) { \
				fprintf(stderr,"%01x:%01x:%01x:%01x:%01x:%01x:%01x:%01x", *(uint16_t*)&x & BITMASK_16, \
																*(((uint16_t*)&x)+1) & BITMASK_16, \
																*(((uint16_t*)&x)+2) & BITMASK_16, \
																*(((uint16_t*)&x)+3) & BITMASK_16, \
																*(((uint16_t*)&x)+4) & BITMASK_16, \
																*(((uint16_t*)&x)+5) & BITMASK_16, \
																*(((uint16_t*)&x)+6) & BITMASK_16, \
																*(((uint16_t*)&x)+7) & BITMASK_16 ); } }
#	define _PRINT_ADDRL(l,x,name) { if( l>=DBG_LEVEL ) { fprintf(stderr,"  %s addr: 0x%08x\n", name, x); } }

#	define _PRINT_HEX_VAL(x,name) PRINT_HEX_VALL(1,x,name)
#	define _PRINT_IP_ADDR_DBG(x) PRINT_IP_ADDR_DBGL(1,x)
#	define _PRINT_INC_IP_ADDR_DBG(x,numOctets) PRINT_INC_IP_ADDR_DBGL(1,x,numOctets)
#	define _PRINT_IPv6_ADDR_DBG(x) PRINT_IPv6_ADDR_DBGL(1,x)
#	define _PRINT_ADDR(x,name) PRINT_ADDRL(1,x,name)
#else
#	define _PRINT_HEX_VAL(x,name) { }
#	define _PRINT_IP_ADDR_DBG(x) { }
#	define _PRINT_INC_IP_ADDR_DBG(x,numOctets) { }
#	define _PRINT_IPv6_ADDR_DBG(x) { }
#	define _PRINT_ADDR(x,name) { }

#	undef  LOG4CXX_DEBUG
#	define LOG4CXX_DEBUG(logger,msg) { }
#	undef  LOG4CXX_TRACE
#	define LOG4CXX_TRACE(logger,msg) { }
#endif

#define _PRINT_INC_IP_ADDR(x,numOctets) { \
					numOctets > 0 ? printf("%01i", *(uint8_t*)&x & BITMASK_8) : printf(""); \
					for (int i=1; i < numOctets ; i++) printf(".%01i",	*(((uint8_t*)&x)+i) & BITMASK_8 ); }

// inet_ntoa uses a static buffer which is overwritten with each write.  No need to free memory.
#define PRINT_IP_ADDR(x)   { std::cout << boost::asio::ip::address_v4( x.s_addr ); }
#define PRINT_IPv6_ADDR(x) { boost::asio::ip::address_v6::bytes_type ipv6; \
std::copy( x.__u6_addr.__u6_addr8, x.__u6_addr.__u6_addr8+16, ipv6.begin() ); \
std::cout << boost::asio::ip::address_v6( ipv6 ); }


#endif
