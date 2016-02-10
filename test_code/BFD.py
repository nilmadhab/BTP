
# Soc d695

import os
import MySQLdb
from MySQLdb import  Error

arr = []


db = MySQLdb.connect("localhost","root","25011994","BTP" )

cursor = db.cursor()

sql = "select * from wrapper_design where soc_name ='%s'" %('d695.soc');

#print sql

cursor.execute(sql)

data = cursor.fetchall()

for row in data :
	
	file_name = row[2:]
	
	arr.append(file_name)


# for x in arr:
# 	print x

#no_input, no_output, no_test_pattern, core_index, no_scan_chain,scan chains
'''
Algorithm:
Wi = number of TAMs for core i :ok
Lines= int Wi / 2 : ok
#SCi = number of scan chains of core i : ok
Sort the scan chains for core i in decreasing order in length : ok
Select the longest scan chains as the (Lines) lines 
While (#SCi > Lines)
Chain the shortest line with shortest scan chain
#SCi = #SCi - 1
Update the length of the longest scan length
Sort scan chain in decreasing order
Add the Fini and Fouti to balance the scan chains
i. max Si = number of Fini + longest wrapper scan chain
ii. max So = number of Fouti + longest wrapper scan chain
iii. min Si = number of Fini + shortest wrapper scan chain
iv. min So = number of Fouti + shortest wrapper scan chain
Ti = { (1+ max ( Si, So) TP) + min( Si, So)}
'''
for core in arr:
	#print "tam ", tam
	test_time = []
	print "core" , int(core[3])
	
	if int(core[3]) == 0:
			continue
	# if int(core[3]) <= 8:
	# 		continue
	# if int(core[3]) > 10:
	# 		continue
	for tam in range(2, 129):
		no_input = int(core[0])
		no_output = int(core[1])
		no_test_pattern = int(core[2])
		core_index = int(core[3])
		
		no_scan_chain = int(core[4])
		scan_chains = core[5].split(" ")
		for i in range(no_scan_chain):
			scan_chains[i] = int(scan_chains[i])
		scan_chains.sort()
		scan_chains.reverse()
		#print " core ", core_index
		Wi = tam
		Lines = Wi/2
		#print "no input",no_input, no_output, no_test_pattern, no_scan_chain, scan_chains,"Lines", Lines

		wrapper_chains = [0]*Lines

		for i in range(min(Lines,no_scan_chain)): # fill as much as you can fill
			wrapper_chains[i] = scan_chains[i]


		while no_scan_chain > Lines:
			out = scan_chains.pop(no_scan_chain-1)
			min_inx = wrapper_chains.index(min(wrapper_chains))
			wrapper_chains[min_inx] += out
			no_scan_chain -=1
		wrapper_chains.sort()
		wrapper_chains.reverse()
		#print wrapper_chains

		# now we have to distribute input and output pins
		# to minimize (1 + max(si,s0))*TP + min(si,so)
		keep_copy = list(wrapper_chains)
		"""
		Lets get si max
		"""
		distribute =  no_input #max(no_input,no_output) # lets get max(si,so) term
		ind = 1
		while distribute  and ind < Lines:
			balance = (wrapper_chains[0]-wrapper_chains[ind])
			if distribute >= balance:
				wrapper_chains[ind] +=  balance
				distribute -= balance
			else:
				wrapper_chains[ind] += distribute
				distribute = 0
			ind +=1
		#print " after distributing input , balance"
		#print wrapper_chains, distribute

		max_input_term = max(wrapper_chains)
		if distribute:
			max_input_term += distribute/Lines 
			distribute = distribute - (distribute/Lines)*Lines
		if distribute:
			max_input_term +=1
		# now get the min(si,so) term
		# restore the chains
		"""
		Lets get so max
		"""

		wrapper_chains = keep_copy

		distribute = no_output# min(no_output,no_output)
		ind = 1
		while distribute and ind < Lines:
			balance = (wrapper_chains[0]-wrapper_chains[ind])
			if distribute >= balance:
				wrapper_chains[ind] +=  balance
				distribute -= balance
			else:
				wrapper_chains[ind] += distribute
				distribute = 0
			ind +=1

		max_output_term = max(wrapper_chains)

		#print " after distributing output , balance"
		#print wrapper_chains, distribute

		if distribute:
			max_output_term += distribute/Lines 
			distribute = distribute - (distribute/Lines)*Lines

		if distribute:
			max_output_term +=1
					


		max_term = max(max_output_term,max_input_term)
		min_term = min(max_input_term,max_output_term)
		T = (1 + max_term)*no_test_pattern + min_term


			
		#print max_term, min_term, core_index , "time", T
		#print Lines,T#,core_index
		#break

		T1 = str(T)
		core_index1 = str(core_index)
		tam1 = str(tam)
		docs = 'd695.soc'
		sql = """INSERT INTO wrapper_output	(soc_name,tam_width,test_time,core_index)
		 VALUES ('"""+docs+"""','"""+tam1+"""','"""+T1+"""','"""+core_index1	+"""')"""
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
		

		#print " test time for core ", core_index, "for no chains ", Lines , "**************	"
		#print T

		

	



