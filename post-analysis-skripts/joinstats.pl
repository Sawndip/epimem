#!/usr/bin/perl
use strict;
use warnings"all";

my $restore = shift@ARGV;
my $graph = shift@ARGV;
my $out = shift@ARGV;

my %hash;
open(IN,"<",$graph);
my $header = <IN>;
while(<IN>){
    chomp;
    my @stats = split(/\s+/);
    my $key = shift@stats;
    $hash{$key} = \@stats;
}
close(IN);

open(OUT,">",$out);
chomp($header);
print OUT $header,"\tpattern\trestore.rate\n";
open(IN,"<",$restore);
while(<IN>){
    chomp;
    my ($p,$i,$r) = split(/\s+/);
    my $key = $p."_".$i;
    if($p > 511){
	$key = (1023-$p)."_".$i;
    }
    if(exists $hash{$key}){
	my $line = $hash{$key};
	print OUT $p,"_",$i,"\t",join("\t",@{$line}),"\t",$p,"\t",$r,"\n";
    }
}
close(IN);
close(OUT);
