#!/usr/bin/perl


sub noncderrors{
    my $child = shift;
    my $parent = shift;
    my $states = shift;
    my $err = 0;
    while($child > 0 || $parent > 0){
	my $c = $child%$states;
	$child = int($child/$states);
	my $p = $parent%$states;
	$parent = int($parent/$states);
	if($p == 0 && $c > 0){$err++};
    }
    return $err;
}

sub cderrors{
    my $child = shift;
    my $parent = shift;
    my $states = shift;
    my $err = 0;
    #print STDERR $states,"\n";
    while($child > 0 || $parent > 0){
	my $c = $child%$states;
	$child = int($child/$states);
	my $p = $parent%$states;
	$parent = int($parent/$states);
	if($p > 0 && $c == 0){$err++};
    }
    return $err;
}

sub num2string{
    my $string = "";
    my $num = shift;
    my $states = shift;
    my $length = shift;
    #print STDERR $states,"\n";
    while($length > 0){
	$string .= "".($num % $states);
	$num = int($num/$states);
	$length--;
    }
    return $string;
}

scalar@ARGV > 2 || die "usage: perl hamming2restorerate.pl <states> <length> <files>\n";
my $states = shift@ARGV;
my $length = shift@ARGV;
my @files = @ARGV;

foreach my $f (@files){
    my %or2count;
    my %orfp;

    -e $f || die "Could not find $f\n";
    open(F,"gunzip -c $f |");
    while(<F>){
	chomp;
	my ($or,$init,$fp) = split(/\s+/);
	if(exists $or2count{$or}){$or2count{$or}++;}
	else{$or2count{$or} = 1;}
	my $key = num2string($or,$states,$length)."_".num2string($fp,$states,$length);
	if(exists $orfp{$key}){
	    $orfp{$key}->[1]++;
	}else{
	    $orfp{$key} = [$or,1,cderrors($or,$fp,$states),noncderrors($or,$fp,$states)];
	}
    }
    close(F);
    foreach my $key (keys %orfp){
	#print STDERR "result\n";
	my ($or,$fp) = split(/_/,$key);
	my ($orint,$count,$cderr,$noncderr) = @{$orfp{$key}};
	$count /= $or2count{$orint};
	print "$key\t$or\t$fp\t$count\t$cderr\t$noncderr\n";
    }
}

