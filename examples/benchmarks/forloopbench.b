for a = 1 to 20000000  # 20 million iterations!
	b = a * 2
end

println "b=", b


# its actually still snappy for 20 million however there is plenty room for improvement
# time ../wsbasic forloopbench.b
# b=40000000.000000
# ../wsbasic forloopbench.b  1.84s user 0.00s system 99% cpu 1.841 total

# checking same loop with python 3.12 we see its a little faster but not even twice as fast:
#  Python 3.12.3
#
#  time python forloopbench.py
#  b= 40000000
#  python forloopbench.py  1.30s user 0.00s system 99% cpu 1.302 total


# good luck doing the same 20 million iterations in bash...
# I've actually found how you can do it, but it runs roughly 15 times slower:
#
# time bash forloopbench.sh
# b=40000000
# bash forloopbench.sh  30.86s user 0.00s system 99% cpu 30.870 total


# however if we take a look at one of the fastest small interpreters I know (lua) we see we
# could improve a lot:
# time lua forloopbench.lua
# b=	40000000
# lua forloopbench.lua  0.21s user 0.00s system 99% cpu 0.209 total
#
# so yeah, lua is almost 9 times faster than wsbasic, >6 times faster than python and ruby here...

# again node/javascript and nodejs that is a proper jit,  we can even beat this time by again a factor of 10:
# time node forloopbench.js
# b= 40000000
# node forloopbench.js  0.05s user 0.01s system 98% cpu 0.057 total

# incidently nodejs is so fast its approaching raw C speed here...

# still c is king, and look at this blazingly fast version:
# gcc forloopbench.c -o forloopbench
# time ./forloopbench                   
# b=40000000
# ./forloopbench  0.00s user 0.00s system 77% cpu 0.002 total
# but we're not counting compile time here. also notice cpu never got to 100% so 20 million is just not enough for c 
# to break a sweat :D
# again passiing -O2 here most likely totally optimizes away the entire loop as c compilers are way smart nowadays...
