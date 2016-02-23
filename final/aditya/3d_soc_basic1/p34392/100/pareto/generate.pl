use POSIX;
use File::Path;
use Data::Dumper;

open(LOGFILE, ">die_testtime.txt") or die("Could not open log file.");
for($w = 5; $w<=100; $w=$w+1)
{
	system("g++ file_read_write.cpp -o a.out");
	system("./a.out ".$w." 19");
	system("g++ new_scheduling_4.cpp -o a.out");
	$test = `./a.out $w.txt 1`;
	print LOGFILE $w." ".$test."\n";
	print $w." ".$test."\n";

}