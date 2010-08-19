#!/usr/local/bin/perl

if( @ARGV != 2 ) {
	print "USAGE: <prog> <bgpdump-output> <bgpparser-output>\n";
	exit;
}

%htOne = ();
open(IN,"gzcat $ARGV[0] |");
while( $sLine=<IN> ) {
	chomp($sLine);
	$htOne{$sLine} = 1;

	($sFormat,$nTS,$sAnnType,$sPeerIP,
	 $nPeerASN,$sPrefix,$sASPath,$sOrigin,
	 $sNextHop,$nLocalPref,$nMED,$sComm,$sAA,$sAgg) = split('\|',$sLine);

	if( $sAnnType eq "A" ) {
		$sNextHop = "255.255.255.255";
		$sNewLine = "$sFormat|$nTS|$sAnnType|$sPeerIP|$nPeerASN|$sPrefix|$sASPath|$sOrigin|$sNextHop|$nLocalPref|$nMED|$sComm|$sAA|$sAgg|";
		$htOne{$sNewLine} = 1;
	}
}
close(IN);

%htTwo = ();
%htOneMissing = ();
open(IN,"gzcat $ARGV[1] |");
while( $sLine=<IN> ) {
	chomp($sLine);
	$htTwo{$sLine} = 1;

	($sFormat,$nTS,$sAnnType,$sPeerIP,
	 $nPeerASN,$sPrefix,$sASPath,$sOrigin,
	 $sNextHop,$nLocalPref,$nMED,$sComm,$sAA,$sAgg) = split('\|',$sLine);

	if( $sAnnType eq "A" ) {
		$sNextHop = "255.255.255.255";
		$sNewLine = "$sFormat|$nTS|$sAnnType|$sPeerIP|$nPeerASN|$sPrefix|$sASPath|$sOrigin|$sNextHop|$nLocalPref|$nMED|$sComm|$sAA|$sAgg|";
		$htTwo{$sNewLine} = 1;
	}

	if( ! defined $htOne{$sLine} && ! defined $htOne{$sNewLine} ) {
		$htOneMissing{$sLine} = 1;
	}
}
close(IN);

%htTwoMissing = ();
foreach $sLine (keys %htOne) {

	($sFormat,$nTS,$sAnnType,$sPeerIP,
	 $nPeerASN,$sPrefix,$sASPath,$sOrigin,
	 $sNextHop,$nLocalPref,$nMED,$sComm,$sAA,$sAgg) = split('\|',$sLine);

	$sNextHop = "255.255.255.255";
	$sNewLine = "$sFormat|$nTS|$sAnnType|$sPeerIP|$nPeerASN|$sPrefix|$sASPath|$sOrigin|$sNextHop|$nLocalPref|$nMED|$sComm|$sAA|$sAgg|";

	if( ! defined $htTwo{$sLine} && ! defined $htTwo{$sNewLine} ) {
		$htTwoMissing{$sLine} = 1;
	}
}

foreach $sLine (keys %htOneMissing) {
	print "dm> $sLine\n";
}

foreach $sLine (keys %htTwoMissing) {
	print "pm> $sLine\n";
}
