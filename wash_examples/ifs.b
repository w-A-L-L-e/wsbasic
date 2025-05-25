#!/usr/local/bin/wsbasic

##########################
#simple if's without else
#
a=2
if (2<3)
  b=1
end

if a<3
  b=2
  b=b/2
end



####################
#if's with an else
#
if a>2 
  b=0
else
  b=5
end
  


if a>2
  b=6
  b=b*2
else
  b=2
  b=b*3
end

c=3

if c==1
  print "nee "
elsif c==3
  println "jaja!"
else
  println "hier moeten we nie zijn"
end


println b

a=1
if a==1 println "a is 1" end
if a>0  println "a>0" end
if 0<a  println "0<a" end
if a>=1 println "a>=1" end
if a<=1 println "a<=1" end
if a!=0 println "a!=0" end

#a=0
if a > 1 println "a>1" end
if a < 1 println "a<1" end
if a != 1 println "a not equal to 1" end

