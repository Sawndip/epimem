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
    my %init2count;
    my %initfp;

    -e $f || die "Could not find $f\n";
    open(F,"gunzip -c $f |");
    while(<F>){
	chomp;
	my ($or,$init,$fp) = split(/\s+/);
	if(exists $init2count{$init}){$init2count{$init}++;}
	else{$init2count{$init} = 1;}
	my $key = num2string($init,$states,$length)."_".num2string($fp,$states,$length);
	if(exists $initfp{$key}){
	    $initfp{$key}->[1]++;
	}else{
	    $initfp{$key} = [$init,1,cderrors($init,$fp,$states),noncderrors($init,$fp,$states)];
	}
    }
    close(F);
    foreach my $key (keys %initfp){
	#print STDERR "result\n";
	my ($init,$fp) = split(/_/,$key);
	my ($initint,$count,$cderr,$noncderr) = @{$initfp{$key}};
	$count /= $init2count{$initint};
	print "$key\t$init\t$fp\t$count\t$cderr\t$noncderr\n";
    }
}

