 # Copyright (c) 2008,2009, University of California, Los Angeles All rights reserved.
 # 
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions are met:
 # 
 # * Redistributions of source code must retain the above copyright notice,
 #     this list of conditions and the following disclaimer.
 # * Redistributions in binary form must reproduce the above copyright
 #     notice, this list of conditions and the following disclaimer in the
 #     documentation and/or other materials provided with the distribution.
 # * Neither the name of NLnetLabs nor the names of its
 #     contributors may be used to endorse or promote products derived from this
 #     software without specific prior written permission.
 # 
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 # AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 # ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 # LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 # CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 # SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 # INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 # CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 # ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 # POSSIBILITY OF SUCH DAMAGE.

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.62)
AC_INIT([bgpparser], [0.1.2], [rveloso@cs.ucla.edu])
AC_CONFIG_SRCDIR([parser/AttributeType.cpp])
AC_CONFIG_HEADERS([include/config.h])

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL

PKG_CHECK_MODULES( XML, [libxml-2.0] )
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)

AX_LOG4CXX

AX_BOOST_BASE( [1.44],, [AC_MSG_ERROR([Boost C++ Libraries are not installed, but are required by libbgpparser])] ) 
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_REGEX
AX_BOOST_IOSTREAMS

AC_PROG_LIBTOOL
AC_PROG_RANLIB

AX_DEBUG

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdint.h string.h sys/socket.h unistd.h])

dnl # Checks for typedefs, structures, and compiler characteristics.
dnl AC_HEADER_STDBOOL
dnl AC_C_INLINE
dnl AC_TYPE_SIZE_T
dnl AC_TYPE_SSIZE_T
dnl AC_TYPE_UINT16_T
dnl AC_TYPE_UINT32_T
dnl AC_TYPE_UINT8_T
dnl 
dnl 
dnl # Checks for library functions.
dnl AC_FUNC_MALLOC
dnl AC_CHECK_FUNCS([inet_ntoa memset])
dnl AC_CHECK_FUNCS([memmove])
dnl AC_CHECK_FUNCS([strerror])
dnl AC_CHECK_FUNCS([strstr])
dnl AC_CHECK_HEADERS([limits.h])
dnl AC_FUNC_REALLOC


AC_OUTPUT(Makefile bgpparser.pc
		  parser/Makefile 
		  dumper/Makefile
		  examples/Makefile
		  )
