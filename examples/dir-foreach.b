#!/usr/local/bin/wsbasic


###################################
#foreach met default seperator = \n
#
i=0
foreach line in run("ls -al")
  i=i+1
  println i,".\t", line
end


print "\n\nnow seperate on '.b'\n"

#################################
#foreach with different seperator
#
i=0
foreach line in run("ls -al") seperated by ".b\n"
  i=i+1
  println i,".\t", line
end

