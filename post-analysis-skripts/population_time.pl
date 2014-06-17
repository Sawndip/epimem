#!/usr/bin/perl
use strict;
use warnings"all";

foreach my $file (@ARGV){
    # load data pattern -> cell division state -> [restored]
    my %data;
    open(IN,"gunzip -c $file |");
    while(<IN>){
	chomp;
	my ($pattern,$cd,$end) = split;
	if(exists $data{$pattern}->{$cd}){
	    push(@{$data{$pattern}->{$cd}}, $end);
	}else{
	    $data{$pattern}->{$cd} = [$end];
	}
    }
    close(IN);
    
    #open output
    my $out = $file;
    $out =~ s/gz/poptime/;
    open(OUT,">",$out);
    
    #init simulations;
    my @pop = ();
    foreach my $key (keys %data){push(@pop,1);};
    my $sum = scalar@pop;
    my $limit = 50 * scalar@pop;
    while($sum < $limit){
	my $new = int(rand(scalar@pop));
	$pop[$new]++;
	$sum++;
    }

    #pattern vector
    my @patterns = keys %data;
    print OUT "gen";
    foreach my $pattern (@patterns){
	print OUT "\t",$pattern,"\t",$pattern,"_rel";
    }
    print OUT "\n";
    my @org = @pop;
    print OUT "0";
    for(my $i = 0; $i < scalar@pop; $i++){
	print OUT "\t",$pop[$i],"\t1";
    }
    print OUT "\n";
    
    
    #run 1000 steps
    my @last = @pop;
    for(my $i = 1; $i < 1000; $i++){
	#set to zero
	for(my $k = 0; $k < scalar@pop; $k++){$pop[$k] = 0;}
	
	#divide cells of each population
	for(my $k = 0; $k < scalar@pop; $k++){
	    my @cdstates = keys %{$data{$patterns[$k]}};
	    for(my $j = 0; $j < $last[$k]; $j++){
		my $index = int(rand(scalar@cdstates));
		my $temp = int(rand(scalar@{$data{$patterns[$k]}->{$cdstates[$index]}}));
		my $restore = $data{$patterns[$k]}->{$cdstates[$index]}->[$temp];
		for(my $l = 0; $l < scalar@patterns; $l++){
		    if($restore == $patterns[$l]){$pop[$l]++;last;}
		}
	    }
	    
	}
	print OUT "$i";
	for(my $k = 0; $k < scalar@pop; $k++){
	    print OUT "\t",$pop[$k],"\t",($pop[$k]/$org[$k]);
	}
	print OUT "\n";
	
    }    
    close(OUT);
}
