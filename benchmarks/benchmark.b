#!/usr/local/bin/wsbasic

i=2

while(i < 10000)
begin
  prime = 1
  j=2
  while j < (i-1)
  begin
    if( (i % j) == 0 )
    begin
      prime=0
    end
    j=j+1
  end

  if prime == 1 
  begin
    print i, " "
  end

  i=i+1
end

println("\ndone")
