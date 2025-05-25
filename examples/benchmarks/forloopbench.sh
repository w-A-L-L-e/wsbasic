#!/bin/bash

for (( a=1; a <= 20000000; a++ )); do 
  b=$(( a*2 ));
done

echo "b=$b";

