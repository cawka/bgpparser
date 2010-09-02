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

PKG_CHECK_MODULES( XML, [libxml-2.0] )
AC_SUBST(XML_CFLAGS)
AC_SUBST(XML_LIBS)

PKG_CHECK_MODULES( log4cxx, [liblog4cxx] )
AC_SUBST(log4cxx_CFLAGS)
AC_SUBST(log4cxx_LIBS)

#AX_BOOST_BASE( [1.30] )
AX_BOOST_BASE( [1.30],, [AC_MSG_ERROR([Boost C++ Libraries are not installed, but are required by libbgpparser])] ) 
AX_BOOST_SYSTEM
AX_BOOST_FILESYSTEM
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_REGEX
AX_BOOST_IOSTREAMS

AC_PROG_LIBTOOL
AC_PROG_RANLIB

# Checks for programs.
# AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX

# AM_SILENT_RULES([no])

##########################################################################
# debug compilation support
##########################################################################

AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
			  [AS_HELP_STRING([--enable-debug],
							  [enable debug data generation (def=no)])],
							  [debugit="$enableval"],
							  [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
	AC_DEFINE([DEBUG],[],[Debug Mode])
	AM_CXXFLAGS="$AM_CXXFLAGS -g -Wall -Werror -Wno-uninitialized -O0"
else
	AC_DEFINE([NDEBUG],[],[No-debug Mode])
	AM_CXXFLAGS="$AM_CXXFLAGS -O3"
fi

##########################################################################

# Checks for libraries.


# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdint.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T


# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([inet_ntoa memset])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([strstr])
AC_CHECK_HEADERS([limits.h])
AC_FUNC_REALLOC


AC_OUTPUT(Makefile bgpparser.pc
		  parser/Makefile 
		  dumper/Makefile
		  examples/Makefile
		  )
