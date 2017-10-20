
d,n=input().split(" ")
print(d,n)
for f in range(int(n)):
    l=input()
    while l=="": l=input()

    print(*[ord(v)-ord('0') if v<'a' else 14+ord(v)-ord('a') for v in l.split(" ")])


