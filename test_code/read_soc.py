import os
import MySQLdb
from MySQLdb import  Error
os.chdir("/home/nilmadhab/Desktop/7th_sem/BTP/test_code/benchm")
db = MySQLdb.connect("localhost","root","25011994","BTP" )

cursor = db.cursor()


for file in os.listdir("."):
	sql = """INSERT INTO list_socs(soc_name) 
 	VALUES ('"""+file+"""')"""
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
	print file




# disconnect from server
db.close()