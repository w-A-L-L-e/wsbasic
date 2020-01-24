#!/usr/local/bin/wsbasic

pass = "change this"
url  = "http://walter.schreppers.com/set_ip.php"

cmd     = "echo \"pass="+pass+"\" | curl -post_data \""+url+"\""
result  = run( cmd )

test    = substr( result, 5,6 )

if test == "OK" 
  println "post successfull"
else
  println "post failed: ",result
  


