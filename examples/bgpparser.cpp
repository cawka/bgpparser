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

#include <bgpparser.h>

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
#include <Exceptions.h>

#include <BGPCommonHeader.h>
#include <BGPUpdate.h>

#include <AttributeType.h>
#include <AttributeTypeOrigin.h>

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/consoleappender.h>
#include <log4cxx/patternlayout.h>
#include <log4cxx/level.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/defaultconfigurator.h>
#include <log4cxx/helpers/exception.h>
using namespace log4cxx;
using namespace log4cxx::helpers;

#include <boost/regex.hpp>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
namespace po = boost::program_options;

#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/detail/iostream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/scoped_array.hpp>

#include <boost/iostreams/categories.hpp>
namespace io = boost::iostreams;

using namespace std;
using namespace boost;

#define MAX_MRT_TYPE_NUM 49 
#define MAX_MRT_SUBTYPE_NUM 6

#define MRT_COMMON_HDR_LEN 12

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
    po::variables_map CONFIG;

	po::options_description opts( "General options" );
	opts.add_options()
		( "help", "Print this help message" )
		( "log",   po::value<string>(), "log4cxx configuration file" )
		( "file",  po::value<string>(),
				  "MRT file or - to input from stdin" )
		( "format", po::value<string>(),
				  "Compression format: txt, z, gz, or bz2 (will be overridden by the actual extension of the file)" )
	;

	po::positional_options_description p;
	p.add("file", -1);

	try
	{
		po::store( po::command_line_parser(argc, argv).
						options(opts).
						positional(p).run(),
				   CONFIG );
	}
	catch( const po::error &error )
	{
		cerr << "ERROR: " << error.what() << endl
			 <<	endl
			 <<	opts << endl;

		exit( 2 );
	}

	if( CONFIG.count("help")>0 )
	{
		cout << opts << endl;
		exit( 0 );
	}

	// configure Logger
	if( CONFIG.count("log")>0 )
		PropertyConfigurator::configureAndWatch( CONFIG["log"].as<string>() );
	else if( fs::exists("log4cxx.properties") )
		PropertyConfigurator::configureAndWatch( "log4cxx.properties" );
	else
	{
//		PatternLayoutPtr   layout   ( new PatternLayout("%d{HH:mm:ss} %p %c{1} - %m%n") );
//		ConsoleAppenderPtr appender ( new ConsoleAppender( layout ) );

//		BasicConfigurator::configure( appender );
		Logger::getRootLogger()->setLevel( log4cxx::Level::getOff() );
	}

	if( CONFIG.count("file")==0 )
	{
		cerr << "ERROR: Input file should be specified" << endl
			 << endl
			 << opts << endl;
		exit( 1 );
	}

	////////////////////////////////////////////////////////////////////////////
	string format=CONFIG.count("format")>0 ? CONFIG["format"].as<string>( ) : "";
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	string filename=CONFIG["file"].as<string>();
	smatch m;
	if( regex_match(filename, m, regex("^.*\\.(gz|bz2)$")) ) format=m[1];
	////////////////////////////////////////////////////////////////////////////

	io::filtering_stream<io::input> in;

	if( format=="gz" )
	{
		LOG4CXX_DEBUG( _log, "Input file has GZIP format" );
		in.push( io::gzip_decompressor() );
	}
	else if( format=="bz2" )
	{
		LOG4CXX_DEBUG( _log, "Input file has BZIP2 format" );
		in.push( io::bzip2_decompressor() );
	}

	ifstream input_file( filename.c_str(), ios_base::in | ios_base::binary );
	if( !input_file.is_open() )
	{
		cerr << "ERROR: " << "cannot open file [" <<  filename  << "] for reading" << endl
			 << endl;
		exit( 3 );
	}

	if( filename=="-" )
		in.push( cin );
	else
		in.push( input_file );


//	_log->info( "Parsing started" );

	// Set any flags
//	if( string(argv[1])=="--" )
//	{
//		unFlags = 0x80000000;
//		char* pchFlagVals = argv[1];
//		while(*(++pchFlagVals) != '\0')
//			unFlags |= rnFlags[(uint32_t)*pchFlagVals];
//	}

	//printf("flags = 0x%08x\n", unFlags);
	LOG4CXX_INFO( _log, "Parsing file [" << filename << "]" );
    MRTTblDumpV2PeerIndexTblPtr peerIndexTbl;

	MRTMessagePtr msg;
    int i=1;
	int mrt_id=0;
	try
	{
		while( in.peek()!=-1 )
		{
			mrt_id++;
//			LOG4CXX_INFO( _log, mrt_id++ );
            size_t szBytesRead = 0;

            msg=MRTCommonHeader::newMessage( in );

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
                            MRTTblDumpPtr tblDump = dynamic_pointer_cast<MRTTblDump>( msg );
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
                            MRTTblDumpV2RibIPv4UnicastPtr tblDumpIpv4Msg =
                            		dynamic_pointer_cast<MRTTblDumpV2RibIPv4Unicast>( msg );
                            // consider passing peerIndexTbl to printMe
                            if (unFlags & PRINT_COMPACT)
                                tblDumpIpv4Msg->printMeCompact(peerIndexTbl);
                            else
                                tblDumpIpv4Msg->printMe(peerIndexTbl);
                        }
                        break;

                        case RIB_IPV4_MULTICAST:
                        {
                            MRTTblDumpV2RibIPv4MulticastPtr tblDumpIpv4Msg =
                            		dynamic_pointer_cast<MRTTblDumpV2RibIPv4Multicast>( msg );
                            //cout << endl;
                            tblDumpIpv4Msg->printMe(peerIndexTbl);
                        }
                        break;

                        case RIB_IPV6_UNICAST:
                        {
                            MRTTblDumpV2RibIPv6UnicastPtr tblDumpIpv6Msg =
                            		dynamic_pointer_cast<MRTTblDumpV2RibIPv6Unicast>( msg );
                            //cout << endl;
                            if (unFlags & PRINT_COMPACT)
                                tblDumpIpv6Msg->printMeCompact(peerIndexTbl);
                            else
                                tblDumpIpv6Msg->printMe(peerIndexTbl);
                        }
                        break;

                        case RIB_IPV6_MULTICAST:
                        {
                            MRTTblDumpV2RibIPv6MulticastPtr tblDumpIpv6Msg =
                            		dynamic_pointer_cast<MRTTblDumpV2RibIPv6Multicast>( msg );
                            //cout << endl;
                            tblDumpIpv6Msg->printMe(peerIndexTbl);
                        }
                        break;
                        
                        case PEER_INDEX_TABLE:
                        {
                            //cout << "  Setting a peer index table." << cout;
                            peerIndexTbl = dynamic_pointer_cast<MRTTblDumpV2PeerIndexTbl>( msg );
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
                            MRTBgp4MPMessagePtr bgp4MPmsg = dynamic_pointer_cast<MRTBgp4MPMessage>( msg );

                            BGPMessagePtr bgpMessage = bgp4MPmsg->getPayload();
                            if( bgpMessage.get() == NULL ) { break; }
                            switch(bgpMessage->Type())
                            {
                                case BGPCommonHeader::UPDATE:
                                {
                                    if ((unFlags & PRINT_COMPACT) == 0)
                                    {
                                        cout << "/" << "Update" ;
                                        cout << endl;
                                        BGPUpdate* bgpUpdate = dynamic_cast<BGPUpdate*>(bgpMessage.get());
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
                                        BGPUpdate* bgpUpdate = dynamic_cast<BGPUpdate*>(bgpMessage.get());
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
                            MRTBgp4MPMessagePtr bgp4MPmsg = dynamic_pointer_cast<MRTBgp4MPMessage>( msg );
                            BGPMessagePtr bgpMessage = bgp4MPmsg->getPayload();
                            if( bgpMessage.get() == NULL ) { break; }
                            switch(bgpMessage->Type())
                            {
                                case BGPCommonHeader::UPDATE:
                                {
                                    if ((unFlags & PRINT_COMPACT) == 0)
                                    {
                                        cout << "/" << "Update" ;
                                        cout << endl;
                                        BGPUpdate* bgpUpdate = dynamic_cast<BGPUpdate*>(bgpMessage.get());
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
                                        BGPUpdate* bgpUpdate = dynamic_cast<BGPUpdate*>(bgpMessage.get());
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
                            MRTBgp4MPStateChangePtr bgp4MPmsg = dynamic_pointer_cast<MRTBgp4MPStateChange>( msg );
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
                            MRTBgp4MPStateChangePtr bgp4MPmsg = dynamic_pointer_cast<MRTBgp4MPStateChange>( msg );
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
        }
	}
	catch( EOFException e )
	{
		LOG4CXX_DEBUG( _log, "EOF" );
		; //do nothing
	}
//	catch( io::gzip_error e )
//	catch( io::bzip2_error e )
	catch( std::istream::failure e )
	{
		cerr << "ERROR: " << e.what() << endl;
		exit( 10 );
	}
	catch( BGPParserError &e )
	{
		cerr << "ERROR in MRT #"<<mrt_id<<": " << e.what() << endl;
		exit( 10 );
	}
//	catch( const string &e )
//	{
//		cerr << "ERROR: " << e << endl;
//		exit( 10 );
//	}
//	catch( BGPParserError &e )
//	{
//		cerr << "ERORR: " << e.what( ) << endl;
//		exit( 10 );
//	}
//	catch( ... )
//	{
//		cerr << "Unknown exception" << endl;
//		exit( 10 );
//	}


	LOG4CXX_INFO( _log, "Parsing ended" );
	return 0;
}

// vim: sw=4 ts=4 sts=4 expandtab
