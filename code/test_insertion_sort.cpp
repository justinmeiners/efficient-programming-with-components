#include <functional>
#include <iostream>
#include "insertion_sort.h"
#include "algorithm.h"

int main() {
  {
      int nums[] = {5, 3, 10, 1, 2};
      print_range(nums, nums + 5);
      binary_insertion_sort_n(nums, 5, std::less<int>());
      print_range(nums, nums + 5);
  }
  {
      int nums[] = {5, 3, 10, 1, 2};
      print_range(nums, nums + 5);
      linear_insertion_sort_n(nums, 5, std::less<int>());
      print_range(nums, nums + 5);
  }
  return 0;
}

