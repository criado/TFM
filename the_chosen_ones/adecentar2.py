import sys

d,n = [int(s) for s in input().split(" ")]
print(d,n)

csp=[]
thevals="01234567abcdefgh"
thekeys=sys.argv[1]
permutation= {thekeys[i]:thevals[i] for i in range(len(thekeys))}
for f in range(n):
    l= input()
    while l=="": l=input(); print("")
    csp.append(sorted({ permutation[s] for s in l.split(" ")}))
    print(*csp[-1], sep=" ")


