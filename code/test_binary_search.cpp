#include <iostream>
#include "partition.h"

bool odd_p(int x) {
  return x % 2 == 1;
}

int main() {
  int a[] = { 1, 3, 7, 2, 4, 6 };
  size_t n = sizeof(a) / sizeof(int);
  int* b = a + n;

  std::cout << (true == is_partitioned(a, b, odd_p)) << std::endl;
  std::sort(a, b, std::less<int>());
  std::cout << (7 == *lower_bound(a, b, 7, std::less<int>())) << std::endl;
}

