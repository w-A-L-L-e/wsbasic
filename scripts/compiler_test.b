
def add(a,b)
begin
  println(a+b)
  # return a+b  # return is not implemented in compiler mode yet
end


println("Start")
add(2,3)
# x=add(40,2)


x=0
while (x < 12) 
begin
  println(x)
  x = x + 2
end

println( 3.1 + 4*2 + 6/2 ) # this should be 3 + 8 + 3 == 14.1

expr = 3.1 + 4*2 + 6/2 

if( expr > 15 )
  println(1.1)
else
  println(2.1)


# assigning strings does not work yet with the compilers
# question = ""

# again in executor mode we can leave out the question=0. so our input needs to also
# check against symtable here in order to know if its a new var or not...
question = 0.0
print("type someting here:")
input question
print("your answer:")
println(question)

exit(x)

