#!/usr/local/bin/perl

# 1235894398|TABLE_DUMP_V2|ENTRY_CNT: 12
#4.0.0.0/8|2|91.103.24.1|42109|ORIGIN: EGP|AS_SEQUENCE:  42109 41965 41877 20771 174 3356|NEXT_HOP: 91.103.24.1|AA: AG|AGGREGATOR: 3356 4.69.130.82|
#4.0.0.0/8|2|193.138.164.1|34225|ORIGIN: IGP|AS_SEQUENCE:  34225 1299 3356|NEXT_HOP: 193.138.164.1|AA: AG|AGGREGATOR: 3356 4.69.130.78|COMMUNITIES: 34225:1000 34225:1001|

# 1235894406|TABLE_DUMP_V2|::/0|296618||2a02:38::2|6881|ORIGIN: IGP|AS_SEQUENCE:  6881 6706 29208 8648 48200|MBGP-NEXTHOP: 2a02:38::2^MBGP-ANNOUNCE:|

# TABLE_DUMP2|1235894398|B|193.138.164.1|34225|4.0.0.0/9|34225 1299 3356|IGP|193.138.164.1|0|0|34225:1000 34225:1001|AG|3356 4.69.130.78|

$ATTR_ORIGIN = "ORIGIN:";
$ATTR_ASPATH = "AS_SEQUENCE:";
$ATTR_NH = "NEXT_HOP:";
$ATTR_AA = "AA:";
$ATTR_AGG = "AGGREGATOR:";
$ATTR_LP = "LOCAL_PREF:";
$ATTR_MED = "MULTI_EXIT_DISC:";
$ATTR_COMM = "COMMUNITIES:";

while( $sLine=<STDIN> ) {
	chomp($sLine);
	if( $sLine eq "" ) { next; }
	($nTS,$sFormat,@ls) = split('\|',$sLine);

	if( $sFormat eq "TABLE_DUMP_V2" ) {
		$sEntryCntStr = shift(@ls);
		if( $sEntryCntStr eq "PEERINDEXTABLE" ) { next; }

		($sEntryCntKey,$nEntryCnt) = split(" ",$sEntryCntStr);
		if( $sEntryCntKey ne "ENTRY_CNT:" ) { print "unexpected format [$sLine]\n"; next; }

		$sFormat = "TABLE_DUMP2";
		for($i=0;$i<$nEntryCnt;$i++) {

			%htAttr = ();
			$htAttr{$ATTR_LP} = 0;
			$htAttr{$ATTR_MED} = 0;
			$htAttr{$ATTR_AA} = "NAG";
			$htAttr{$ATTR_AGG} = "";

			$sEntry = <STDIN>;
			chomp($sEntry);
			@lsEntry = split('\|',$sEntry);
			$sPrefix = shift(@lsEntry);
			$nNone = shift(@lsEntry);
			$sPeerIP = shift(@lsEntry);
			$sPeerASN = shift(@lsEntry);

			@lsMBGPANN = ();
			foreach $sAttrPair (@lsEntry) {
				@lsAttrPair = split(" ",$sAttrPair);
				$sKey = shift(@lsAttrPair);	
				$sVal = join(" ",@lsAttrPair);

				if( $sKey eq "MBGP-NEXTHOP:" ) {
					@lsMBGP = split('\^',$sVal);
					$htAttr{$ATTR_NH} = shift(@lsMBGP);
					@lsMBGPANN = split(" ",shift(@lsMBGP));
					shift(@lsMBGPANN);
				} else {
					$htAttr{$sKey} = $sVal;
				}
			}
			if( @lsMBGPANN > 0 ) {
				for $sPrefix (@lsMBGPANN) {
					print "$sFormat|$nTS|B|$sPeerIP|$sPeerASN|$sPrefix|$htAttr{$ATTR_ASPATH}|";
					print "$htAttr{$ATTR_ORIGIN}|$htAttr{$ATTR_NH}|$htAttr{$ATTR_LP}|$htAttr{$ATTR_MED}|";
					print "$htAttr{$ATTR_COMM}|$htAttr{$ATTR_AA}|$htAttr{$ATTR_AGG}|\n";
				}
			} else {
				print "$sFormat|$nTS|B|$sPeerIP|$sPeerASN|$sPrefix|$htAttr{$ATTR_ASPATH}|";
				print "$htAttr{$ATTR_ORIGIN}|$htAttr{$ATTR_NH}|$htAttr{$ATTR_LP}|$htAttr{$ATTR_MED}|";
				print "$htAttr{$ATTR_COMM}|$htAttr{$ATTR_AA}|$htAttr{$ATTR_AGG}|\n";
			}
		}
	} elsif( $sFormat eq "TABLE_DUMP" ) {
		($nViewNum,$nSeqNum,$sPrefix,$nStatus,$sPeerIP,$nPeerASN,@lsAttr) = @ls;
		%htAttr = ();
		$htAttr{$ATTR_LP} = 0;
		$htAttr{$ATTR_MED} = 0;
		$htAttr{$ATTR_AA} = "NAG";
		$htAttr{$ATTR_AGG} = "";
		foreach $sAttrPair (@lsAttr) {
			@lsAttrPair = split(" ",$sAttrPair);
			$sKey = shift(@lsAttrPair);	
			$sVal = join(" ",@lsAttrPair);

			if( $sKey eq "MBGP-NEXTHOP:" ) {
				@lsMBGP = split('\^',$sVal);
				$htAttr{$ATTR_NH} = shift(@lsMBGP);
				@lsMBGPANN = split(" ",shift(@lsMBGP));
				shift(@lsMBGPANN);
			} else {
				$htAttr{$sKey} = $sVal;
			}
		}
		print "$sFormat|$nTS|B|$sPeerIP|$nPeerASN|$sPrefix|$htAttr{$ATTR_ASPATH}|";
		print "$htAttr{$ATTR_ORIGIN}|$htAttr{$ATTR_NH}|$htAttr{$ATTR_LP}|$htAttr{$ATTR_MED}|";
		print "$htAttr{$ATTR_COMM}|$htAttr{$ATTR_AA}|$htAttr{$ATTR_AGG}|\n";
	}
}
