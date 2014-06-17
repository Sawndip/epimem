#!/usr/bin/perl
use strict;
use warnings"all";

sub getInverse{
    my $p = shift;
    my $cd = shift;
    my $inv = 0;
    my $base = 1;
    for(my $i = 0; $i < 10; $i++){
	my $curp = $p % 2;
	$p = int($p/2);
	my $curcd = $cd % 2;
	$cd = int($cd/2);
	if($curp == 1 && $curcd == 0){$inv += $base;}
	$base *= 2;
    }
    return $inv;
}

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
    
    # calculate cell division pattern pairs
    my %inverse;
    foreach my $pattern (keys %data){
	foreach my $cd (keys %{$data{$pattern}}){
	    my $inv = getInverse($pattern,$cd);
	    $inverse{$pattern}->{$cd} = $inv;
	}
    }

    # clean up patterns cell division state and skip if no usefull cell division state available 
    foreach my $pattern (keys %inverse){
	my $deletepattern = 1;
	foreach my $cd (keys %{$inverse{$pattern}}){
	    if(exists $data{$pattern}->{$cd}){
		if(!exists $data{$pattern}->{$inverse{$pattern}->{$cd}}){
		    delete $data{$pattern}->{$cd};
		}else{
		    $deletepattern = 0;
		}
	    }
	}
	if($deletepattern == 1){
	    delete $data{$pattern};
	}
    }
    
    if(scalar(keys %data) == 0){next;}


    #pattern vector
    my @patterns = keys %data;
    #loop over all patterns
    for(my $p = 0; $p < scalar@patterns; $p++){
	
	#open output
	my $out = $file;
	$out =~ s/gz/$patterns[$p].1cell.poptime/;
	open(OUT,">",$out);
	
	#init simulations;
	print STDERR "creating inital popuation\n";
	my @pop = ();
	foreach my $key (@patterns){push(@pop,0);};
	$pop[$p] = 1; # start simulation with one cell from only one pattern
	print STDERR "done\n";
	
	print STDERR "printing header\n";
	print OUT "gen";
	foreach my $pattern (@patterns){
	    print OUT "\t",$pattern,"\t",$pattern,"_rel";
	}
	print OUT "\n";
	print STDERR "done\n";

	my @org = @pop;
	print OUT "0";
	for(my $i = 0; $i < scalar@pop; $i++){
	    print OUT "\t",$pop[$i],"\t1";
	}
	print OUT "\n";
	
	print STDERR "init printed\n";
	#run 1000 steps
	my @last = @pop;
	my $sum = 1;
	for(my $i = 1; $i < 20; $i++){
	    print STDERR "$i $sum\n";
	    #set to zero
	    for(my $k = 0; $k < scalar@pop; $k++){$pop[$k] = 0;}
	    
	    #divide cells of each population
	    for(my $k = 0; $k < scalar@pop; $k++){
		my @cdstates = keys %{$data{$patterns[$k]}};
	#	if($last[$k] > 0){print STDERR "$last[$k] cells to populate with $patterns[$k]\n";}
		for(my $j = 0; $j < $last[$k]; $j++){
		    # select one cell division state
		    my $index = int(rand(scalar@cdstates));
		    # calculate inverse pattern
		    my $inv = $inverse{$patterns[$k]}->{$cdstates[$index]};
		    while(!exists $data{$patterns[$k]}->{$cdstates[$index]} 
			  || !exists  $data{$patterns[$k]}->{$inv}){
			print STDERR "recalc pattern it was: $cdstates[$index] and $inv for pattern $patterns[$k]\n";
                        # select one cell division state
			$index = int(rand(scalar@cdstates));
			# calculate inverse pattern
			$inv = $inverse{$patterns[$k]}->{$cdstates[$index]};
			
		    }
		    # calculate restore state
		    my $temp = int(rand(scalar@{$data{$patterns[$k]}->{$cdstates[$index]}}));
		    my $restore = $data{$patterns[$k]}->{$cdstates[$index]}->[$temp];

	#	    print STDERR "restore $cdstates[$index] to $restore\n";
		    # save cell in array
		    for(my $l = 0; $l < scalar@patterns; $l++){
			if($restore == $patterns[$l]){$pop[$l]++;last;}
		    }
		    
		    
		    #calculate restore state for inverse pattern
		    if(!exists $data{$patterns[$k]}->{$inv}){
			print STDERR "$inv is not in the list of cd patterns of $patterns[$k]\n";
		    }
		    $temp = int(rand(scalar@{$data{$patterns[$k]}->{$inv}}));
		    $restore = $data{$patterns[$k]}->{$inv}->[$temp];
	#	    print STDERR "restore $inv to $restore\n";
		    # save inverse cell in array
		    for(my $l = 0; $l < scalar@patterns; $l++){
			if($restore == $patterns[$l]){$pop[$l]++;last;}
		    }
		    
		    
		}
		
	    }
	    print OUT "$i";
	    $sum = 0;
	    for(my $k = 0; $k < scalar@pop; $k++){
	#	print STDERR $pop[$k],"\t";
		my $diff = 1;
		$sum += $pop[$k];
		if($org[$k] > 0){$diff=$org[$k]};
		print OUT "\t",$pop[$k],"\t",($pop[$k]/$diff);
	    }
	    if($sum == 0){$i = 10000;}
	    print OUT "\n";
	   # print STDERR "\n";
	    @last = @pop;
	}    
	close(OUT);
	print STDERR "$patterns[$p] done\n";
    }
}
