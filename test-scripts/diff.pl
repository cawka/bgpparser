#!/usr/local/bin/perl

# binaries to compare
$sBGPDump = "/home/j13park/libbgpdump-1.4.99.9/bgpdump";
$sBGPParser = "/export/data/j13park/bgpparser/bin/bgpparser";

# data root dir
$sDataRootDir = "/export/data/j13park/mrt";

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
@lsMRTProj = glob("$sDataRootDir/*");

foreach $sProj (@lsMRTProj) {
	@lsCollector = glob("$sProj/*");
	foreach $sCollector (@lsCollector) {
		foreach $sDate (@lsDates) {
			@lsUpdateFiles = glob("$sCollector/$sDate/updates.*");
			foreach $sUpdateFile (sort @lsUpdateFiles) {

print "=================================\n";
print "processing $sUpdateFile ... ";

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
#						print "BGPDUMP  : $s1\n";
#						print "BGPPARSER: $s2\n";
						$nDiffCnt = 0;
						for($i=0; $i<@lsBGPDumpLine; $i++) {
							if( $lsBGPDumpLine[$i] ne $lsBGPParserLine[$i] ) {
								$nDiffCnt++;
								print "$nType $i ( $lsBGPDumpLine[$i] ne $lsBGPParserLine[$i] )\n";
							}
						}
					}
				} 

				close(BGPDUMP);
				close(BGPPARSER);

print "done.\n";
print "=================================\n";

			}
		}
	}
}
