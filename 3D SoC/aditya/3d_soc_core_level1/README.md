
1. Combine core wise wrapper files containing test times for each core (core_level_testtime.txt/d695.txt ...). Combine all files of all dies in the order of stacking (topmost in the file is lowest in the stack) - (sic_level_1.txt.. the 3D/SIC1 folder)

2. Run file_read_write.cpp to generate tam_testtime.txt and iolist.h

3. Run 3d_scheduling_4.cpp