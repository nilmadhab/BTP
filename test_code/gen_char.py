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
	    #for x in lines:
	    	#print x
	    no_mod_line =  lines[1]

	    no_mod = no_mod_line.split(" ")[1]
	    no_mod = no_mod
	    print no_mod , url

	    no_levels = 2
	    no_levels = str(no_levels)
	    
	    for line in lines:
	    	all_lines.append(line)



	#print "all_lines ",all_lines    
	no_modules = int(no_mod)
	max_lev = 1
	for x in range(1,len(all_lines)):
		#print all_lines[x]
		if all_lines[x-1].strip("\n") == '':
			#print all_lines[x]
			info_per_mod = all_lines[x].strip().split(" ")
			#print info_per_mod
			try:
				max_lev = max(max_lev,int(info_per_mod[3]))
			except:
				max_lev = max_lev

	max_lev +=1
	print url,max_lev
	max_lev = str(max_lev)
	sql = """INSERT INTO gen_char(soc_name,modules,levels) VALUES ('"""+docs+"""','"""+no_mod+"""','"""+max_lev+"""')"""
	print sql
	try:
	   # Execute the SQL command
	   cursor.execute(sql)
	   # Commit your changes in the database
	   db.commit()
	except Error as error:
	   # Rollback in case there is any error
	   print error
	   db.rollback()

	
