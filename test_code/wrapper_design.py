import os
import MySQLdb
from MySQLdb import  Error

document = "p93791.soc"
url = "benchm/"+document


arr = []
import MySQLdb

db = MySQLdb.connect("localhost","root","25011994","BTP" )

cursor = db.cursor()

sql = "select * from list_socs";

cursor.execute(sql)

data = cursor.fetchall()

for row in data :
	
	file_name = row[1]
	
	arr.append(file_name)
print arr

for docs in arr:
	all_lines = []
	url = "benchm/"+docs
	print docs
	with open(url) as f:
	    lines = f.readlines()

	    
	    for line in lines:
	    	all_lines.append(line)
	count = 0
	for x in range(1,len(all_lines)):
		#print all_lines[x]
		if all_lines[x-1].strip("\n") == '':
			print all_lines[x]
			info_per_mod = all_lines[x].strip().split(" ")
			print info_per_mod,docs
			no_input = info_per_mod[5]
			no_output = info_per_mod[7]
			no_scan_chain = info_per_mod[11]
			scan_chains = ' '.join(info_per_mod[13:])
			no_test_pattern = all_lines[x+2].strip().split(" ")[-1]
			print no_scan_chain,scan_chains

			core_index = str(count)

	


			# sql = """INSERT INTO wrapper_design(soc_name,no_input,no_output,no_test_pattern,core_index,no_scan_chain,scan_chains)
			#  VALUES ('"""+docs+"""','"""+no_input+"""','"""+no_output+"""','"""+no_test_pattern+"""','"""+core_index+"""','"""+no_scan_chain+"""','"""+scan_chains+"""')"""
			# print sql
			# try:
			#    # Execute the SQL command
			#    cursor.execute(sql)
			#    # Commit your changes in the database
			#    db.commit()
			# except Error as error:
			#    # Rollback in case there is any error
			#    print error
			#    db.rollback()

			count +=1

	
