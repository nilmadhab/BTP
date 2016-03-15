v = [ 1.0,0.9,0.8,0.7]

alpha = 1.3


vth = 0.5


f0 = pow((1-.5),alpha)/1.00




f = []
for x in v:
	f.append(pow(x-vth,alpha)/x)

for x in range(len(f)):
	print round(f[x]/f0,2),

print 


for x in range(len(f)):
	f[x] = round((f[x]/f0)*120,2)
	
print f

k = [120,100,80,60]

for x in k:
	print round(x/120.0,2) ,
print 

