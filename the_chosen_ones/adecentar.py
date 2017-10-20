threshold = 14
base1= {}; base2= {}
psc= []

line= input()
d,n= [int(x) for x in line.split(" ")]
for i in range(n):
    line=input()
    psc.append([int(x) for x in line.split(" ")])
    for v in psc[-1]:
        if v< 14 and v not in base1: base1[v]= chr(ord('0')+len(base1))
        elif v>=14 and v not in base2: base2[v]= chr(ord('a')+len(base2))

bases= {**base1, **base2}

print(d,n)
trig1=[]; trig2=[]
for i in range(5):
    if i>0: print("")
    for facet in psc:
        if len(set(facet).intersection(base2.keys()))==i:
            print(*[bases[v] for v in facet])
            if(i==1): trig1.append(list(set(facet).intersection(base1)))
            if(i==4): trig2.append(list(set(facet).intersection(base2)))

print("")
print("base 1:")
for facet in trig1:
    print(*[bases[v] for v in facet])
print("")
for v1 in base1.keys():
    for v2 in base1.keys():
        if v2 > v1:
            suspension=True
            for facet in trig1:
                if v1 not in facet and v2 not in facet:
                    suspension=False
                    break

            if suspension: print("one point suspension: ", bases[v1], bases[v2])

print("")
print("base 2:")
for facet in trig2:
    print(*[bases[v] for v in facet])
print("")
for v1 in base2.keys():
    for v2 in base2.keys():
        if v2>v1:
            suspension=True
            for facet in trig2:
                if v1 not in facet and v2 not in facet:
                    suspension=False
                    break

            if suspension: print("one point suspension: ", bases[v1], bases[v2])



