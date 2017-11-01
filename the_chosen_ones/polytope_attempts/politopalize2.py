from sympy import *
from queue import Queue

def toNumber(s):
    if s.isalpha(): return ord(s)-ord('a')+7
    else: return ord(s)-ord('0')

def tochar(a):
    if a<7: return chr(a+ord('A'))
    else: return chr(a-7+ord('a'))

coordinates=[
        [None, None, None, None, 1.0, 1.0],
        [None, None, None, None, 1.0, 1.0],
        [None, None, None, None, 1.0, 1.0],
        [None, None, None, None, 1.0, 1.0],
        [None, None, None, None, 1.0, 1.0],
        [None, None, None, None, 1.0, 1.0],
        [None, None, None, None, 1.0, 1.0],

        [None, None, None, None, 0.0, 1.0],
        [None, None, None, None, 0.0, 1.0],
        [None, None, None, None, 0.0, 1.0],
        [None, None, None, None, 0.0, 1.0],
        [None, None, None, None, 0.0, 1.0],
        [None, None, None, None, 0.0, 1.0],
        [None, None, None, None, 0.0, 1.0]]

for i in range(len(coordinates)):
    for j in range(6):
        if coordinates[i][j] is None:
            coordinates[i][j]= Symbol(tochar(i)+"_"+str(j))

PSC=[]
thefile=open("sol1963_skeleton")
d,n= thefile.readline()[:-1].split(" ")

for l in thefile:
    if l=="\n": continue
    PSC.append([toNumber(a) for a in l[:-1].split(" ")])

print("Maximize")
print(" obj: e")
print("Subject to")
for i, f in enumerate(PSC):
    thefacet=[Symbol("f"+str(i)+"_"+str(j)) for j in range(6)]
    print(" norm"+ str(i), end=": ")
    print( sum(thefacet[i]*thefacet[i] for i in range(5)), end="")
    print(" == 1")

    for v in range(14):
        print(" eq"+str(i)+tochar(v), end=": ")
        expr=sum(thefacet[i]*coordinates[v][i] for i in range(6))
        print(expr, end="")

        if v in f:
            print(" - e >= 0")
        else: print(" + e <= 0")

print("Bounds")
for i in range(len(coordinates)):
    for j in range(4):
        print(" -1 <= "+str(coordinates[i][j])+" <= 1")

for i in range(len(PSC)):
    for j in range(5):
        print(" -1 <= "+"f"+str(i)+"_"+str(j)+" <= 1")
    print(" -2.24 <= "+"f"+str(i)+"_5 <= 1")
