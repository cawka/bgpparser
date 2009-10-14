#!/usr/local/bin/perl

# binaries to compare
$sBGPDump = "/home/j13park/libbgpdump-1.4.99.9/bgpdump";
$sBGPParser = "/export/data/j13park/bgpparser/bin/bgpparser";

# data root dir
$sDataRootDir = "/export/data/j13park/mrt-new";

# date range
@lsDates = ();
if( @ARGV == 1 ) {
	push(@lsDates,$ARGV[0]);
} else {
	push(@lsDates,"2009.06");
}

# utility
@lsDumpBuffer = ();
@lsParserBuffer = ();
$nMaxLookAhead = 10;

sub getLine {
	my ($sType) = @_;

	my $s = undef;

	if( $sType eq "DUMP" ) {
		if( @lsDumpBuffer > 0 ) {
			$s = shift(@lsDumpBuffer);
		} else {
			$s = <BGPDUMP>;
		}
	} elsif( $sType eq "PARSER" ) {
		if( @lsParserBuffer > 0 ) {
			$s = shift(@lsParserBuffer);
		} else {
			$s = <BGPPARSER>;
		}
	}

	return $s;
}

sub lookAhead {
	my ($sType) = @_;

	my $s = undef;

	if( $sType eq "DUMP" ) {
		$s = <BGPDUMP>;
		push(@lsDumpBuffer,$s);
	} elsif( $sType eq "PARSER" ) {
		$s = <BGPPARSER>;
		push(@lsParserBuffer,$s);
	}
	return $s;
}

# main
#@lsMRTProj = glob("$sDataRootDir/*");
@lsMRTProj = ("$sDataRootDir/rv");

foreach $sProj (@lsMRTProj) {
	@lsCollector = glob("$sProj/*");
	foreach $sCollector (@lsCollector) {
		foreach $sDate (@lsDates) {
			@lsUpdateFiles = glob("$sCollector/$sDate/updates.*");
			foreach $sUpdateFile (sort @lsUpdateFiles) {

$sUpdateFile = "/export/data/j13park/mrt-new/rv/oreg/2009.06/updates.20090601.0000.bz2";
print "=================================\n";
print "processing $sUpdateFile\n";

				open(BGPDUMP,"$sBGPDump -m $sUpdateFile |");
				open(BGPPARSER,"$sBGPParser -B $sUpdateFile |");

				# BGP4MP|1235865609|A|91.103.24.1|42109|193.219.31.0/24|42109 20681|EGP|91.103.24.1|0|0||NAG||NONE
				while( $sBGPDumpLine = &getLine("DUMP") ) {
					chomp($sBGPDumpLine);
					@lsBGPDumpLine = split('\|',$sBGPDumpLine);
#					$sMBGP = pop(@lsBGPDumpLine);
#					if( $sMBGP ne "NONE" ) { next; }

					$sBGPParserLine =  &getLine("PARSER");
					chomp($sBGPParserLine);
					@lsBGPParserLine = split('\|',$sBGPParserLine);
					$lsBGPParserLine[2] eq "A" ? $nType = 1 : $nType = 2;

					$s1 = join('|',@lsBGPDumpLine);
					$s2 = join('|',@lsBGPParserLine);
					while( $s1 =~ m/.*\|$/ ) { chop($s1); }
					while( $s2 =~ m/.*\|$/ ) { chop($s2); }

					if( $s1 ne $s2 ) {
						$nDiffCnt = 0;
						for($i=0; $i<@lsBGPDumpLine; $i++) {
							if( $lsBGPDumpLine[$i] ne $lsBGPParserLine[$i] ) {
								$nDiffCnt++;
							}
						}
						if( $nDiffCnt < 3 ) {
							for($i=0; $i<@lsBGPDumpLine; $i++) {
								if( $lsBGPDumpLine[$i] ne $lsBGPParserLine[$i] ) {
									print "$nType $i ( $lsBGPDumpLine[$i] ne $lsBGPParserLine[$i] )\n";
								}
							}
						} else {
							$nFoundMatch = 0;
							$nLookAheadCnt = 0;
							$s2l = $s2;
							while ( $nFoundMatch == 0 ) {
								$nLookAheadCnt++;
								if( $nLookAheadCnt > $nMaxLookAhead ) { last; }
								print "bgpdump missing $s2l ?\n";
								$sBGPParserLine =  &lookAhead("PARSER");
								chomp($sBGPParserLine);
								@lsBGPParserLine = split('\|',$sBGPParserLine);
								$lsBGPParserLine[2] eq "A" ? $nType = 1 : $nType = 2;
								$s2l = join('|',@lsBGPParserLine);
								while( $s1 =~ m/.*\|$/ ) { chop($s1); }
								while( $s2l =~ m/.*\|$/ ) { chop($s2l); }

								if( $s1 ne $s2l ) {
									$nDiffCnt = 0;
									for($i=0; $i<@lsBGPDumpLine; $i++) {
										if( $lsBGPDumpLine[$i] ne $lsBGPParserLine[$i] ) {
											$nDiffCnt++;
										}
									}
									if( $nDiffCnt <= 2 ) {
										while( $nLookAheadCnt >= 0 ) { shift(@lsParserBuffer); $nLookAheadCnt--; }
										$nFoundMatch = 1; goto L_DONE;
									}
								} else {
									while( $nLookAheadCnt >= 0 ) { shift(@lsParserBuffer); $nLookAheadCnt--; }
									$nFoundMatch = 1; goto L_DONE;
								}
							}

							$nFoundMatch = 0;
							$nLookAheadCnt = 0;
							$s1l = $s1;
							while ( $nFoundMatch == 0 ) {
								$nLookAheadCnt++;
								if( $nLookAheadCnt > $nMaxLookAhead ) { last; }
								print "bgpparser missing $s1l ?\n";
								$sBGPDumpLine =  &lookAhead("PARSER");
								chomp($sBGPDumpLine);
								@lsBGPDumpLine = split('\|',$sBGPDumpLine);
								$lsBGPDumpLine[2] eq "A" ? $nType = 1 : $nType = 2;
								$s1l = join('|',@lsBGPDumpLine);
								while( $s1l =~ m/.*\|$/ ) { chop($s1l); }
								while( $s2 =~ m/.*\|$/ ) { chop($s2); }

								if( $s1l ne $s2 ) {
									$nDiffCnt = 0;
									for($i=0; $i<@lsBGPDumpLine; $i++) {
										if( $lsBGPDumpLine[$i] ne $lsBGPDumpLine[$i] ) {
											$nDiffCnt++;
										}
									}
									if( $nDiffCnt <= 2 ) {
										while( $nLookAheadCnt >= 0 ) { shift(@lsDumpBuffer); $nLookAheadCnt--; }
										$nFoundMatch = 1; goto L_DONE;
									}
								} else {
									while( $nLookAheadCnt >= 0 ) { shift(@lsDumpBuffer); $nLookAheadCnt--; }
									$nFoundMatch = 1; goto L_DONE;
								}
							}
						}
					}
L_DONE:
				} 

				close(BGPDUMP);
				close(BGPPARSER);

L_SKIP:

			}
		}
	}
}
