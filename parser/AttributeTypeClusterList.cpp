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

// Author: Jonathan Park
#include <bgpparser.h>

#include "AttributeTypeClusterList.h"
using namespace std;

#include <boost/iostreams/read.hpp>
namespace io = boost::iostreams;

log4cxx::LoggerPtr AttributeTypeClusterList::Logger = log4cxx::Logger::getLogger( "bgpparser.AttributeTypeClusterList" );

AttributeTypeClusterList::AttributeTypeClusterList( AttributeType &header, std::istream &input )
					: AttributeType(header) {
	LOG4CXX_DEBUG(Logger,"");

	uint32_t left=length;
	while( left > 0 )
	{
		uint32_t cluster_id;
		io::read( input, reinterpret_cast<char*>(&cluster_id), sizeof(cluster_id) );
		left -= sizeof(cluster_id);
		cluster_list.push_back(cluster_id);
	}
}

AttributeTypeClusterList::~AttributeTypeClusterList(void) {
}

void AttributeTypeClusterList::printMe() { 
	IPAddress addr;
	list<uint32_t>::iterator it;
	cout << "CLUSTER-LIST:";
	for (it = cluster_list.begin(); it != cluster_list.end(); it++) {
		memcpy(&addr,&(*it),sizeof(uint32_t));
		cout << " ";
		PRINT_IP_ADDR(addr.ipv4);
	}
}

void AttributeTypeClusterList::printMeCompact() { 
	IPAddress addr;
	list<uint32_t>::iterator it;
	cout << "CLUSTER-LIST:";
	for (it = cluster_list.begin(); it != cluster_list.end(); it++) {
		memcpy(&addr,&(*it),sizeof(uint32_t));
		cout << " ";
		PRINT_IP_ADDR(addr.ipv4);
	}
}
