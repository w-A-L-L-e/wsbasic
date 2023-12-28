#include "benchmark.h"
#include <iostream>
#include <string>

int main() {
  std::cout << std::fixed;
  long i = 2;
  while (i < 10000) {
    long prime = 1;
    long j = 2;
    while (j < i - 1) {
      if (i % j == 0) {
        prime = 0;
      }

      j = j + 1;
    }

    if (prime == 1.0) {
      std::cout << i;
      std::cout << " ";
    }

    i = i + long(1.000000);
  }

  std::cout << "\ndone" << std::endl;
  return 0;
}
