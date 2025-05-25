#!/usr/local/bin/wsbasic

begin

for i=0 to 500 begin
  prime = 1

  for j=2 to i-1
    if( i % j == 0 )
      prime=0

  if prime==1 
  begin
    print(i)
    print(" ")
  end

end

print("done")

end

