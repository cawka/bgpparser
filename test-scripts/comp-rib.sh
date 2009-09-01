#!/usr/local/bin/bash

PID=$$
#FILES=`find ribs-rv/rib.*.bz2`
FILES=`find ribs-v2/bview.*.gz`
#FILES=`find ribs-v1/bview.*.gz`

for F in $FILES; do
	/amnt/nuggets/homedirs/rveloso/libbgpdump-1.4.99.8/bgpdump -m $F | gzip > bgpdump-$PID.out.gz
	../parser/bgpparser -m $F | ./cvRib2Bgpdump.pl | gzip > bgpparser-$PID.out.gz
	if [ -f $F.comp.gz ]; then
		mv -f $F.comp.gz $F.comp.gz.old
	fi
	./cmp.pl bgpdump-$PID.out.gz bgpparser-$PID.out.gz | gzip > $F.comp.gz
done
rm -f bgpdump-$PID.out.gz bgpparser-$PID.out.gz
