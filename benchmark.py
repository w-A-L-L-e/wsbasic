#!env python
i=2
while(i < 5000):
  prime = 1
  j=2
  while j < (i-1):
    if( (i % j) == 0 ):
      prime=0
    j=j+1

  if prime == 1:
    print(i, end=" ", flush=True)

  i=i+1

print("\ndone\n")
