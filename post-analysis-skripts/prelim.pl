#!/usr/bin/perl
use strict;
use warnings"all";

my @files = @ARGV;
my $matrix = shift@files;

foreach my $f (@files){
    print STDERR $f,"\n";
    open(F,"gunzip -c $f |");
    
    my @temp = split(/_/,$f);
    my $it = $temp[2];
    $it =~ s/\..*$//;
    while(<F>){
	my ($pattern,$val1,$val2,$val3) = split(/\s+/);
	print $pattern,"\t",$it,"\t",$val1/($val1+$val2+$val3),"\n";
    }
    close(F);
}
exit;    
open(M,"<",$matrix);
while(<M>){
    chomp;
    my ($p,$g) = split(/\s+/);
    open(G,">",$p."_".$g.".graph");
    for(my $i = 0; $i < 10; $i++){
	my $l = <M>;
	print G $l;
    }
    close(G);
}
close(M);
