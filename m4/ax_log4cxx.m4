##########################################################################
# log4cxx support
##########################################################################
#   AX_LOG4CXX( )
#
# DESCRIPTION
#
#   This macro calls:
#
#     AC_SUBST(log4cxx_CFLAGS) / AC_SUBST(log4cxx_LIBS)
#
#   And sets:
#
#     LOG4CXX
#

AC_DEFUN([AX_LOG4CXX],
[
AC_MSG_CHECKING([whether to build with log4cxx support])
AC_ARG_ENABLE([log4cxx],
			  [AS_HELP_STRING([--disable-log4cxx],
							  [disable log4cxx support (def=yes)])],
							  [log4cxx="$enableval"],
							  [log4cxx=yes])
AC_MSG_RESULT([$log4cxx])

if test x"$log4cxx" = x"yes"; then
	AC_DEFINE([LOG4CXX],[],[Log4cxx support])

	PKG_CHECK_MODULES( log4cxx, [liblog4cxx] )
	AC_SUBST(log4cxx_CFLAGS)
	AC_SUBST(log4cxx_LIBS)
fi
] )

