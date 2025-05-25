if 1==1
  println ("ok 1")
end


if (1<=1)
  println("ok 2")
else
  println("whoops")
end



println("before loop 1")

for i=1 to 3
  print(i)
  
  if i==1 
    print("-> een ")
  else 
    if i==2 
      print("-> twee ")
    else 
      print("-> drie ")
    end
  end
  
  println ""
end

println("after loop 1")



println("before loop 2")

for i=1 to 3
  print(i)
  if i==1 
    print("-> een ")
  elsif i==2 
    print("-> twee ")
  else 
    print("-> drie ")
  end

  println ""
end

println("after loop 2")
