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
 *  File: xmlinternal.h
 *  Authors: He Yan
 *           Pei-chun (Payne) Cheng
 *  Date: Jun 22, 2008 
 */

#ifndef XMLINTERNAL_H_
#define XMLINTERNAL_H_

/* needed for XML processing */
#include <libxml/parser.h>
#include <libxml/tree.h>

/* constants */
#define XML_BUFFER_LEN      10240000  /* 10M  Bytes - max size of XML buffer, for the whole XML message */
#define XML_TEMP_BUFFER_LEN 512       /* 512 Bytes  - max size of XML temporary buffer, for a single ascii word, like '128.110.1.1' or '7013' */

/*----------------------------------------------------------------------------------------
 * Purpose: special string concatenation routines that work in linear time 
 * input:   dst - pointer to the destination in the buffer
 *          max - pointer to the end of the buffer
 *          src - pointer to the source
 * Output: the position of pointer in the buffer after concatenation
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
char * 
fcat ( char *dst, char *max, char *src );

/*----------------------------------------------------------------------------------------
 * Purpose: special string concatenation routines with 3 sources
 * input:   dst - pointer to the destination in the buffer
 *          max - pointer to the end of the buffer
           src1 - pointer to the source1
           src2 - pointer to the source2
           src3 - pointer to the source3
 * Output: the position of pointer in the buffer after concatenation
 * He Yan @ Jun 22, 2008
 * -------------------------------------------------------------------------------------*/
char *
fcat3 (char *dst, char *max, char *src1, char *src2, char *src3 );

/*----------------------------------------------------------------------------------------
 * Purpose: replace the first occurence of a particular substring in one string
 * input:   str - pointer to the string
 *         orig - pointer to substring to be replaced
 *          rep - pointer to the new substring
 * Output: the pointer to the string
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
char *
replace_str(char *str, char *orig, char *rep);

/*----------------------------------------------------------------------------------------
 * Purpose: Get the AFI for a particular address string
 * input:   addr - pointer to the address string
 * Output:  the AFI number for the address, currently support 1:IPv4 and 2:IPv6)
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
int 
get_afi(char* addr);

/*----------------------------------------------------------------------------------------
 * Purpose: Basic utiliy functions that add a property (attribute) to an existing xml node
 * input:   node  - pointer to the existing xml node
 *          name  - the attribute name
 *          value - the attribute value (with different data types)
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
/* Integer */ xmlNodePtr xmlNewPropInt   (xmlNodePtr node, char *name, int   value);   
/* Float   */ xmlNodePtr xmlNewPropFloat (xmlNodePtr node, char *name, float value);  
/* String  */ xmlNodePtr xmlNewPropString(xmlNodePtr node, char *name, char *value);
/* AFI     */ xmlNodePtr xmlNewPropAFI   (xmlNodePtr node,             int   value);
/* SAFI    */ xmlNodePtr xmlNewPropSAFI  (xmlNodePtr node,             int   value);

/*----------------------------------------------------------------------------------------
 * Purpose: Basic utility functions that generate a plain xml node, for example: <mytag>myvalue</mytag>
 * input:   tag   - the node name
 *          value - the node value (with different data types)
 * Output:  the xml node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
/* Integer */ xmlNodePtr xmlNewNodeInt         (char *tag, int       value);
/* u_int   */ xmlNodePtr xmlNewNodeUnsignedInt (char *tag, u_int32_t value);
/* Float   */ xmlNodePtr xmlNewNodeFloat       (char *tag, float     value);
/* String  */ xmlNodePtr xmlNewNodeString      (char *tag, char     *value);
/* Time    */ xmlNodePtr xmlNewNodeGmtTime     (char *tag, time_t    value);
/* Octet   */ xmlNodePtr xmlNewNodeOctets      (char *tag, u_char   *value, int len); /* len: octet length */
/* IP      */ xmlNodePtr xmlNewNodeIP          (char *tag, u_int32_t value);
/* AFI     */ xmlNodePtr xmlNewNodeAFI         (char *tag, int       value);
/* SAFI    */ xmlNodePtr xmlNewNodeSAFI        (char *tag, int       value);

/*----------------------------------------------------------------------------------------
 * Purpose: Basic utility functions that generate a plain xml node, 
 *          and attach it as a child to an existing xml node
 * input:   parent_node - the parent xml node
 *          tag   - the child node name
 *          value - the child node value (for different data types)
 * Output:  the newly created child node
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
/* Integer */ xmlNodePtr xmlNewChildInt         (xmlNodePtr parent_node, char *tag, int       value);
/* u_int   */ xmlNodePtr xmlNewChildUnsignedInt (xmlNodePtr parent_node, char *tag, u_int32_t value);
/* Float   */ xmlNodePtr xmlNewChildFloat       (xmlNodePtr parent_node, char *tag, float     value);
/* String  */ xmlNodePtr xmlNewChildString      (xmlNodePtr parent_node, char *tag, char     *value);
/* IP      */ xmlNodePtr xmlNewChildIP          (xmlNodePtr parent_node, char *tag, u_int32_t value);
/* Time    */ xmlNodePtr xmlNewChildGmtTime     (xmlNodePtr parent_node, char *tag, time_t    value);
/* Octet   */ xmlNodePtr xmlNewChildOctets      (xmlNodePtr parent_node, char *tag, u_char   *value, int len); /* len: octet length */
/* AFI     */ xmlNodePtr xmlNewChildAFI         (xmlNodePtr parent_node, char *tag, int       value);
/* SAFI    */ xmlNodePtr xmlNewChildSAFI        (xmlNodePtr parent_node, char *tag, int       value);

/*----------------------------------------------------------------------------------------
 * Purpose: print the xml node
 * input:   node - pointer to the xml node
 * Output:  length of printed string
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/ 
int
printNode(xmlNodePtr node);

#endif /*XMLINTERNAL_H_*/

/* vim: sw=4 ts=4 sts=4 expandtab
 */
