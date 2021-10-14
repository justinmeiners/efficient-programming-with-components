#include <iostream>
#include <iterator>
#include <stdint.h>
#include "list_pool.h"

int main() {
  typedef typename list_pool<char, uint16_t>::iterator I;

  list_pool<char, uint16_t> pool;

  // Create a list "a b c",
  // same as Lisp (cons a (cons b (cons c '()))).
  I last(pool);
  I first = last; 
  push_front(first, 'c');
  push_front(first, 'b');
  push_front(first, 'a');

  // Print contents of list.
  std::copy(first, last, std::ostream_iterator<char>(std::cout, " "));
}


