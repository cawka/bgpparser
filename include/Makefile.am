SUBDIRS = parsers visitors

AM_CPPFLAGS = -I$(top_srcdir)/include $(log4cxx_CFLAGS) $(BOOST_CPPFLAGS)

BUILT_SOURCES = bgpparser.h.gch

%.h.gch: %.h
	        $(CXX) -c $(AM_CPPFLAGS) $(DEFS) $(CXXFLAGS) $<

clean-local:
	    -rm *.gch

fooincludedir=$(pkgincludedir)
fooinclude_HEADERS = Exceptions.h bgpparser.h  config.h 

