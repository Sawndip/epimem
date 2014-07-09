#!/usr/bin/perl
use strict;
use warnings"all";

my $infile = $ARGV[0];
my $outfile = $ARGV[1];


open(OUT,">",$outfile);
open(IN, "gunzip < $infile |") ;
my $lrep = -1;
my $count = 1;
while(<IN>){
    chomp;
    my ($comefrom, $cds, $rest, $gen, $rep) = split;
    if($lrep != $rep){
	$count = 1;
	$lrep = $rep;
    }
    if($count == 0){next;}
    if($comefrom != $rest){
	print OUT $gen,"\n";
	$count = 0;
    }
}
close(IN);
close(OUT);
