open(LOGFILE, ">output.txt") or die("Could not open log file.");
$w = 50;
$t = 140;
system("g++ file_read_write.cpp -o a.out");
system("./a.out ".$w." ".$t." 5");
system("g++ 3d_scheduling_4.cpp -o a.out");
$test = `./a.out 1`;
print LOGFILE $test;
