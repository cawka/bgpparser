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
 *  File:    xml.h
 *  Authors: He Yan
 *           Pei-chun (Payne) Cheng
 *  Date:    Jun 22, 2008 
 */

#ifndef XML_H_
#define XML_H_

/*--------------------------------------------------------------------------------------
 * Purpose: launch xml converter thread, called by main.c
 * Input:   none
 * Output:  none
 * He Yan @ July 22, 2008
 * -------------------------------------------------------------------------------------*/
void launchXMLThread();

/*----------------------------------------------------------------------------------------
 * Purpose: get the length of a XML message (from the attribute "length")
 * Input:   pointer to the XML message or partial XML message
 * Output:  length of the message
 * He Yan @ Jun 22, 2008
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/
int getXMLMessageLen(char *xmlMsg);

#endif /*XML_H_*/

/* vim: sw=4 ts=4 sts=4 expandtab
 */
