# this now parses, but there is a bug somewhere so output is not same as before
# its something simple most likely where more newlines are printed then we want

def mandelbrot(real, imag)
  limit = 99 
  zReal = real
  zImag = imag
  r2 = 0
  i2 = 0
  c4 = 4
  
  for i=0 to limit
    r2 = zReal * zReal
    i2 = zImag * zImag
    
    if (r2 + i2 > c4)
      return i
    end
    
    zImag = 2.0 * zReal * zImag + imag
    zReal = r2 - i2 + real
  end
  
  return limit+1
end


#### main routine ###

width = 80  # number of characters on one line
height = 42  # number of chars vertically

x_start = -0.33
x_fin = -0.055
y_start = -0.95
y_fin = -0.82


dx = (x_fin - x_start)/(width-1)
dy = (y_fin - y_start)/(height-1)

block_char  = "\u2588"
black       = "\033[22;30m"
red         = "\033[22;31m"
l_red       = "\033[01;31m"
green       = "\033[22;32m"
l_green     = "\033[01;32m"
orange      = "\033[22;33m"
yellow      = "\033[01;33m"
blue        = "\033[22;34m"
l_blue      = "\033[01;34m"
magenta     = "\033[22;35m"
l_magenta   = "\033[01;35m"
cyan        = "\033[22;36m"
l_cyan      = "\033[01;36m"
gray        = "\033[22;37m"
white       = "\033[01;37m"


for i=0 to height

  for j=0 to width
    x = x_start + j*dx  # current real value
    y = y_fin - i*dy    # current imaginary value
			
    value = mandelbrot(x,y)
    
    if (value == 100)     print " "
    elsif (value > 90)  print red, block_char
    elsif (value > 70)  print l_red, block_char
    elsif (value > 50)  print orange, block_char
    elsif (value > 30)  print yellow, block_char
    elsif (value > 20)  print l_green, block_char
    elsif (value > 10)  print green, block_char
    elsif (value > 5)   print l_cyan, block_char
    elsif (value > 4)   print cyan, block_char
    elsif (value > 3)   print l_blue, block_char
    elsif (value > 2)   print blue, block_char
    elsif (value > 1)   print magenta, block_char
    else                  
      print l_magenta, block_char
    end 
    
    print "\033[0m"  # reset colors
  end

  println ""
end

