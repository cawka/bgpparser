##########################################################################
# debug compilation support
##########################################################################
#   AX_DEBUG()
#
# DESCRIPTION
#
#   This macro sets:
#
#     DEBUG
#

AC_DEFUN([AX_DEBUG],
[
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
] )

