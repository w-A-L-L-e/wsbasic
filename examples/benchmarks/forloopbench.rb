for a in 1..20_000_000 do
  b = a * 2
end

puts 'b=', b

# actually a little faster than the python version :D
#
# ruby 3.4.4 (2025-05-14 revision a38531fd3f) +PRISM [x86_64-linux]
#
# time ruby forloopbench.rb
# b=
# 40000000
# ruby forloopbench.rb  1.24s user 0.01s system 99% cpu 1.253 total
