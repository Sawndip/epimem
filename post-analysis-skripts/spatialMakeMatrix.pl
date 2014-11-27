#!/usr/bin/perl
use strict;
use warnings"all";

my $dir = $ARGV[0]; 
my $outpre = $ARGV[1];

opendir(DIR,$dir);
my @files = readdir(DIR);
closedir(DIR);

my %total;
my %totalc;
my %ratio;
my %ratioc;
my %p1;
my %p1c;
my %p2;
my %p2c;
foreach my $file (@files){
    my $full = $dir."/".$file;
    my ($bla1,$bla2,$ss,$lf) = split(/_/,$file);
    open(IN,"<",$full);
    while(<IN>){
	chomp;
	my ($num1,$pattern1,$cells1) = split;
	my $sec = <IN>;
	my ($num2,$pattern2,$cells2) = split(/\s/,$sec);
	$total{$ss}->{$lf} += $cells1+$cells2;
	$totalc{$ss}->{$lf} += 2;
	if(($cells1+$cells2) != 0){$ratio{$ss}->{$lf} += $cells1/($cells1+$cells2);}
	else {$ratio{$ss}->{$lf} += 0;}
	$ratioc{$ss}->{$lf}++;
	$p1{$ss}->{$lf} += $cells1;
	$p1c{$ss}->{$lf}++;
	$p2{$ss}->{$lf} += $cells2;
	$p2c{$ss}->{$lf}++;
    }
    close(IN);
}

foreach my $ss (keys %total){
    foreach my $lf (keys %{$total{$ss}}){
	$total{$ss}->{$lf} /= $totalc{$ss}->{$lf};
	$ratio{$ss}->{$lf} /= $ratioc{$ss}->{$lf};
    }
}

foreach my $ss (keys %p1){
    foreach my $lf (keys %{$p1{$ss}}){
	$p1{$ss}->{$lf} /= $p1c{$ss}->{$lf};
    }
}

foreach my $ss (keys %p2){
    foreach my $lf (keys %{$p2{$ss}}){
	$p2{$ss}->{$lf} /= $p2c{$ss}->{$lf};
    }
}

my @sss = sort{$a <=> $b} keys %total;
my @lfs = sort{$a <=> $b} keys %{$total{$sss[0]}};

open(OUT,">",$outpre.".total");
print OUT join("\t",@lfs),"\n";
foreach my $ss (@sss){
    print OUT $ss;
    foreach my $lf (@lfs){
	print OUT "\t",$total{$ss}->{$lf};
    }
    print OUT "\n";
}
close(OUT);

open(OUT,">",$outpre.".ratio");
print OUT join("\t",@lfs),"\n";
foreach my $ss (@sss){
    print OUT $ss;
    foreach my $lf (@lfs){
	print OUT "\t",$ratio{$ss}->{$lf};
    }
    print OUT "\n";
}
close(OUT);

open(OUT,">",$outpre.".p1");
print OUT join("\t",@lfs),"\n";
foreach my $ss (@sss){
    print OUT $ss;
    foreach my $lf (@lfs){
	print OUT "\t",$p1{$ss}->{$lf};
    }
    print OUT "\n";
}
close(OUT);

open(OUT,">",$outpre.".p2");
print OUT join("\t",@lfs),"\n";
foreach my $ss (@sss){
    print OUT $ss;
    foreach my $lf (@lfs){
	print OUT "\t",$p2{$ss}->{$lf};
    }
    print OUT "\n";
}
close(OUT);



