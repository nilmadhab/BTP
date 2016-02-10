use lib '../../Parallel-ForkManager/lib';
use Parallel::ForkManager;
#use File::chdir; 
use Data::Dumper;

# $output = "power_die_testime.txt" or die("Could not open log file.");
# open (MYFILE, '>'.$output);
$tsv = $ARGV[0];
$power = $ARGV[1];
# my %testtime;
my $pm = new Parallel::ForkManager(2);
  $pm -> run_on_finish (
    sub {
      my ($pid, $exit_code, $ident, $exit_signal, $core_dump, $data_structure_reference) = @_;

      if (defined($data_structure_reference)) {  
        my $data = $$data_structure_reference;
        # $testtime{$data[0]} = $data[1];
      } else {
        print qq|ident "$ident" did not send anything.\n\n|;
      }
    }
  );
for($w=15;$w<=50;$w=$w+5)
{
	#print MYFILE $w." ";
	#print $w." ";
	#for($p=$power_min;$p<=$power_max;$p=$p+200)
	{
		
		# for($i=$p;$i<$p+200;$i=$i+50)
		{
			my $pid = $pm->start and next; 
			system("mkdir $w");
			system("cp file_read_write.cpp $w/");
			system("cp 3d_scheduling_4.cpp $w/");
			system("cp sic_* $w/");
			system("cp generate.pl $w/");
			#system("cp -r ../../File-chdir $w/");
			my $testtime = 0;
			system("perl ".$w."/generate.pl ".$w." ".$tsv." 0 ".$power);
			#`perl $w/generate.pl $w $tsv 0 $power`;
			#system("rm -r $w");
			# my $temp = ($i,$testtime);
			$pm->finish(0,\$testtime);
			
		}
		

		# for (sort keys %testtime) {
		#    print MYFILE "$testtime{$_} ";
		#    print "$testtime{$_} ";
		#  }
		
		
	}

	# print MYFILE "\n";
	# print "\n";
}
$pm->wait_all_children;