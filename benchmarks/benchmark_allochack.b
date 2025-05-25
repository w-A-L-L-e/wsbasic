#!/usr/local/bin/wsbasic

# hack, we just pre-assign the constants
# this avoids a lot of copying of var objects...
# by doing this we get twice the speed on same 10k run
c5 = 10000
c1 = 1
c2 = 2
cz = 0

# also bring out j and prime's first assign outside of loop
i=2
prime=0
j=0

while(i < c5)
  prime = c1
  j = c2
  while j < (i-c1)
    if( (i % j) == cz )
      prime = cz
    end
    j = j + c1
  end

  if prime == c1 
    print(i)
    print(" ")
  end

  i=i + c1
end

println("\ndone")
