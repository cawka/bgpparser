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

// BGP Message Dumper
#ifndef __DUMPER_H_
#define __DUMPER_H_

#include <stdint.h>
#include <string>
#include <libxml/tree.h>

using namespace std;

class Dumper
{
public:
	Dumper();
	virtual ~Dumper();
	
	/* XML output */
	static int printXmlNode(xmlNodePtr node, int format, int newline);
	virtual void printXml(bool format, bool newline) 
	{ 
	        xmlNodePtr node = genXml();	
		printXmlNode(node, format, newline);
		xmlUnlinkNode(node);
		xmlFreeNode(node);
	};
	virtual xmlNodePtr genXml() { return NULL; };

	/* ASCII output */
	static int printAsciiNode(string node, int format, int newline);
	virtual void printAscii(bool format, bool newline) 
	{ 
	        string node = genAscii();	
		printAsciiNode(node, format, newline);
	};
	virtual string genAscii() { return ""; };

protected:

};

#endif /* __DUMPER_H_ */
