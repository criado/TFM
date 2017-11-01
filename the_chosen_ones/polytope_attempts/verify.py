from pyhull import qconvex
from queue import *
from math import *

thefile= open("prismatoid14")
d,n=[int(s) for s in thefile.readline()[:-1].split(" ")]

points=[]
for line in thefile:
    points.append([float(s) for s in line[:-1].split(" ")])

PSC= [ [int(s) for s in line.split(" ")[1:]] for line in qconvex("Fv", points)[1:]]
base1, base2= [ i for i in range(len(PSC)) if len(PSC[i])==7]

distances= {base1: 0}
q= Queue()
q.put(base1)


while not q.empty():
    i= q.get(); f=PSC[i]
    for j, f2 in enumerate(PSC):
        if len(set(f) & set(f2)) ==4:
            if j not in distances.keys():
                distances[j]= distances[i]+1
                q.put(j)

print("Width:", distances[base2])
