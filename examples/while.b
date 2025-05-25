#!/usr/local/bin/wsbasic
####################
# inline is not supported anymore
# to get cleaner code a while is always a block and
# needs an end statement
#
i=0
while i+1<5*200
  i=i+1 
end

println "i after first while = ",i 

#################
#while with block (here begin is now not needed anymore)
#
i=1
while i<10*2 
  i=i+1
  print "i=", i, " "
  println "i+1=",i+1
end
