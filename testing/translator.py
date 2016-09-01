#! /bin/python

if __name__=="__main__":
  line=input().split(" ")
  dim=int(line[0]); facets=int(line[1])
  d={}; base1=set(); base2=set()

  for i in range(facets):
    line=input().split(" ")
    for j in range(dim):
      k=int(line[j])
      if k in d: k=d[k]
      else:
        d[k]=len(d)
        if k<14: base1.add(d[k])
        else:    base2.add(d[k])
        k=d[k]

      print(k, end=" ")
    print("")
  for v in base1:
    print(v,end=" ")
  print("")
  for v in base2:
    print(v,end=" ")
  print("")




  
