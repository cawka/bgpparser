/*
 *  Copyright (c) 2008 Colorado State University
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom
 *  the Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 *  File:    xmlinternal.c
 *  Authors: Pei-chun Cheng 
 *  Date:    Dec 20, 2008
 *
 */

/*
 * The purpose of xmlinternal.c is to provide some basic building blocks for xml conversion
 */

/* needed for u_char data types */
#include <sys/types.h>

/* needed for time related functions */
#include <time.h>

/* needed for function inet_ntoa */
#include <arpa/inet.h>
#include <netinet/in.h>

/* needed for string and math operation */
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>

/* needed for xml operation tring and math operation */
#include <libxml/parser.h>
#include <libxml/tree.h>

/* needed for interanl functions */
#include "xmlinternal.h"


//#define DEBUG

/*----------------------------------------------------------------------------------------
 * Purpose: special string concatenation routines that work in linear time 
 * input:   dst - pointer to the destination in the buffer
 *          max - pointer to the end of the buffer
 *          src - pointer to the source
 * Output: the position of pointer in the buffer after concatenation
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/ 
char * 
fcat ( char *dst, char *max, char *src )
{
    while ( dst < max && *dst)
        dst++;
    while ( dst < max && (*dst++ = *src++) )
        ;
    return --dst;
}

/*----------------------------------------------------------------------------------------
 * Purpose: special string concatenation routines with 3 sources
 * input:   dst - pointer to the destination in the buffer
 *          max - pointer to the end of the buffer
 *         src1 - pointer to the source1
 *         src2 - pointer to the source2
 *         src3 - pointer to the source3
 * Output: the position of pointer in the buffer after concatenation
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
char *
fcat3 (char *dst, char *max, char *src1, char *src2, char *src3 )
{
    char *dst3 = dst;
    dst3 = fcat( dst3, max, src1 );
    dst3 = fcat( dst3, max, src2 );
    dst3 = fcat( dst3, max, src3 );
    
    return dst3;
}

/*----------------------------------------------------------------------------------------
 * Purpose: replace the first occurence of some particular substring in one string
 * input:   str - pointer to the string
 *         orig - pointer to substring to be replaced
 *          rep - pointer to the new substring
 * Output: the position of pointer in the buffer after concatenation
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
char *replace_str(char *str, char *orig, char *rep)
{
    char *p;
    if(!(p = strstr(str, orig)))  // Is 'orig' even in 'str'?
    return str;
    memmove(p, rep, strlen(rep));
    return str;
}

/*----------------------------------------------------------------------------------------
 * Purpose: Get the AFI for one address string
 * input:   addr - pointer to the address
 * Output:  the AFI number for the address, currently support IPv4 and IPv6
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
/*
int get_afi(char* addr)
{
    int afi = 0;
    struct in6_addr addr_s;
    int rc = inet_pton(AF_INET, addr, &addr_s);
    if (rc == 1)    // if it is a valid IPv4 address 
    {
        afi=1;
    }
    else
    {
        rc = inet_pton(AF_INET6, addr, &addr_s);
        if (rc == 1) // if it is a valid IPv6 address
        {
            afi=2;
        }
    }
    return afi;
}
*/

/*----------------------------------------------------------------------------------------
 * Purpose: print xml node
 * input:	node - pointer to the xml node
 * Output:  string length
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/ 
int
printNode(xmlNodePtr node)
{
    xmlBufferPtr buff = xmlBufferCreate();
    int len = xmlNodeDump(buff, NULL, node, 0, 1);
    printf("%s", (char *) buff->content);
    xmlBufferFree(buff);  /* free xml buffer       */
    return len;
}


/*----------------------------------------------------------------------------------------
 * Purpose: add an integer property (attribute) to an xml node
 * input:   node  - pointer to the xml node
 *          name  - name string
 *          value - an integer
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewPropInt(xmlNodePtr node, char *name, int value)
{
    static char str[XML_TEMP_BUFFER_LEN];
    snprintf(str, XML_TEMP_BUFFER_LEN, "%d", value);
    xmlNewProp(node, BAD_CAST name, BAD_CAST str);
    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: add an float property (attribute) to an xml node
 * input:   node  - pointer to the xml node
 *          name  - name string
 *          value - a float
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewPropFloat(xmlNodePtr node, char *name, float value)
{
    static char str[XML_TEMP_BUFFER_LEN];
    snprintf(str, XML_TEMP_BUFFER_LEN, "%f", value);
    xmlNewProp(node, BAD_CAST name, BAD_CAST str);
    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: add an string property (attribute) to an xml node
 * input:   node  - pointer to the xml node
 *          name  - name string
 *          value - a character string
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewPropString( xmlNodePtr node, char *name, char *value )
{
    xmlNewProp(node, BAD_CAST name, BAD_CAST value);
    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with a string value, for example: <mytag>myvalue</mytag>
 * input:   tag   - tag string
 *          value - a string
 * Output:  the string xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeString(char *tag, char *value)
{
    xmlNodePtr node = xmlNewNode(NULL, BAD_CAST tag);
    xmlNodePtr text = xmlNewText(BAD_CAST value);
    xmlAddChild(node, text);
    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: add a string child node to a xml node
 * input:   parent_node - parent xml node
 *          tag   - tag string for the child node
 *          value - the string value for the child node
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewChildString(xmlNodePtr parent_node, char *tag, char *value)
{
    return xmlAddChild(parent_node, xmlNewNodeString(tag, value));
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with an integer value, for example: <mytag>5</mytag>
 * input:   tag   - tag string
 *          value - an integer
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeInt(char *tag, int value)
{
    static char str[XML_TEMP_BUFFER_LEN];
    snprintf(str, XML_TEMP_BUFFER_LEN, "%d", value);
    return xmlNewNodeString(tag, str);
}

/*----------------------------------------------------------------------------------------
 * Purpose: add an integer child node to a xml node
 * input:   parent_node - parent xml node
 *          tag   - tag string for the child node
 *          value - the integer value for the child node
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewChildInt(xmlNodePtr parent_node, char *tag, int value)
{
    return xmlAddChild(parent_node, xmlNewNodeInt(tag, value));
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with a unsigned integer value, for example: <mytag>5</mytag>
 * input:   tag   - tag string
 *          value - a unsigned integer
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeUnsignedInt(char *tag, u_int32_t value)
{
    static char str[XML_TEMP_BUFFER_LEN];
    snprintf( str, XML_TEMP_BUFFER_LEN, "%u", value);
    return xmlNewNodeString(tag, str);
}

/*----------------------------------------------------------------------------------------
 * Purpose: add a unsigned integer child node to a xml node
 * input:   parent_node - parent xml node
 *          tag   - tag string for the child node
 *          value - the unsigned integer value for the child node
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewChildUnsignedInt(xmlNodePtr parent_node, char *tag, u_int32_t value)
{
    return xmlAddChild(parent_node, xmlNewNodeUnsignedInt(tag, value));
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with an float value, for example: <mytag>5.0</mytag>
 * input:   tag   - tag string
 *          value - a float
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeFloat(char *tag, float value)
{
    static char str[XML_TEMP_BUFFER_LEN];
    snprintf(str, XML_TEMP_BUFFER_LEN, "%f", value);
    return xmlNewNodeString(tag, str);
}

/*----------------------------------------------------------------------------------------
 * Purpose: add an float child node to a xml node
 * input:   parent_node - parent xml node
 *          tag   - tag string for the child node
 *          value - the integer value for the child node
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewChildFloat(xmlNodePtr parent_node, char *tag, float value)
{
    return xmlAddChild(parent_node, xmlNewNodeFloat(tag, value));
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with a IPV4 address, for example: <mytag> 1.2.3.4 </mytag>
 * input:   tag   - tag string
 *          ip    - a unsigned integer for ipv4 address
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeIP(char *tag, u_int32_t ip)
{
    static struct in_addr in;
    in.s_addr = ip;
    char *buff = inet_ntoa( in );

    return xmlNewNodeString(tag, buff);
}

/*----------------------------------------------------------------------------------------
 * Purpose: add a ipv4 child node to a xml node
 * input:   parent_node - parent xml node
 *          tag   - tag string for the child node
 *          ip    - the unsigned integer ipv4 for the child node
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewChildIP(xmlNodePtr parent_node, char *tag, u_int32_t ip)
{
    return xmlAddChild(parent_node, xmlNewNodeIP(tag, ip));
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with GMT format time, 
 *          for example: <mytag> 2008-10-09T08:27:35Z </mytag>
 * input:   tag        - tag string
 *          timestamp  - time_t
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeGmtTime(char *tag, time_t timestamp )
{
    static char gmttime[XML_TEMP_BUFFER_LEN];
    strftime(gmttime, XML_TEMP_BUFFER_LEN, "%Y-%m-%dT%H:%M:%SZ", gmtime(&timestamp));
    return xmlNewNodeString(tag, gmttime);
}

/*----------------------------------------------------------------------------------------
 * Purpose: add a GMT time child node to a xml node
 * input:   parent_node - parent xml node
 *          tag    - tag string for the child node
 *          timestamp  - time_t
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
inline xmlNodePtr
xmlNewChildGmtTime(xmlNodePtr parent_node,  char *tag, time_t timestamp)
{
    return xmlAddChild(parent_node, xmlNewNodeGmtTime(tag, timestamp));
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with hexdecimal value, for example: <mytag>A1F0</mytag>
 * input:   tag    - tag string
 *          octets - binary octets
 *          int    - length  of octets
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeOctets(char *tag, u_char *octets, int len)
{
    xmlNodePtr node = NULL;
    static char tmpbuffs[XML_BUFFER_LEN];
    static char hexbuffs[XML_BUFFER_LEN];
    tmpbuffs[0] = '\0';
    hexbuffs[0] = '\0';

    /* Convert the binary string to hexdecimal ascii string */
    static char *hex[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" };
    int i;
    for ( i = 0; i < len; i++ )
    {
        tmpbuffs[2*i  ] = *hex[ octets[i] >> 4 ];
        tmpbuffs[2*i+1] = *hex[ octets[i] & 15 ];
    }
    strncpy(hexbuffs, tmpbuffs, len*2);
    hexbuffs[len*2] = '\0'; // terminate the string buffer

    node = xmlNewNodeString(tag, hexbuffs);
    xmlNewPropInt(node, "length", len); /* length attribute */

    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: add a octets child node to a xml node
 * input:   parent_node - parent xml node
 *          tag    - tag string for the child node
 *          octets - the octets value for the child node
 * Output:  the child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewChildOctets(xmlNodePtr parent_node, char *tag, u_char *octets, int len)
{
    return xmlAddChild(parent_node, xmlNewNodeOctets(tag, octets, len));
}

/*----------------------------------------------------------------------------------------
 * Purpose: add an afi property (attribute) to an xml node
 * input:   node  - pointer to the xml node
 *          afi   - an afi number
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewPropAFI(xmlNodePtr node, int afi)
{
    char *afi_str   = ""; /* null string */

    switch(afi)
    {
        case 1:  afi_str = "IPV4";  break;
        case 2:  afi_str = "IPV6";  break;
        default: afi_str = "OTHER"; break;
    }

    xmlNewPropString(node, "afi",       afi_str);  /* AFI string, ex: IPV4 (human   readable)*/
    xmlNewPropInt(node,    "afi_value", afi);      /* AFI value,  ex: 1    (machine readable)*/

    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: add a safi property (attribute) to an xml node
 * input:   node  - pointer to the xml node
 *          safi  - a  safi number
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewPropSAFI(xmlNodePtr node, int safi)
{
    char *safi_str   = ""; /* null string */

    switch(safi)
    {
        case 1:  safi_str = "UNICAST";   break;
        case 2:  safi_str = "MULTICAST"; break;
        case 4:  safi_str = "MPLS";      break;
        default: safi_str = "OTHER";     break;
    }

    xmlNewPropString(node,  "safi",       safi_str);  /* SAFI string, ex: UNICAST (human   readable)*/
    xmlNewPropInt(node,     "safi_value", safi);      /* SAFI value,  ex: 1       (machine readable)*/

    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with AFI value and text
 * input:   tag  - tag string
 *          afi  - afi value
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 *
 * Ex: <AFI value="1">IPV4</AFI>
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeAFI(char *tag, int afi)
{
    xmlNodePtr node = NULL;
    char *afi_str   = "";

    switch(afi)
    {
        case 1:  afi_str = "IPV4"; break;
        case 2:  afi_str = "IPV6"; break;
        default: afi_str = "OTHER"; break;
    }

    node = xmlNewNodeString(tag, afi_str); 
    xmlNewPropInt(node, "value", afi);

    return node;
}

/*----------------------------------------------------------------------------------------
 * Purpose: generate a xml node with SAFI value and text
 * input:   tag  - tag string
 *          safi - safi value
 * Output:  the new xml node
 * Pei-chun Cheng @ Dec 20, 2008
 *
 * Ex: <SAFI value="1">UNICAST</AFI>
 * -------------------------------------------------------------------------------------*/
xmlNodePtr
xmlNewNodeSAFI(char *tag, int safi)
{
    xmlNodePtr node = NULL;
    char *safi_str   = "";

    switch(safi)
    {
        case 1:  safi_str = "UNICAST"; break;
        case 2:  safi_str = "MULTICAST"; break;
        case 4:  safi_str = "MLPS"; break;
        default: safi_str = "OTHER"; break;
    }

    node = xmlNewNodeString(tag, safi_str); 
    xmlNewPropInt(node, "value", safi);

    return node;
}


/* vim: sw=4 ts=4 sts=4 expandtab
 */
