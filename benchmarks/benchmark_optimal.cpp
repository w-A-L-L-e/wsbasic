#include <iostream>
#include <string>

int main() {
  std::cout << std::fixed;
  double i = 2;
  while (i < 10000) {
    double prime = 1;
    double j = 2;
    while (j < i - 1) {
      if ((long)i % (long)j == 0) {
        prime = 0;
      }

      j = j + 1;
    }

    if (prime == 1.0) {
      std::cout << i;
      std::cout << " ";
    }

    i = i + double(1.000000);
  }

  std::cout << "\ndone" << std::endl;
  return 0;
}
