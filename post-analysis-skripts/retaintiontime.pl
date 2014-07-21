#!/usr/bin/perl
use strict;
use warnings"all";

my $infile = $ARGV[0];
my $outfile = $ARGV[1];


open(OUT,">",$outfile);
open(IN, "gunzip < $infile |") ;
my $lrep = -1;
my $count = 3;
while(<IN>){
    chomp;
    my ($comefrom, $cds, $rest, $gen, $rep) = split;
    if($lrep != $rep){
	if($count > 0){ print OUT "30\n";}
	$count = 3;
	$lrep = $rep;
    }
    if($count == 0){next;}
    if($comefrom != $rest && $count > 0){
	if($count == 1){
	    print OUT $gen,"\n";
	}
	$count--;
    }
}
close(IN);
close(OUT);

