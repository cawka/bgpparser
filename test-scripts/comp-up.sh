#!/usr/local/bin/bash

YEARS="2008 2009"
MONTHS="09 10 11 12" # for 2008
DAYS="01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31"
YEARS="2008"
MONTHS="09" # for 2008
DAYS="06"

PID=$$

for Y in $YEARS; do

	if [ $Y = "2008" ]; then
		MONTHS="09 10 11 12" # for 2008
	elif [ $Y = "2009" ]; then
		MONTHS="09 10 11 12"
	fi

	for M in $MONTHS; do
	for D in $DAYS; do
		FILES=`find /export/data/j13park/mrt/rv/oreg/2008.09/updates.20080906.0021.bz2`
		#FILES=`find /export/data/j13park/mrt/rv/oreg/$Y.$M/updates.$Y$M$D.*.bz2`
		#FILES=`find ../sample/updates.200903${D}.*.gz`

		for F in $FILES; do
			/amnt/nuggets/homedirs/rveloso/libbgpdump-1.4.99.8/bgpdump -m $F | gzip > bgpdump-$PID.out.gz
			../parser/bgpparser -m $F | ./cvUp2Bgpdump.pl | gzip > bgpparser-$PID.out.gz
			if [ -f $F.comp.gz ]; then
				mv -f $F.comp.gz $F.comp.gz.old
			fi
			./cmp.pl bgpdump-$PID.out.gz bgpparser-$PID.out.gz | gzip > $F.comp.gz
		done
	done
	done
done
rm bgpdump-$PID.out.gz bgpparser-$PID.out.gz
