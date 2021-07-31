#include "count_operations.h"
#include <set>

struct set_functor
{
  template <typename I> 
  // I is random-access iterator
  int operator()(I first, I last) const {
    std::set< typename std::iterator_traits<I>::value_type > a(first, last);
    return a.size();
  }
};


struct unique_functor
{
  template <typename I> 
  // I is random-access iterator
  int operator()(I first, I last) const {
    std::sort(first, last);
    return std::unique(first, last) - first;
  }
};



int main() {  
  std::cout << "unique and sort" << std::endl;
  count_operations(16, 16 * 1024 * 1024, unique_functor());
  std::cout << "set sort" << std::endl;
  count_operations(16, 16 * 1024 * 1024, set_functor());
}
