#!/usr/local/bin/wsbasic

# WARNING: this is currently broken as we dont have auto casting of int to string anymore
# this is because we don't want this for compile mode... we'll need to introduce some kind of casting op here...
count = run("ls -al | wc -l")
println "count=", count

# TODO: make count a number, we nees omething like casting (number) or make our parser
# of forloops smarter and auto convert or something, or throw an error at least...
count  = 18

for i=2 to count
  print(i)
  line=run("ls -al | head -n " + i + " | tail -n 1")
  print i-1,".\t", line
end

