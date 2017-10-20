
d,n = [int(s) for s in input().split(" ")]
print(d,n)

csp=[]
permutation= {'1':'0', '2':'1', '5':'2', '6':'3', '7':'4', '3':'5', '0':'6', '4':'7',
        'a':'a', 'c':'b', 'f':'c', 'd':'d', 'b':'e', 'g':'f', 'h':'g', 'e':'h'}
for f in range(n):
    l= input()
    while l=="": l=input(); print("")
    csp.append(sorted({ permutation[s] for s in l.split(" ")}))
    print(*csp[-1], sep=" ")


