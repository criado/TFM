from sympy import *
from queue import Queue

def toNumber(s):
    if s.isalpha(): return ord(s)-ord('a')+7
    else: return ord(s)-ord('0')

def tochar(a):
    if a<7: return chr(a+ord('A'))
    else: return chr(a-7+ord('a'))

coordinates=[
        [1.0, 1.0, None, None, None, None],
        [1.0, 1.0, None, None, None, None],
        [1.0, 1.0, None, None, None, None],
        [1.0, 1.0, 0.0, 0.0, 1.0, 0.0],
        [1.0, 1.0, 0.0, 0.0, 0.0, 0.0],
        [1.0, 1.0, 0.0, 0.0, 0.0, 1.0],
        [1.0, 1.0, None, None, None, None],

        [0.0, 1.0, None, None, None, None],
        [0.0, 1.0, None, None, None, None],
        [0.0, 1.0, None, None, None, None],
        [0.0, 1.0, 1.0, 0.0, 0.0, 0.0],
        [0.0, 1.0, 0.0, 0.0, 0.0, 0.0],
        [0.0, 1.0, None, None, None, None],
        [0.0, 1.0, None, None, None, None]]

for i in range(len(coordinates)):
    for j in range(6):
        if coordinates[i][j] is None:
            coordinates[i][j]= Symbol(tochar(i)+"_"+str(j))

PSC=[]
thefile=open("sol1963dd")
d,n= thefile.readline()[:-1].split(" ")

for l in thefile:
    if l=="\n": continue
    PSC.append([toNumber(a) for a in l[:-1].split(" ")])

orientations={0:1.0}
visited= {0}
q= Queue()
q.put(0)

while not q.empty():
    i= q.get(); f=PSC[i]
    for j,f2 in enumerate(PSC):
        if len(set(f)&set(f2))== 4:
            if j not in visited:
                visited.add(j)
                q.put(j)

                first= list(set(f)-set(f2))[0]
                second= list(set(f2)-set(f))[0]
                if first>second:
                    first,second=second,first
                intermediate= len([x for x in sorted(set(f)|set(f2)) if x>first and x<second])
                orientations[j]= -orientations[i] if intermediate%2==0 else orientations[i]

print("Maximize")
print(" obj: 0")
print("Subject to")
base1={toNumber(s) for s in list("0123456")}
base2={toNumber(s) for s in list("abcdefg")}
for i, f in enumerate(PSC):
    if len(set(f)&base1)==4 or len(set(f)&base2)==4:
        for v in range(14):
            if v not in f:
                print(" eq"+str(i)+tochar(v),end=": ")
                l=[coordinates[j] for j in f]
                l.append(coordinates[v])
                m=Matrix(l)
                print(orientations[i]*m.det(), end="")
                print(" - 1e-4 >= 0")

