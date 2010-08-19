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
 *  File:    xmldata.h
 *  Authors: Pei-chun (Payne) Cheng
 *  Date:    Dec 20, 2008
 */

#ifndef XMLDATA_H_ 

/* Constants */
#define XFB_VERSION             "0.2"                                  /* Current XFB version    */
#define XFB_NS                  "urn:ietf:params:xml:ns:xfb-0.2"       /* Current XFB name space */

/*----------------------------------------------------------------------------------------
 * Purpose: entry fucntion which converts all types of BMF messages to XML text representations
 * input:   bmf - our internal BMF message
 *          xml - pointer to the buffer used for conversion
 *          maxlen - max length of the buffer
 * output:  the length of generated xml string
 * Pei-chun Cheng @ Dec 20, 2008
 * -------------------------------------------------------------------------------------*/ 
int BMF2XMLDATA(BMF bmf, char *xml, int maxlen);

#endif /*XMLDATA_H_*/

/* vim: sw=4 ts=4 sts=4 expandtab
 */
