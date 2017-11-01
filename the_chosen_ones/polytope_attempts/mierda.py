from pyhull import qconvex
from queue import *
from math import *

N= 30

def list_to_mask(l):
    return sum(2**i for i in l)
def mask_to_list(m):
    thelist= []; power=0
    while m!=0:
        if m%(2**(power+1))!=0: m-=2**power; thelist+=[power]
        power+=1
    return thelist


for i in range(N):
    for j in range(N//2):
        for k in range(N):
            p1=[-1.0+(i+1.0)*2.0/(N+1.0), 1.0]
            p2=[-1.0+(k+1.0)*2.0/(N+1.0), (j+1.0)/(N//2+1.0)]
            #print(p1)
            #print(p2)
            if p1==p2: continue

            thepoints= [[-1.0, 0.0, -1.0, 0.0, 1.0],
                        [0.5, sqrt(3.0)/2.0, -1.0, 0.0, 1.0],
                        [0.5, -sqrt(3.0)/2.0, -1.0, 0.0, 1.0],
                        [0.0, 0.0, 1.0, 0.0, 1.0],
                        [0.0, 0.0, p1[0], p1[1], 1.0],
                        [0.0, 0.0, p1[0], -p1[1], 1.0],
                        [0.0, 0.0, p2[0], p2[1], 1.0],
                        [0.0, 0.0, p2[0], -p2[1], 1.0]
                        ]
            thepoints+=[[p[2],-p[3],p[0],p[1], -1.0] for p in thepoints]

            #print(*thepoints, sep='\n')
            thefacets= [list_to_mask([int(s) for s in l.split(" ")[1:]]) for l in qconvex("Fv", thepoints)[1:]]
            #print(i,j,k)
            if list_to_mask(range(8)) not in thefacets: continue
            #print(*[mask_to_list(m) for m in thefacets], sep='\n')
            if len([l for l in [len(mask_to_list(f)) for f in thefacets] if l is not 5]) is not 2: continue
            #print(*[mask_to_list(f) for f in thefacets], sep='\n')

            distances={list_to_mask(list(range(8))): 0}
            visited=set([list_to_mask(range(8))])
            q= Queue()
            q.put(list_to_mask(list(range(8))))

            while not q.empty():
                f= q.get()
                for f2 in thefacets:
                    if len(set(mask_to_list(f))&set(mask_to_list(f2)))==4:
                        if f2 not in visited:
                            distances[f2]=distances[f]+1
                            visited.add(f2)
                            q.put(f2)
            #print(*[(mask_to_list(k), distances[k]) for k in distances.keys()], sep='\n')
            #a=0/0

            index= open("index","a")
            print(i,j,k, p1, p2)
            print(i,j,k, p1, p2, file=index)
            print("distance:", distances[list_to_mask(range(8,16))])
            print("distance:", distances[list_to_mask(range(8,16))], file=index)
            index.close







