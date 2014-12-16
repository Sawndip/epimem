#/usr/bin/perl
use strict; 
use warnings"all";

#srand(NULL);

my $pattern_file = $ARGV[0];
my $lfs = $ARGV[1];
my $lfe = $ARGV[2];
my $lfst = $ARGV[3];
my $sss = $ARGV[4];
my $sse = $ARGV[5];
my $ssst = $ARGV[6];

open(PAT,"<",$pattern_file);
while(<PAT>){
    chomp;
    my ($p1,$p2) = split;
    if(rand() >= 0.5){$p1 = 1023 - $p1;}
    if(rand() >= 0.5){$p2 = 1023 - $p2;}
    for(my $lf = $lfs; $lf <= $lfe; $lf += $lfst){
	for(my $ss = $sss; $ss <= $sse; $ss += $ssst){
	    my $out = "spatial_result/$p1"."_$p2"."_$ss"."_$lf";
	    system("./spatialS bla $p1 $p2 $ss $lf 10 > $out")
	}
    }
}
close(PAT);
