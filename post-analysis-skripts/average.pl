#!/usr/bin/perl
use strict;
use warnings"all";

my $file = $ARGV[0];

$file =~ /[A-Za-z0-9]+_(\d+)_(\d+)\.basinscd\.(\d+)\..*/;

my $model = $1;
my $rep = $2;
my $pattern = $3;
my $inv = 1023-$3;

my $average = 0;
my $av2 = 0;
my $count = 0;
my $index;
my $invind;
open(IN,"<",$file);
my $header = <IN>;
my @t = split(/\t/,$header);
if($t[1] == $pattern){
    $index = 1;
    $invind = 3;
}else{
    $index = 3;
    $invind = 1;
}
while(<IN>){
    chomp;
    my @f = split;
    if($f[0] < 200){next;}
    $average += $f[$index];
    $av2 += $f[$invind];
    $count++;
}
close(IN);

print $model,"\t",$rep,"\t",$pattern,"\t",($average/$count),"\n";
print $model,"\t",$rep,"\t",$inv,"\t",($av2/$count),"\n";
