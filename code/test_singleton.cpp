#include "singleton.h"
#include <iostream>

typedef singleton<int> si;

int main() {
  si x(1);
  si y(2);

  std::cout << "x == x: " << (x == x) << std::endl;
  std::cout << "x < y: " << (x < y) << std::endl;
}

