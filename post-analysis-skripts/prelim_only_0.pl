#!/usr/bin/perl
use strict;
use warnings"all";

my @files = @ARGV;

foreach my $f (@files){
    print STDERR $f,"\n";
    open(F,"gunzip -c $f |");
    my @temp = split(/_/,$f);
    my $it = $temp[1];#2 fuer die wlfs
    $it =~ s/\..*$//;
    my %pattern;
    while(<F>){
	my ($from,$to,$restore) = split;
	if($to != 0 || $from != $restore){next;}
	$pattern{$from}++;
    }
    close(F);
    foreach my $p (keys %pattern){
	print $p,"\t",$it,"\t",$pattern{$p}/1000,"\n";
    }
}
