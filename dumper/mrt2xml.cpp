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

// mrt2xml.cpp : Conver mrt to xml output
// Author: Pei-chun Cheng

#include <bgpparser.h>

#define _USE_32BIT_TIME_T
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>
#include <getopt.h>
#include <map>

/* MRT classes */
#include "MRTCommonHeader.h"
#include "MRTBgp4MPStateChange.h"
#include "MRTBgp4MPMessage.h"
#include "MRTTblDump.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "Exceptions.h"

/* Dumper class */
#include "Dumper.h"
#include "MRTTblDumpV1Dumper.h"
#include "MRTTblDumpV2Dumper.h"
#include "MRTBgp4MPMessageDumper.h"
#include "MRTBgp4MPStateChangeDumper.h"

//#define MAX_MRT_TYPE_NUM    49
//#define MAX_MRT_SUBTYPE_NUM  6

/* Constants */
//#define MRT_COMMON_HDR_LEN  12

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

static LoggerPtr _log=Logger::getLogger( "bgpdump" );

//int dump_type = 0;   // 1: TABLE_DUMP1
//                     // 2: TABLE_DUMP2
//                     // 0: BGP4MP

int procBgpMessage(xmlNodePtr node, int flag_format, int flag_newline, int flag_bgpdump)
{
    Dumper::printXmlNode(node, flag_format, flag_newline);
    return 0;
}

int procBgpMessages(xmlNodePtr node, int flag_format, int flag_newline, int flag_bgpdump)
{
    for (xmlNodePtr child_node=node->children; child_node!= NULL; child_node=child_node->next)
    {
        procBgpMessage(child_node, flag_format, flag_newline, flag_bgpdump);
    }
    return 1;
}

int procMsg( const DumperPtr &dumper, int flag_format, int flag_newline, int flag_bgpdump)
{
    if (flag_bgpdump)
    {
        dumper->printAscii(flag_format, flag_newline);
    }
    else
    {
        xmlNodePtr root_node = dumper->genXml();

        if (!root_node->name) return 0;
        if      ((strcmp((char*)root_node->name, "BGP_MESSAGES") == 0)) procBgpMessages(root_node, flag_format, flag_newline, flag_bgpdump);
        else if ((strcmp((char*)root_node->name, "BGP_MESSAGE" ) == 0)) procBgpMessage(root_node,  flag_format, flag_newline, flag_bgpdump);

        xmlUnlinkNode(root_node);
        xmlFreeNode(root_node);
    }
    return 0;
}

void print_usage()
{
    cout << "Usage: \n\
1. Convert MRT files to XML format \n\
   bgpparser [-f] [-n] [-m] [-h] mrt_file\n\
    -f format/indent the xml bgp message (default: YES),\n\
    -n insert a newline character between each xml bgp message (default: YES),\n\
    -m compact format, equivalent to disable -f and enable -n option (default: NO),\n\
    -r include a root element (default: NO),\n\
    -h dispaly this message\n\
    \n\
2. Convert MRT files to bgpdump ascii short format \n\
   bgpparser -B mrt_file\n\
    -B output bgpdump compatible ascii short format,\n\
	   \n";
}

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char** argv)
{
    /* -------------------------- */
    /* Get options                */
    /* -------------------------- */
    const char* const short_options = "hrmnfBt:";
    const struct option long_options[] = {
        { "help",    0, NULL, 'h' },
        { "compact", 0, NULL, 'm' },
        { "format" , 0, NULL, 'f' },
        { "newline", 0, NULL, 'n' },
        { "root",    0, NULL, 'r' },
        { "bgpdump", 0, NULL, 'B' },
        { NULL,      0, NULL, 0   }
    };
    int next_options;
    int flag_compact = 0;
    int flag_format  = 1;
    int flag_newline = 1;
    int flag_root    = 0;
    int flag_bgpdump = 0;

    do {
        next_options=getopt_long(argc,argv,short_options,long_options,NULL);
        switch (next_options){
            case 'h': 
                      print_usage();
                      exit(0);
            case 'm': flag_compact = 1; flag_format = 0; flag_newline = 1; break;
            case 'f': flag_format  = 1; break;
            case 'n': flag_newline = 1; break;
            case 'r': flag_root    = 1; break;
            case 'B': flag_bgpdump = 1; break;
        }
    } while (next_options != -1);

    /* -------------------------- */
    /* Initialize Logger          */
    /* -------------------------- */
	// configure Logger
	if( fs::exists("log4cxx.properties") )
		PropertyConfigurator::configureAndWatch( "log4cxx.properties" );
	else
	{
//		PatternLayoutPtr   layout   ( new PatternLayout("%d{HH:mm:ss} %-7p %-25c{1} %m%n") );
//		ConsoleAppenderPtr appender ( new ConsoleAppender( layout ) );
//
//		BasicConfigurator::configure( appender );
		Logger::getRootLogger()->setLevel( log4cxx::Level::getOff() );
	}

    /* -------------------------- */
    /* File                       */
    /* -------------------------- */
    char* fileName;
    if(optind < argc){
        fileName = argv[optind];
    }
    else if (optind == argc)
    {
        print_usage();
        exit(0);
    }

	string format="";
	smatch m;
	if( regex_match(string(fileName), m, regex("^.*\\.(gz|bz2)$")) ) format=m[1];
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

	ifstream input_file( fileName, ios_base::in | ios_base::binary );
	if( !input_file.is_open() )
	{
		cerr << "ERROR: " << "cannot open file [" <<  fileName  << "] for reading" << endl
			 << endl;
		exit( 3 );
	}

	if( string(fileName)=="-" )
		in.push( cin );
	else
		in.push( input_file );

    LOG4CXX_INFO( _log,  "Parsing file [" << fileName << "]" );



    /* -------------------------- */
    /* Process file               */
    /* -------------------------- */
	if (flag_root) cout << "<BGP_MESSAGES>" << endl;

	unsigned int unTotalBytesRead = 0;

	shared_ptr<MRTTblDumpV2Dumper> mrt_tblv2_dumper( new MRTTblDumpV2Dumper() );
	while( in.peek()!=-1 )
	{
		try
		{
			MRTMessagePtr msg = MRTCommonHeader::newMessage( in );

			switch( msg->getType() )
			{
				// -------------------------- //
				// TABLE_DUMP                 //
				// -------------------------- //
				case TABLE_DUMP:
				{
//					dump_type = 1;
					switch(msg->getSubType())
					{
						case AFI_IPv4:
						case AFI_IPv6:
						{
							// Prepare BGP4MP Message pointer ./
							MRTTblDumpPtr tblDump = dynamic_pointer_cast<MRTTblDump>( msg );

							// Dumper for BGP Message ./
							MRTTblDumpV1DumperPtr mrt_tblv1_dumper( new MRTTblDumpV1Dumper() );
							mrt_tblv1_dumper->setTblDumpMsg( tblDump );
							procMsg( mrt_tblv1_dumper, flag_format, flag_newline, flag_bgpdump );
						}
						break;

						default:
							cerr << "Undefined Subtype (TABLE_DUMP_V1) :" <<  msg->getSubType() << endl;
							break;
					}
				}
				break;

				// -------------------------- //
				// TABLE_DUMP_V2              //
				// -------------------------- //
				case TABLE_DUMP_V2:
				{
//					dump_type = 2;
					// Dumper for Table Dumper v2 //
					switch (msg->getSubType())
					{
						case RIB_IPV4_UNICAST:
						case RIB_IPV4_MULTICAST:
						case RIB_IPV6_UNICAST:
						case RIB_IPV6_MULTICAST:
						{
							MRTTblDumpV2RibHeaderPtr tblDumpHeader =
                            		dynamic_pointer_cast<MRTTblDumpV2RibHeader>( msg );

							mrt_tblv2_dumper->setTblDumpMsg( tblDumpHeader );
							procMsg( mrt_tblv2_dumper, flag_format, flag_newline, flag_bgpdump );
						}
						break;

						case PEER_INDEX_TABLE:
						{
							MRTTblDumpV2PeerIndexTblPtr peerIndexTbl
								= dynamic_pointer_cast<MRTTblDumpV2PeerIndexTbl>( msg );

							mrt_tblv2_dumper->setPeerIndexTbl( peerIndexTbl );
						}
						break;

						default:
							cerr << "Undefined Subtype (TABLE_DUMP_V2) :" <<  msg->getSubType() << endl;
						break;
					}
					break;
				}
				break;

				// -------------------------- //
				// BGP4MP                     //
				// -------------------------- //
				case BGP4MP:
				{
//					dump_type = 0;
					switch (msg->getSubType())
					{
						case BGP4MP_MESSAGE:
						case BGP4MP_MESSAGE_AS4:
						{
							// Prepare BGP4MP Message pointer ./
							MRTBgp4MPMessagePtr bgp4MPmsg = dynamic_pointer_cast<MRTBgp4MPMessage>( msg );

							// Dumper for BGP Message ./
							MRTBgp4MPMessageDumperPtr mrt_bgp4mp_msg_dumper = MRTBgp4MPMessageDumper::newDumper( bgp4MPmsg );
							procMsg( mrt_bgp4mp_msg_dumper, flag_format, flag_newline, flag_bgpdump );
						}
						break;

						case BGP4MP_STATE_CHANGE:
						case BGP4MP_STATE_CHANGE_AS4:
						{
							// Prepare BGP4MP State Change pointer //
							MRTBgp4MPStateChangePtr bgp4MPmsg = dynamic_pointer_cast<MRTBgp4MPStateChange>( msg );

							// Dumper for BGP State Change //
							MRTBgp4MPStateChangeDumperPtr mrt_bgp4mp_sc_dumper = MRTBgp4MPStateChangeDumper::newDumper( bgp4MPmsg );
							procMsg( mrt_bgp4mp_sc_dumper, flag_format, flag_newline, flag_bgpdump );
						}
						break;

						default:
							cerr << "Undefined Subtype (BGP4MP) :" <<  msg->getSubType() << endl;
						break;
					}
				}
				break;

				default:
					cerr << "Undefined Type:" <<  msg->getType() << endl;
				break;
			}
		}
		catch( BGPParserError &e )
		{
			LOG4CXX_ERROR(_log, "ERROR: " << e.what() );
			// moving on to the next MRT entry
		}
	}

	if (flag_root) cout << "</BGP_MESSAGES>" << endl;

    LOG4CXX_INFO( _log, "parsing ends" );
    return 0;
}

// vim: sw=4 ts=4 sts=4 expandtab
