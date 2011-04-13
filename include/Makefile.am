SUBDIRS = parsers visitors

AM_CPPFLAGS = -I. -I$(top_srcdir)/include -I$(top_srcdir)/include/parsers $(log4cxx_CFLAGS) $(BOOST_CPPFLAGS)

# BUILT_SOURCES = $(precompiled_headers)
# 
# if GCC
# precompiled_headers = bgpparser.h.gch 
# %.h.gch: %.h
# 	        $(CXX) -c $(AM_CPPFLAGS) $(DEFS) $(CXXFLAGS) $<
# endif
# 
# if CLANG
# precompiled_headers = bgpparser.h.gch 
# %.h.gch: %.h
# 	        $(CXX) -x c++-header -c $(AM_CPPFLAGS) $(DEFS) $(CXXFLAGS) $<
# endif
# 
# clean-local:
# 	-rm *.gch

fooincludedir=$(pkgincludedir)
fooinclude_HEADERS = Exceptions.h bgpparser.h  config.h 

