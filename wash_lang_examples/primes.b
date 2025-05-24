#!/usr/local/bin/wsbasic


for i=0 to 500
  prime = 1

  for j=2 to i-1
    if( i % j == 0 )
      prime=0
    end
  end

  if prime==1 
    print(i)
    print(" ")
  end

end

print("done")


