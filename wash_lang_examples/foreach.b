#!/usr/local/bin/wsbasic

foreach word in "hello little world!" seperated by " "
  println "'", word ,"'"
end


foreach word in "hello little world!" seperated by " "
  if word!="little"
    print word+" "
  end
end
println ""
