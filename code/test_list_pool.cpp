#include <iostream>
#include <stdint.h>
#include "list_pool.h"

int main() {
  typedef typename list_pool<char, uint16_t>::list_type L;

  list_pool<char, uint16_t> pool;

  // Create a list "a b c",
  // same as Lisp (cons a (cons b (cons c '()))).
  L l = pool.end();
  l = pool.allocate('c', l);
  l = pool.allocate('b', l);
  l = pool.allocate('a', l);

  // Print contents of list.
  L current = l;
  while (current != pool.end()) {
    std::cout << pool.value(current) << " ";
    current = pool.next(current);
  }
  std::cout << std::endl;

  // Pool will cleanup automatically when out of scope.
  // But, let's go ahead and test free.
  free_list(pool, current);
}

