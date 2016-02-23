$w = $ARGV[0];
$t = $ARGV[1];
$h = $ARGV[2];
#for($w=5; $w <=50 ; $w=$w+5)
{
	system("g++ file_read_write.cpp -o a.out");
	system("./a.out ".$w." ".$t." 5 ".$h);
	system("g++ 3d_scheduling_4.cpp -o a.out");
	system("./a.out 1 res.txt");
}
