averages={}
l=input()
while l!="":
	try:
	    val=int(l.split(" ")[2][1:-1])
	    key= l.split(":")[-1]
	    if key in averages: averages[key][0]+=1; averages[key][1]+=val
	    else: averages[key]=[1, val]
	    l= input()

	except EOFError: break

for key in averages:
	p=averages[key]
	print(key, p[1]/p[0], p[0])
  

