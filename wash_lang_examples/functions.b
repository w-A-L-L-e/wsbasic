#!/usr/local/bin/wsbasic


#function without parameters
def hello()
  println "hello there!"
end

#function with parameters
def test(a,b,c)
  println "a=", a, ", b=", b, ", c=", c
  println "a+b+c= ", a+b+c
  println ""
end


#function that returns something
def square(i)
  return i*i
end


def fac(i)
  if(i<=1) begin
    return 1
  end
  else begin
    return i*fac(i-1)
  end
end

glob=5
hello()
test(1,2,3)

j=1
while j<10 
  println "fac(",j,")=",fac(j)
  j=j+1
end

println ""
println "glob        =" , glob
println "square(glob)=" , square(glob) 

