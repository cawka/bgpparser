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

#include <bgpparser.h>

#include "AsciiVisitor.h"
#include "IPTypeDiscoveryVisitor.h"

#ifdef BGPPARSER_HAVE_LIBLOG4CXX
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

#ifdef BGPPARSER_HAVE_LIBLOG4CXX
static LoggerPtr _log = Logger::getLogger("bgpparser");
#endif

int
main(int argc, char** argv)
{
  po::variables_map CONFIG;

  po::options_description opts("General options");
  opts.add_options()("help", "Print this help message")
#ifdef BGPPARSER_HAVE_LIBLOG4CXX
    ("log", po::value<string>(), "log4cxx configuration file")
#endif
      ("file", po::value<string>(), "MRT file or - to input from stdin")(
        "format", po::value<string>(),
        "Compression format: txt, z, gz, or bz2 (will be overridden by the actual extension of the "
        "file)")("ipv6", "Output only IPv6-related records")("ipv4",
                                                             "Output only IPv4-related records");

  po::positional_options_description p;
  p.add("file", -1);

  try {
    po::store(po::command_line_parser(argc, argv).options(opts).positional(p).run(), CONFIG);
  }
  catch (const po::error& error) {
    cerr << "ERROR: " << error.what() << endl << endl << opts << endl;

    exit(2);
  }

  if (CONFIG.count("help") > 0) {
    cout << opts << endl;
    exit(0);
  }

#ifdef BGPPARSER_HAVE_LIBLOG4CXX
  // configure Logger
  if (CONFIG.count("log") > 0)
    PropertyConfigurator::configureAndWatch(CONFIG["log"].as<string>());
  else if (fs::exists("log4cxx.properties"))
    PropertyConfigurator::configureAndWatch("log4cxx.properties");
  else {
    //		PatternLayoutPtr   layout   ( new PatternLayout("%d{HH:mm:ss} %p %c{1} - %m%n") );
    //		ConsoleAppenderPtr appender ( new ConsoleAppender( layout ) );

    //		BasicConfigurator::configure( appender );
    Logger::getRootLogger()->setLevel(log4cxx::Level::getOff());
  }
#endif

  if (CONFIG.count("file") == 0) {
    cerr << "ERROR: Input file should be specified" << endl << endl << opts << endl;
    exit(1);
  }

  ////////////////////////////////////////////////////////////////////////////
  string format = CONFIG.count("format") > 0 ? CONFIG["format"].as<string>() : "";
  ////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////
  string filename = CONFIG["file"].as<string>();
  smatch m;
  if (regex_match(filename, m, regex("^.*\\.(gz|bz2)$")))
    format = m[1];
  ////////////////////////////////////////////////////////////////////////////

  filtering_stream<input> in;

  if (format == "gz") {
    LOG4CXX_DEBUG(_log, "Input file has GZIP format");
    in.push(gzip_decompressor());
  }
  else if (format == "bz2") {
    LOG4CXX_DEBUG(_log, "Input file has BZIP2 format");
    in.push(bzip2_decompressor());
  }

  ifstream input_file(filename.c_str(), ios_base::in | ios_base::binary);
  if (!input_file.is_open()) {
    cerr << "ERROR: "
         << "cannot open file [" << filename << "] for reading" << endl << endl;
    exit(3);
  }

  bool ipv6 = false;
  bool ipv4 = false;

  if (CONFIG.count("ipv4") > 0)
    ipv4 = true;
  if (CONFIG.count("ipv6") > 0)
    ipv6 = true;
  if (!CONFIG.count("ipv4") && !CONFIG.count("ipv6")) {
    ipv4 = true;
    ipv6 = true;
  }

  if (filename == "-")
    in.push(cin);
  else
    in.push(input_file);

  AsciiVisitor ascii;
  IPTypeDiscoveryVisitor iptype;
  MRTTblDumpV2PeerIndexTblPtr tbldumpv2_indextbl;

  int mrt_id = 0;
  int count_error = 0;
  try {
    while (in.peek() != -1) {
      mrt_id++;

      try {
        MRTMessagePtr msg = MRTCommonHeader::newMessage(in, tbldumpv2_indextbl);
        bool canprint = true;
        if (!ipv4 || !ipv6) {
          mrt_type type = any_cast<mrt_type>(msg->accept(iptype));
          if (ipv4 && type != TYPE_IPv4)
            canprint = false;
          if (ipv6 && type != TYPE_IPv6)
            canprint = false;
        }

        if (canprint)
          msg->accept(ascii);
      }
      catch (MRTException e) {
        LOG4CXX_ERROR(_log, e.what());
        count_error++;
      }
      catch (BGPTextError e) {
        LOG4CXX_ERROR(_log, e.what());
        count_error++;
      }
      catch (BGPError e) {
        // information should be already logged, if the logger for bgpparser is enabled
        count_error++;
      }
    }
  }
  catch (EOFException e) {
    LOG4CXX_DEBUG(_log, "EOF");
    ; // do nothing
  }
  //	catch( gzip_error e )
  //	catch( bzip2_error e )
  catch (std::istream::failure e) {
    cerr << "ERROR: " << e.what() << endl;
    exit(10);
  }
  catch (BGPParserError& e) {
    cerr << "ERROR in MRT #" << mrt_id << ": " << e.what() << endl;
    exit(10);
  }

  LOG4CXX_INFO(_log, "Parsing ended");
  return 0;
}

// vim: sw=4 ts=4 sts=4 expandtab
