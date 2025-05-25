i=2
while(i < 10000)
  prime = 1
  j=2
  while j < (i-1)
    if( (i % j) == 0 )
      prime=0
    end
    j=j+1
  end

  if prime == 1 
    print(i)
    print(" ")
  end

  i=i+1
end

print("\ndone")


