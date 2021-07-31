#if 0
g++ -std=c++98 -O3 -o test_instrumented "$0" instrumented.cpp
exit;
#endif

#include "count_operations.h"

struct heap_sort_functor
{
  template <typename I> 
  // I is random-access iterator
  void operator()(I first, I last) const { std::partial_sort(first, last, last); }
};


int main() {  
  count_operations(16, 16 * 1024 * 1024, heap_sort_functor());

  count_operations(16, 16 * 1024 * 1024, heap_sort_functor(), normalized_by_nlogn1);
}
