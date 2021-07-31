#ifndef MERGE_LINKED_H
#define MERGE_LINKED_H

#include "binary_counter.h"

template <typename I>
// requires I is Linked Iterator
I reverse_linked(I first, I last, I tail) {
  while (first != last) {
    I next = first;
    ++next;
    set_successor(first, tail);
    tail = first;
    first = next;
  }
  return tail;
}

template <typename I, typename Compare>
// requires I is Linked Iterator
I merge_linked_simple(I first1, I last1, I first2, I last2, Compare cmp) {
  I result = last1;
  while (first1 != last1 && first2 != last2) {
    I tmp = cmp(*first2, *first1) ? first2++ : first1++;
    set_successor(tmp, result);
    result = tmp;
  }
  return reverse_linked(result, last1, first1 == last1 ? first2 : first1);
}

template <typename I, typename Compare>
// I is a linked forward iterator
std::pair<I, std::pair<I, I> >
merge_linked_non_empty(I first1, I last1, I first2, I last2, Compare cmp) {
  I head, tail;
  if (cmp(*first2, *first1)) {
    head = first2;
    tail = first2;
    ++first2;
    goto winner2;
  } else {
    head = first1;
    tail = first1;
    ++first1;
    // goto winner1;
  }
winner1:
  if (first1 == last1) goto empty1;
  if (!cmp(*first2, *first1)) {
    tail = first1;
    ++first1;
    goto winner1;
  } else {
    set_successor(tail, first2); 
    tail = first2;
    ++first2;
    // goto winner2;
  }
winner2:
  if (first2 == last2) goto empty2;
  if (cmp(*first2, *first1)) {
    tail = first2;
    ++first2;
    goto winner2;
  } else {
    set_successor(tail, first1);
    tail = first1;
    ++first1;
    goto winner1;
  }
empty1:
  set_successor(tail, first2);
  return std::make_pair(head, std::make_pair(first2, last2));
empty2:
  set_successor(tail, first1);
  return std::make_pair(head, std::make_pair(first1, last1));
}


template <typename I, typename Compare>
// I is Linked Iteratork
struct mergesort_linked_operation
{
  typedef I argument_type;
  I nil;
  Compare cmp;
  mergesort_linked_operation(I nil, const Compare& cmp) : nil(nil), cmp(cmp) {}
  I operator()(I x, I y) { return merge_linked_simple(x, nil, y, nil, cmp); }
};

template <typename I, typename Compare>
// I is Linked Iterator
I mergesort_linked(I first, I last, Compare cmp) {
  mergesort_linked_operation<I, Compare> op(last, cmp);
  binary_counter<mergesort_linked_operation<I, Compare> > counter(op, last);
  counter.reserve(16);
  while (first != last) {
    I tmp = first++;
    set_successor(tmp, last);
    counter.add(tmp);
  }
  return counter.reduce();
}


template <typename I0, typename I1>
// requires I0 is Input Iterator
// requires I1 is Singly Linked List Iterator
I1 generate_list(I0 first, I0 last, I1 tail) {  
  if (first == last) return tail;
  push_front(tail, *first++);
  I1 front = tail;
  while (first != last) {
    push_back(tail, *first++);
    ++tail;
  }
  return front;
}




#endif
