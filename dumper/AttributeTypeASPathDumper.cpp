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

#include <string>
#include <libxml/tree.h>
#include "AttributeTypeDumper.h"
#include "AttributeTypeASPath.h"

extern "C" {
    #include "xmlinternal.h"
}

AttributeTypeASPathDumper::AttributeTypeASPathDumper(AttributeType* attr)
: AttributeTypeDumper(attr)
{}

AttributeTypeASPathDumper::~AttributeTypeASPathDumper()
{}

xmlNodePtr AttributeTypeASPathDumper::genXml()
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST (char *)type_str.c_str()); /* AS_PATH or AS4_PATH */

    list<AttributeTypeASPathSegment>* pathSegments = NULL;
	switch(type_code)
	{
		case AttributeType::AS_PATH:
		{
            pathSegments = ((AttributeTypeASPath *)attr_type)->getPathSegmentsComplete();
			break;
		}
		case AttributeType::NEW_AS_PATH:
		{
            pathSegments = ((AttributeTypeASPath *)attr_type)->getPathSegments();
			break;
		}
    }

	if (pathSegments != NULL)
	{	
		list<AttributeTypeASPathSegment>::iterator it;
		for (it = pathSegments->begin(); it != pathSegments->end(); it++)
		{
            AttributeTypeASPathSegment* as_seg = &(*it);

            xmlNodePtr as_seg_node = xmlNewNode(NULL, BAD_CAST "AS_SEG");
            xmlAddChild(node, as_seg_node);

            char *type = NULL;
            uint8_t pathSegmentType = as_seg->getPathSegmentType();
            switch (pathSegmentType)
            {
                case AttributeTypeASPathSegment::AS_SEQUENCE:          type = "AS_SEQUENCE";         break;
                case AttributeTypeASPathSegment::AS_SET:               type = "AS_SET";              break;
                //[TO_DO] conf_set / conf_sequence
                case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:   type = "AS_CONFED_SEQUENCE";  break;
                case AttributeTypeASPathSegment::AS_CONFED_SET:        type = "AS_CONFED_SET";       break;
                default:                                               type = "OTHER";               break;
            }
            xmlNewPropString(as_seg_node, "type",   type);
            xmlNewPropInt(as_seg_node,    "length", as_seg->getPathSegmentLength());

            list<uint32_t> *pathSegmentValue =  as_seg->getPathSegmentValue();
            if (pathSegmentValue != NULL)
            {
                list<uint32_t>::iterator it;
                for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++)
                {
                    xmlNewChildInt(as_seg_node, "AS", *it);
                }
            }
		}
	}

    return node;
}

string AttributeTypeASPathDumper::genAscii()
{
    string node = "";

    list<AttributeTypeASPathSegment>* pathSegments = NULL;
	switch(type_code)
	{
		case AttributeType::AS_PATH:
		{
            pathSegments = ((AttributeTypeASPath *)attr_type)->getPathSegmentsComplete();
			break;
		}
		case AttributeType::NEW_AS_PATH:
		{
            pathSegments = ((AttributeTypeASPath *)attr_type)->getPathSegments();
			break;
		}
    }

	if (pathSegments != NULL)
	{	
        static string head = "";
        static string tail = "";
        static string sep  = " ";
        static char buffer[16];

		list<AttributeTypeASPathSegment>::iterator it;
        bool first_seg = true;
		for (it = pathSegments->begin(); it != pathSegments->end(); it++)
		{
            if (first_seg)
            {
                first_seg = false;
            }
            else
            {
                node += " ";
            }

            AttributeTypeASPathSegment* as_seg = &(*it);

            uint8_t pathSegmentType = as_seg->getPathSegmentType();
            switch (pathSegmentType)
            {
                case AttributeTypeASPathSegment::AS_SEQUENCE:          head = "";   tail="";   sep=" ";  break;
                case AttributeTypeASPathSegment::AS_SET:               head = "{";  tail="}",  sep=",";  break;
                //[TO_DO] conf_set / conf_sequence
                case AttributeTypeASPathSegment::AS_CONFED_SEQUENCE:   head = "(";  tail=")";  sep=" ";  break; //[TODO]
                case AttributeTypeASPathSegment::AS_CONFED_SET:        head = "({"; tail="})"; sep=",";  break; //[TODO]
                default:                                               head = "";   tail="";   sep=" ";  break;
            }

            node += head;
            list<uint32_t> *pathSegmentValue =  as_seg->getPathSegmentValue();
            bool first = true;
            if (pathSegmentValue != NULL)
            {
                list<uint32_t>::iterator it;
                for (it = pathSegmentValue->begin(); it != pathSegmentValue->end(); it++)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        node += sep;
                    }

                    buffer[0] = '\0';
                    sprintf(buffer, "%u", *it);
                    node += buffer;
                }
            }
            node += tail;
		}
	}

    return node;
}

// vim: sw=4 ts=4 sts=4 expandtab
