#include <functional>
#include <iostream>
#include <cassert>
#include "insertion_sort.h"
#include "list_pool.h"
#include "algorithm.h"

int main() {

  // test binary insertion sort
  {
      int nums[] = {5, 3, 10, 1, 2};
      binary_insertion_sort_n(nums, 5, std::less<int>());
      print_range(nums, nums + 5);
      assert(is_sorted(nums, nums + 5));
  }
  // test linear insertion sort
  {
      int nums[] = {5, 3, 10, 1, 2};
      linear_insertion_sort_n(nums, 5, std::less<int>());
      print_range(nums, nums + 5);

      assert(is_sorted(nums, nums + 5));
  }
  // test for forward iterators
  {
      // std::list provides bidirectional iterators and
      // std::forward_list is in C++11, so we will use
      // our list pool to test this.
      
      list_pool<int, int> pool;

      // Create a list "1 2 3",
      typename list_pool<int, int>::list_type l = pool.end();
      l = pool.allocate(3, l);
      l = pool.allocate(2, l);
      l = pool.allocate(1, l);

      typedef typename list_pool<int, int>::iterator I;
      I first(pool, l);
      I last(pool);

      // do a few rotates and print the results
      rotate_right_by_one(first, last);
      print_range(first, last);

      rotate_right_by_one(first, last);
      print_range(first, last);

      rotate_right_by_one(first, last);
      print_range(first, last);
  }


  return 0;
}

