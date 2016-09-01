import sys
for l in sys.stdin:
  line=l.replace("&","\&")
  line=line.replace("_","\_")
  line=line.replace("#","\#")
  print(line[:-1])

