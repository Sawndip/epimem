#!/usr/bin/perl
use strict;
use warnings"all";

my @files = @ARGV;

foreach my $f (@files){
    print STDERR $f,"\n";
    open(F,"gunzip -c $f |");
    my @temp = split(/_/,$f);
    my $it = $temp[2];#2 fuer die wlfs,1 fuer die anderen
    $it =~ s/\..*$//;
    my %pattern;
    while(<F>){
	chomp;
	my ($from,$to,$restore) = split;
	if($to != 0 || $from != $restore){next;}
	$pattern{$from}++;
    }
    close(F);
    my %patternall;
    $f =~ s/basinscd.gz/celldivision.gz/;
    open(F,"gunzip -c $f |");
    while(<F>){
	chomp;
	my ($p,$rest,$diff,$unst) = split;
	$patternall{$p} = $rest/($rest+$diff+$unst);
    }
    close(F);
    
    foreach my $p (keys %patternall){
	my $pcount = 0;
	if(exists $pattern{$p}){$pcount = $pattern{$p}}
	print $p,"\t",$it,"\t",$pcount/1000,"\t",$patternall{$p},"\n";
    }
}
