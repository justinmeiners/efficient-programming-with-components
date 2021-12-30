#include <iostream>
#include <functional>
#include <algorithm>
#include "binary_counter.h"

template <typename I>
void show_counter(I first, I last)
{
  while (first != last) {
    std::cout << (*first) << " ";
    ++first;
  }
  std::cout << std::endl;
}

template<typename T>
struct min_op {
  T operator()(const T& a, const T& b) {
    return std::min(a, b);
  }
};

int main() {
  char letters[] = { 'C', 'H', 'B',  'F', 'I', 'D', 'E', 'G', 'A', 'J' };

  char* first = letters;
  char* last = letters + (sizeof(letters) / sizeof(char));

  typedef min_op<char> op_type;

  binary_counter<op_type, char> counter(op_type(), '_');

  while (first != last) {
    std::cout << "add: " << *first << std::endl;
    counter.add(*first);
    show_counter(counter.counter.begin(), counter.counter.end());
    ++first;
  }
  std::cout << "min (after reduce): " << counter.reduce() << std::endl;
}
