#include <iostream>
#include <algorithm>
#include <set>

int main() {
  int a[] = { 1, 3, 1, 4, 1, 5 };

  std::set<int> set_of_ints(a, a + 6);
  std::cout << set_of_ints.size() << std::endl;

  std::sort(a, a + 6);
  std::cout << std::unique(a, a + 6) - a << std::endl;
}
