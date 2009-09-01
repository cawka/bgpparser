#!/usr/local/bin/perl

$ATTR_ORIGIN = "ORIGIN:";
$ATTR_ASPATH = "AS_SEQUENCE:";
$ATTR_AS4PATH = "AS4_SEQUENCE:";
$ATTR_NH = "NEXT_HOP:";
$ATTR_AA = "AA:";
$ATTR_AGG = "AGGREGATOR:";
$ATTR_LP = "LOCAL_PREF:";
$ATTR_MED = "MULTI_EXIT_DISC:";
$ATTR_COMM = "COMMUNITIES:";
$ATTR_WITHCNT = "WITH_CNT:";
$ATTR_ATTRCNT = "ATTR_CNT:";
$ATTR_ANNCNT = "ANN_CNT:";
$ATTR_MBGP = "MBGP-NEXTHOP:";
$ATTR_MBGPANN = "MBGP-ANNOUNCE:";
$ATTR_MBGPWITH = "MBGP-WITHDRAWN:";

#1235865609|BGP4MP|193.138.164.1|34225|193.0.4.28|12654|WITH_CNT: 0||ATTR_CNT: 6|ORIGIN: IGP|AS_SEQUENCE:  34225 1299 3549 27065 27065 27065 1733 6037|NEXT_HOP: 193.138.164.1|AA: AG|AGGREGATOR: 65535 144.106.214.226|COMMUNITIES: 34225:1000 34225:1001|ANN_CNT: 1|144.106.215.0/24|

while( $sLine=<STDIN> ) {
	if( $sLine =~ m/KEEPALIVE/ ) { next; }
	chomp($sLine);
#print "+++ $sLine\n";
	if( $sLine eq "" ) { next; }
	($nTS,$sFormat,$sPeerIP,$nPeerASN,$sLocalIP,$nLocalASN,@lsAttr) = split('\|',$sLine);
# dm> 1235865887|STATE|203.119.0.116|2.0|3|2
# pm> BGP4MP|1235865766|STATE|203.119.0.116|2.0|4|3

	if( $sLine =~ m/STATE/ ) { print "BGP4MP|$nTS|STATE|$sPeerIP|$nPeerASN|$sLocalIP|$nLocalASN\n"; next; }

#print "$nTS,$sFormat,$sLocalIP,$nLocalASN,$sPeerIP,$nPeerASN\n";

	%htAttr = ();
	$htAttr{$ATTR_LP} = 0;
	$htAttr{$ATTR_MED} = 0;
	$htAttr{$ATTR_AA} = "NAG";
	$htAttr{$ATTR_AGG} = "";
	@lsMBGPANN = ();
	@lsMBGPWITH = ();

	($sWithKey,$nWithCnt) = split(" ",shift(@lsAttr));
#print "with $sWithKey,$nWithCnt\n";
	@lsWithdrawnPrefixes = split(" ",shift(@lsAttr));
	($sAttrKey,$nAttrCnt) = split(" ",shift(@lsAttr));
#print "attr $sAttrKey,$nAttrCnt\n";
# MBGP-NEXTHOP: 129.250.0.11^MBGP-ANNOUNCE: 203.25.92.0/22 150.229.0.0/19 192.107.9.0/24 152.83.0.0/16
	for($i=0;$i<$nAttrCnt;$i++) {
		($sKey,@lsVal) = split(" ",shift(@lsAttr));
		if( $sKey eq "MBGP-NEXTHOP:" ) {
			@lsMBGP = split('\^',"@lsVal");
			$htAttr{$ATTR_NH} = shift(@lsMBGP);
#print "NH: $htAttr{$ATTR_NH}\n";
			@lsMBGPANN = split(" ","@lsMBGP");
			shift(@lsMBGPANN);
#for($k=0; $k<@lsMBGPANN; $k++) {
#print " - MBGP-ANN ($k): $lsMBGPANN[$k]\n";
#}
		} elsif( $sKey eq "MBGP-WITHDRAWN:" ) {
			@lsMBGPWITH = @lsVal;
		} else {
#print "key: $sKey val: @lsVal\n";
			$htAttr{$sKey} = "@lsVal";
		}
	}
	($sAnnKey,$nAnnCnt) = split(" ",shift(@lsAttr));
#print "ann $sAnnKey,$nAnnCnt\n";
	@lsAnnouncedPrefixes = split(" ",shift(@lsAttr));

	if( defined $htAttr{$ATTR_AS4PATH} ) {
		@lsASPath = ();
		@ls1 = split(" ",$htAttr{$ATTR_ASPATH});
		@ls2 = split(" ",$htAttr{$ATTR_AS4PATH});
		$nLenDiff = @ls1 - @ls2;
		$nAS4Len = @ls2;
		for($i=0; $i<$nLenDiff; $i++) {
			push(@lsASPath,shift(@ls1));
		}
		for($i=0; $i<$nAS4Len; $i++) {
			push(@lsASPath,shift(@ls2));
		}
		$htAttr{$ATTR_ASPATH} = join(" ",@lsASPath);
	}

	foreach $sPrefix (@lsWithdrawnPrefixes) {
		print "$sFormat|$nTS|W|$sPeerIP|$nPeerASN|$sPrefix\n"
	}
	foreach $sPrefix (@lsAnnouncedPrefixes) {
		print "$sFormat|$nTS|A|$sPeerIP|$nPeerASN|$sPrefix|$htAttr{$ATTR_ASPATH}|";
		print "$htAttr{$ATTR_ORIGIN}|$htAttr{$ATTR_NH}|$htAttr{$ATTR_LP}|$htAttr{$ATTR_MED}|";
		print "$htAttr{$ATTR_COMM}|$htAttr{$ATTR_AA}|$htAttr{$ATTR_AGG}|\n";
	}
	foreach $sPrefix (@lsMBGPANN) {
		print "$sFormat|$nTS|A|$sPeerIP|$nPeerASN|$sPrefix|$htAttr{$ATTR_ASPATH}|";
		print "$htAttr{$ATTR_ORIGIN}|$htAttr{$ATTR_NH}|$htAttr{$ATTR_LP}|$htAttr{$ATTR_MED}|";
		print "$htAttr{$ATTR_COMM}|$htAttr{$ATTR_AA}|$htAttr{$ATTR_AGG}|\n";
	}
	foreach $sPrefix (@lsMBGPWITH) {
		print "$sFormat|$nTS|W|$sPeerIP|$nPeerASN|$sPrefix\n"
	}
}
