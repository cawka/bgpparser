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

// testr.cpp : Defines the entry point for the console application.
// Author: Jason Ryder
//
#define _USE_32BIT_TIME_T
#include <cstdio>
#include <cstdlib>

#include <ctime>
#include <MRTCommonHeader.h>
#include <MRTBgp4MPStateChange.h>
#include <MRTBgp4MPMessage.h>
#include <MRTBgp4MPEntry.h>
#include <MRTBgp4MPSnapshot.h>
#include <MRTTblDump.h>
#include <MRTTblDumpV2PeerIndexTbl.h>
#include <MRTTblDumpV2RibIPv4Unicast.h>
#include <MRTTblDumpV2RibIPv4Multicast.h>
#include <MRTTblDumpV2RibIPv6Unicast.h>
#include <MRTTblDumpV2RibIPv6Multicast.h>

#include <BGPCommonHeader.h>
#include <BGPUpdate.h>

#include <AttributeType.h>
#include <AttributeTypeOrigin.h>

#include <log4cxx/Logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/defaultconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

extern "C" {
	#include "cfile_tools.h"
}

#define INPUT_MAX_SIZE 0x4000000  // ~67MB
#define OUTPUT_MAX_SIZE 0x4000000 // ~67MB
#define MAX_MRT_TYPE_NUM 49 
#define MAX_MRT_SUBTYPE_NUM 6

#define MRT_COMMON_HDR_LEN 12

// File types
#define UNKNOWN 0
#define GZIP	1
#define BZIP2	2
#define XML 	3

const uint32_t rnFlags[] = { 
                            0,0,0,0,0,0,0,0,0,0,                        //  0 -  9
						    0,0,0,0,0,0,0,0,0,0,                        // 10 - 19
						    0,0,0,0,0,0,0,0,0,0,                        // 20 - 29
							0,0,0,0,0,0,0,0,0,0,                        // 30 - 39
							0,0,0,0,0,0,0,0,0,0,                        // 40 - 49
							0,0,0,0,0,0,0,0,0,0,                        // 50 - 59
							0,0,0,0,0,0,0,0,0,0,                        // 60 - 69
							0,0,0,0,0,0,0,0,0,0,                        // 70 - 79
							0,0,0,0,0,0,0,0,0,0,                        // 80 - 89
							0,0,0,0,0,0,0,                              // 90 - 96
							1,2,4,8,0x10,0x20,0x40,0x80,0x100,0x200,    // 'a' - 'j'
							0x400,0x800,0x1000,0x2000,0x4000,0x8000,    // 'k' - 'p'
							0x10000,0x20000,0x40000,0x80000,            // 'q' - 't'
							0x100000,0x200000,0x400000,0x800000,        // 'u' - 'x'
							0x1000000,0x2000000,                        // 'y' - 'z'
							0,0,0,0,0,0,0,0,0,0
                        };
											 
#define PRINT_COMPACT rnFlags[109]            // flag m

const char* rrchMRTTypes[] = {
                            "NULL",
                            "START",			// 1
                            "DIE",
                            "I_AM_DEAD",		// 3
                            "PEER_DOWN", 
                            "BGP",			// --5--
                            "RIP", 
                            "IDRP", 
                            "RIPNG", 
                            "BGP4PLUS", 
                            "BGP4PLUS_01",	// --10--
                            "OSPF",			// 11
                            "TABLE_DUMP",	// 12
                            "TABLE_DUMP_V2",	// 13
                            "UNKNOWN", 
                            "UNKNOWN",		// --15-- 
                            "BGP4MP",		// 16
                            "BGP4MP_ET",		// 17
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN",		// --20--
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN",		// --25--
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN",		// --30--
                            "UNKNOWN",
                            "ISIS",			// 32
                            "ISIS_ET",		// 33
                            "UNKNOWN", 
                            "UNKNOWN",		// --35--
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN",		// --40--
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "UNKNOWN",		// --45--
                            "UNKNOWN", 
                            "UNKNOWN", 
                            "OSPFv3",		// 48
                            "OSPFv3_ET"		// 49
						};

const char* rrrchMRTSubTypes[][MAX_MRT_SUBTYPE_NUM + 1] 
						= {	  
							{/* 0 */}, {/* 1 */},
							{/* 2 */}, {/* 3 */},
							{/* 4 */}, {/* 5 */},
							{/* 6 */}, {/* 7 */},
							{/* 8 */}, {/* 9 */},
							{/* 10 */}, 
							{"OSPF_STATE_CHANGE", "OSPF_LSA_UPDATE"},	// 11
							{"UNKNOWN", "AFI_IPv4", "AFI_IPv6"},			// 12
							{"UNKNOWN", "PEER_INDEX_TABLE",						// 13
								"RIB_IPV4_UNICAST", "RIB_IPV4_MULTICAST",
								"RIB_IPV6_UNICAST", "RIB_IPV6_MULTICAST",
								"RIB_GENERIC" },
							{/* 14 */}, {/* 15 */},
							{"BGP4MP_STATE_CHANGE", "BGP4MP_MESSAGE",	// 16
								"BGP4MP_ENTRY", "BGP4MP_SNAPSHOT",
								"BGP4MP_MESSAGE_AS4", "BGP4MP_STATE_CHANGE_AS4"}, 
							{/* 17 */}, {/* 18 */},
							{/* 19 */}, {/* 20 */},
							{/* 21 */}, {/* 22 */},
							{/* 23 */}, {/* 24 */},
							{/* 25 */}, {/* 26 */},
							{/* 27 */}, {/* 28 */},
							{/* 29 */}, {/* 30 */},
							{/* 31 */},
							{"UNKNOWN"}, {"UNKNOWN"},					// 32, 33
							{/* 34 */},
							{/* 35 */}, {/* 36 */},
							{/* 37 */}, {/* 38 */},
							{/* 39 */}, {/* 40 */},
							{/* 41 */}, {/* 42 */},
							{/* 43 */}, {/* 44 */},
							{/* 45 */}, {/* 46 */},
							{/* 47 */}, 
							{"UNKNOWN"}, {"UNKNOWN"}					// 48, 49
						};

static LoggerPtr _log=Logger::getLogger( "simple" );

int main(int argc, char** argv)
{
	uint32_t    unFlags = 0;
	char*       pchFileName;
    char        rchMRTCommonHeader[MRT_COMMON_HDR_LEN];

	if (argc < 2)
	{
		cout << "Usage:\n\tsimple <MRT File> [<log4cxx config file>]"
			 << endl;
		exit( -1 );
	}

	// configure Logger
	if( exists("log4cxx.properties") )
		PropertyConfigurator::configureAndWatch( "log4cxx.properties" );
	else if( argc==3 )
		PropertyConfigurator::configure( argv[2] );
	else
		BasicConfigurator::configure( );

	_log->info( "Parsing started" );

	// Set any flags
	if( string(argv[1])=="-" )
	{
		unFlags = 0x80000000;
		char* pchFlagVals = argv[1];
		while(*(++pchFlagVals) != '\0')
			unFlags |= rnFlags[(uint32_t)*pchFlagVals];
	}

	//printf("flags = 0x%08x\n", unFlags);
	pchFileName = (unFlags == 0 ? argv[1] : argv[2]);
	//cout << "File: " << pchFileName << endl;
    MRTTblDumpV2PeerIndexTbl* peerIndexTbl = NULL; 

    CFRFILE *f;
    f = cfr_open(pchFileName);
    if (f)
    {
        // The index table needs to persist over the processing
        uint32_t unTotalBytesRead = 0;

        while(!cfr_eof(f))
        {
            size_t szBytesRead = 0;

            /* Read header from file */
            szBytesRead += cfr_read_n(f, rchMRTCommonHeader, MRT_COMMON_HDR_LEN);
            if( szBytesRead < MRT_COMMON_HDR_LEN ) { break; }

            /* Compute MRT message length */
            uint32_t unMRTBodyLen = ntohl(*(uint32_t*)(rchMRTCommonHeader+8));
            uint32_t unMRTMsgLen = MRT_COMMON_HDR_LEN + unMRTBodyLen;
            char *pchMRTMsg = (char *)malloc(unMRTMsgLen);
            memset(pchMRTMsg, 0, unMRTMsgLen);

            /* Copy the whole MRT record */

            memcpy(pchMRTMsg, (char *)rchMRTCommonHeader, MRT_COMMON_HDR_LEN);
            szBytesRead += cfr_read_n(f, pchMRTMsg + MRT_COMMON_HDR_LEN, unMRTBodyLen);
            unTotalBytesRead += szBytesRead;

            /* Parse the common header */
            int nSaveMsg = false;
            uint8_t *pchMRTMsgTemp = reinterpret_cast<uint8_t *>(pchMRTMsg);
            MRTMessage* msg = MRTCommonHeader::newMessage(&pchMRTMsgTemp);
            if (msg == NULL) { continue; } /* EOF? */

            /* Get time to display */
            time_t      tmTime = (time_t)msg->getTimestamp();
            struct tm   *ts;
            char        rchTime[80];
         
            /* Format and print the time, "ddd yyyy-mm-dd hh:mm:ss zzz" */
            ts = localtime(&tmTime);
            strftime(rchTime, sizeof(rchTime), "%a %Y-%m-%d %H:%M:%S %Z", ts);

            if ((unFlags & PRINT_COMPACT) == 0)
                printf("\nTIME: %s\n", rchTime);
            else
                printf("%u", (uint32_t)tmTime);

            if ((unFlags & PRINT_COMPACT) == 0)
            {
                cout << "TYPE: " << rrchMRTTypes[msg->getType()] << "/";
                cout << rrrchMRTSubTypes[msg->getType()][msg->getSubType()]; 
            }
            
            switch(msg->getType())
            {
                case TABLE_DUMP:
                {
                    if (unFlags & PRINT_COMPACT) {
                        cout << "|" << rrchMRTTypes[msg->getType()];
                        cout << "|" << msg->getSubType();
                    }
                    else
                    {
                        cout << endl;
                    }
                    switch(msg->getSubType())
                    {
                        case AFI_IPv4:
                        case AFI_IPv6:
                        {
                            MRTTblDump* tblDump = (MRTTblDump*)msg;
                            tblDump->printMeCompact();
                        }
                        break;

                        default:
                            break;
                    }		
                }
                break;

                case TABLE_DUMP_V2:
                {
                    if (unFlags & PRINT_COMPACT) {
                        cout << "|" << rrchMRTTypes[msg->getType()];
                        cout << "|";
                    }
                    else
                    {
                        cout << endl;
                    }
                    switch(msg->getSubType())
                    {
                        case RIB_IPV4_UNICAST:
                        {
                            MRTTblDumpV2RibIPv4Unicast* tblDumpIpv4Msg = (MRTTblDumpV2RibIPv4Unicast*)msg;
                            // consider passing peerIndexTbl to printMe
                            if (unFlags & PRINT_COMPACT)
                                tblDumpIpv4Msg->printMeCompact(peerIndexTbl);
                            else
                                tblDumpIpv4Msg->printMe(peerIndexTbl);
                        }
                        break;

                        case RIB_IPV4_MULTICAST:
                        {
                            MRTTblDumpV2RibIPv4Multicast* tblDumpIpv4Msg = (MRTTblDumpV2RibIPv4Multicast*)msg;
                            //cout << endl;
                            tblDumpIpv4Msg->printMe(peerIndexTbl);
                        }
                        break;

                        case RIB_IPV6_UNICAST:
                        {
                            MRTTblDumpV2RibIPv6Unicast* tblDumpIpv6Msg = (MRTTblDumpV2RibIPv6Unicast*)msg;
                            //cout << endl;
                            if (unFlags & PRINT_COMPACT)
                                tblDumpIpv6Msg->printMeCompact(peerIndexTbl);
                            else
                                tblDumpIpv6Msg->printMe(peerIndexTbl);
                        }
                        break;

                        case RIB_IPV6_MULTICAST:
                        {
                            MRTTblDumpV2RibIPv6Multicast *tblDumpIpv6Msg = (MRTTblDumpV2RibIPv6Multicast *)msg;
                            //cout << endl;
                            tblDumpIpv6Msg->printMe(peerIndexTbl);
                        }
                        break;
                        
                        case PEER_INDEX_TABLE:
                        {
                            // We need to preserve the index while we process the list of MRT table records
                            nSaveMsg = true;
                            if (peerIndexTbl) {
                                delete peerIndexTbl; // Make way for the next one.
                            }
                    
                            //cout << "  Setting a peer index table." << cout;
                            peerIndexTbl = (MRTTblDumpV2PeerIndexTbl*)msg;
                            cout << "PEERINDEXTABLE";
                        }
                        break;

                        default:
                            break;
                    }		
                }
                break;
                
                case BGP4MP:
                {
                    switch(msg->getSubType())
                    {
                        case BGP4MP_MESSAGE:
                        {
                            if (unFlags & PRINT_COMPACT ) {
                                cout << "|" << rrchMRTTypes[msg->getType()];
                            }
                            MRTBgp4MPMessage* bgp4MPmsg = (MRTBgp4MPMessage*)msg;
                            BGPMessage* bgpMessage = (BGPMessage*)msg->getPayload();
                            if( bgpMessage == NULL ) { break; }
                            switch(bgpMessage->Type())
                            {
                                case BGPCommonHeader::UPDATE:
                                {
                                    if ((unFlags & PRINT_COMPACT) == 0)
                                    {
                                        cout << "/" << "Update" ;
                                        cout << endl;
                                        BGPUpdate* bgpUpdate = (BGPUpdate*)bgpMessage;
                                        cout << "FROM: ";
                                        PRINT_IP_ADDR(bgp4MPmsg->getPeerIP().ipv4);
                                        cout << " AS" << bgp4MPmsg->getPeerAS() << endl;
                                        cout << "TO: ";
                                        PRINT_IP_ADDR(bgp4MPmsg->getLocalIP().ipv4);
                                        cout << " AS" << bgp4MPmsg->getLocalAS() << endl;
                            
                                        bgpUpdate->printMe();
                                    }
                                    else
                                    {
                                        cout << "|";
                                        BGPUpdate* bgpUpdate = (BGPUpdate*)bgpMessage;
                                        if( bgp4MPmsg->getAddressFamily() == AFI_IPv4 ) {
                                            PRINT_IP_ADDR(bgp4MPmsg->getPeerIP().ipv4);
                                            cout << "|" << bgp4MPmsg->getPeerAS() << "|";
                                            PRINT_IP_ADDR(bgp4MPmsg->getLocalIP().ipv4);
                                            cout << "|" << bgp4MPmsg->getLocalAS() << "|";
                                        } else {
                                            PRINT_IPv6_ADDR(bgp4MPmsg->getPeerIP().ipv6);
                                            cout << "|" << bgp4MPmsg->getPeerAS() << "|";
                                            PRINT_IPv6_ADDR(bgp4MPmsg->getLocalIP().ipv6);
                                            cout << "|" << bgp4MPmsg->getLocalAS() << "|";
                                        }
                                        bgpUpdate->printMeCompact();
                                    }
                                }
                                break;
                        
                                case BGPCommonHeader::KEEPALIVE: 
                                {
                                    cout << "/" << "KEEPALIVE";
                                }
                                break;

                                default: 
                                break;
                            }
                        }
                        break;
                        
                        case BGP4MP_MESSAGE_AS4:
                        {
                            if (unFlags & PRINT_COMPACT ) {
                                cout << "|" << rrchMRTTypes[msg->getType()];
                            }
                            MRTBgp4MPMessage* bgp4MPmsg = (MRTBgp4MPMessage*)msg;
                            BGPMessage* bgpMessage = (BGPMessage*)msg->getPayload();
                            if( bgpMessage == NULL ) { break; }
                            switch(bgpMessage->Type())
                            {
                                case BGPCommonHeader::UPDATE:
                                {
                                    if ((unFlags & PRINT_COMPACT) == 0)
                                    {
                                        cout << "/" << "Update" ;
                                        cout << endl;
                                        BGPUpdate* bgpUpdate = (BGPUpdate*)bgpMessage;
                                        cout << "FROM: ";
                                        PRINT_IP_ADDR(bgp4MPmsg->getPeerIP().ipv4);
                                        cout << " AS" << bgp4MPmsg->getPeerAS() << endl;
                                        cout << "TO: ";
                                        PRINT_IP_ADDR(bgp4MPmsg->getLocalIP().ipv4);
                                        cout << " AS" << bgp4MPmsg->getLocalAS() << endl;
                            
                                        bgpUpdate->printMe();
                                    }
                                    else
                                    {
                                        cout << "|";
                                        BGPUpdate* bgpUpdate = (BGPUpdate*)bgpMessage;
                                        if( bgp4MPmsg->getAddressFamily() == AFI_IPv4 ) {
                                            PRINT_IP_ADDR(bgp4MPmsg->getPeerIP().ipv4);
                                        } else {
                                            PRINT_IPv6_ADDR(bgp4MPmsg->getPeerIP().ipv6);
                                        }

                                        uint16_t t, b;
                                        uint32_t asn;
                                        cout << "|";
                                        asn = bgp4MPmsg->getPeerAS();
                                        t = (uint16_t)((asn>>16)&0xFFFF); 
                                        b = (uint16_t)((asn)&0xFFFF); 
                                        if( t == 0 ) { 
                                            printf("%u",b);
                                        } else {
                                            printf("%u.%u",t,b);
                                        }
                                        cout << "|";

                                        if( bgp4MPmsg->getAddressFamily() == AFI_IPv4 ) {
                                            PRINT_IP_ADDR(bgp4MPmsg->getLocalIP().ipv4);
                                        } else {
                                            PRINT_IPv6_ADDR(bgp4MPmsg->getLocalIP().ipv6);
                                        }

                                        cout << "|";
                                        asn = bgp4MPmsg->getLocalAS();
                                        t = (uint16_t)((asn>>16)&0xFFFF); 
                                        b = (uint16_t)((asn)&0xFFFF); 
                                        if( t == 0 ) { 
                                            printf("%u",b);
                                        } else {
                                            printf("%u.%u",t,b);
                                        }
                                        cout << "|";
                                        bgpUpdate->printMeCompact();
                                    }
                                }
                                break;
                        
                                case BGPCommonHeader::KEEPALIVE: 
                                {
                                    cout << "/" << "KEEPALIVE";
                                }
                                break;

                                default: break;
                            }
                        }
                        break;

                        case BGP4MP_STATE_CHANGE:
                        {
                            MRTBgp4MPStateChange* bgp4MPmsg = (MRTBgp4MPStateChange*)msg;
                            if (unFlags & PRINT_COMPACT ) {
                                cout << "|" << "STATE" << "|";
                                bgp4MPmsg->printMeCompact();
                            } else {
                                cout << "/" << "STATE_CHANGE";
                                bgp4MPmsg->printMe();
                            }
                        }
                        break;
                        case BGP4MP_STATE_CHANGE_AS4:
                        {
                            MRTBgp4MPStateChange* bgp4MPmsg = (MRTBgp4MPStateChange*)msg;
                            if (unFlags & PRINT_COMPACT ) {
                                cout << "|" << "STATE" << "|";
                                bgp4MPmsg->printMeCompact();
                            } else {
                                cout << "/" << "STATE_CHANGE_AS4";
                                bgp4MPmsg->printMe();
                            }
                        }
                        break;
                        
                        default: break;
                    }
                }
                break;
            
                default: break;		
            }

            cout << endl;
            delete [] pchMRTMsg;
            /* if this is not peerIndexTable, free allocated memory */
            if( !nSaveMsg ) { if( msg != NULL ) delete msg; }
        }

        if (peerIndexTbl)
        {
            delete peerIndexTbl;
            peerIndexTbl = NULL;
        }
    }

	_log->info( "Parsing ended" );
	return 0;
}

// vim: sw=4 ts=4 sts=4 expandtab
