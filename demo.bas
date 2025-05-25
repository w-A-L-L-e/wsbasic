#!/usr/local/bin/wsbasic

def make_webrequest()
  url =  "https://ipinfo.io/ip"   
  result  = run("curl -s " + url)
  println "your public ip = ", result
end

make_webrequest()

println "simplest forloop example"
for i=1 to 5
  println i
end

