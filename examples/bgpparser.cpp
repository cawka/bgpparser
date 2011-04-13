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
#include <bgpparser.h>

#include "SimplePrinter.h"

#ifdef LOG4CXX
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
#endif

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

using namespace std;
using namespace boost;
using namespace boost::iostreams;

bool PrintCompact=false;

#ifdef LOG4CXX
static LoggerPtr _log=Logger::getLogger( "simple" );
#endif

int main(int argc, char** argv)
{
    po::variables_map CONFIG;

	po::options_description opts( "General options" );
	opts.add_options()
		( "help", "Print this help message" )
#ifdef LOG4CXX
		( "log",   po::value<string>(), "log4cxx configuration file" )
#endif
		( "file",  po::value<string>(),
				  "MRT file or - to input from stdin" )
//		( "compact", "Compact view" )
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

#ifdef LOG4CXX
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
#endif

	if( CONFIG.count("file")==0 )
	{
		cerr << "ERROR: Input file should be specified" << endl
			 << endl
			 << opts << endl;
		exit( 1 );
	}

	if( CONFIG.count("compact")>0 ) PrintCompact=true;

	////////////////////////////////////////////////////////////////////////////
	string format=CONFIG.count("format")>0 ? CONFIG["format"].as<string>( ) : "";
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	string filename=CONFIG["file"].as<string>();
	smatch m;
	if( regex_match(filename, m, regex("^.*\\.(gz|bz2)$")) ) format=m[1];
	////////////////////////////////////////////////////////////////////////////

	filtering_stream<input> in;

	if( format=="gz" )
	{
		LOG4CXX_DEBUG( _log, "Input file has GZIP format" );
		in.push( gzip_decompressor() );
	}
	else if( format=="bz2" )
	{
		LOG4CXX_DEBUG( _log, "Input file has BZIP2 format" );
		in.push( bzip2_decompressor() );
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

	//printf("flags = 0x%08x\n", unFlags);
	LOG4CXX_INFO( _log, "Parsing file [" << filename << "]" );

    int i=1;
	int mrt_id=0;
	SimplePrinter printer;
	int count_error=0;
	MRTTblDumpV2PeerIndexTblPtr tbldumpv2_indextbl;

	try
	{
		while( in.peek()!=-1 )
		{
			mrt_id++;

			try
			{
				MRTMessagePtr msg=MRTCommonHeader::newMessage( in, tbldumpv2_indextbl );

				msg->accept( printer );
			}
            catch( MRTException e )
            {
                LOG4CXX_ERROR( _log, e.what() );
                count_error++;
            }
            catch( BGPTextError e )
            {
                LOG4CXX_ERROR( _log, e.what() );
                count_error++;
            }
            catch( BGPError e )
            {
                //information should be already logged, if the logger for bgpparser is enabled
                count_error++;
            }
		}
	}
	catch( EOFException e )
	{
		LOG4CXX_DEBUG( _log, "EOF" );
		; //do nothing
	}
//	catch( gzip_error e )
//	catch( bzip2_error e )
	catch( std::istream::failure e )
	{
		cerr << "ERROR: " << e.what() << endl;
		exit( 10 );
	}

	LOG4CXX_INFO( _log, "Parsing ended" );
	return 0;
}

// vim: sw=4 ts=4 sts=4 expandtab
