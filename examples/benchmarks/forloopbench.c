#include <stdio.h>

int main() {

  long b;
  for (long a = 1; a <= 20000000; a++) {
    b = a * 2;
  }

  printf("b=%ld\n", b);
}
