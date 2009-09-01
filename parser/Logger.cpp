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

// Author: Jonathan Park (jpark@cs.ucla.edu)
#include "Logger.h"
#include <ctime>

/* static members */
FILE *Logger::fdout = NULL; 
FILE *Logger::fderr = NULL; 

time_t Logger::t = 0;
struct tm *Logger::ts = NULL;
char Logger::msgbuf[LOG_BUF_SIZE];
char Logger::timebuf[TIME_BUF_SIZE];

int Logger::init(void) {
    char f[32];

    memset(Logger::msgbuf,0,LOG_BUF_SIZE);
    memset(Logger::timebuf,0,TIME_BUF_SIZE);

    Logger::t = time(NULL);
    Logger::ts = localtime(&Logger::t);
    strftime(Logger::timebuf, TIME_BUF_SIZE, "%Y-%m-%d", Logger::ts);

    snprintf(f,32,"log.%s.txt",Logger::timebuf);
    Logger::fdout = fopen( f, "a+" );

    snprintf(f,32,"err.%s.txt",Logger::timebuf);
    Logger::fderr = fopen( f, "a+" );

    if( Logger::fdout == NULL ) { return -1; }
    return 0;
}

int Logger::out(const char fmt[], ... ) {

    if( Logger::fdout == NULL ) { return -1; }

	va_list		ap;
	va_start( ap, fmt );
	(void)vsnprintf( msgbuf, LOG_BUF_SIZE-1, fmt, ap );
	va_end( ap );

    Logger::t = time(NULL);
    Logger::ts = localtime(&Logger::t);
    strftime(Logger::timebuf, TIME_BUF_SIZE, "%a %Y-%m-%d %H:%M:%S %Z", Logger::ts);

    fprintf(fdout,"%s: %s\n",Logger::timebuf,Logger::msgbuf);
    return 0;
}

int Logger::err(const char fmt[], ... ) {

    if( Logger::fderr == NULL ) { return -1; }

	va_list		ap;
	va_start( ap, fmt );
	(void)vsnprintf( msgbuf, LOG_BUF_SIZE-1, fmt, ap );
	va_end( ap );

    Logger::t = time(NULL);
    Logger::ts = localtime(&Logger::t);
    strftime(Logger::timebuf, TIME_BUF_SIZE, "%a %Y-%m-%d %H:%M:%S %Z", Logger::ts);

    fprintf(fderr,"%s: %s\n",Logger::timebuf,Logger::msgbuf);

    fflush(fderr);
    return 0;
}

int Logger::finalize(void) {
    fclose(Logger::fdout);
    fclose(Logger::fderr);
    return 0;
}
// vim: sw=4 ts=4 sts=4 expandtab
