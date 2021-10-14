#include <iostream>
#include <iterator>
#include <stdint.h>
#include "list_pool.h"

int main() {
  list_pool<char, uint16_t> pool;

  // Create a list "a b c",
  // same as Lisp (cons a (cons b (cons c '()))).
  typename list_pool<char, uint16_t>::list_type l = pool.end();
  l = pool.allocate('c', l);
  l = pool.allocate('b', l);
  l = pool.allocate('a', l);

  typedef typename list_pool<char, uint16_t>::iterator I;
  I first(pool, l);
  I last(pool);

  // Print contents of list.
  std::copy(first, last, std::ostream_iterator<char>(std::cout, " "));
}


