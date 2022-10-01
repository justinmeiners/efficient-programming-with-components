#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include "binary_insertion_sort.h"

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
I linear_insert(I first, I current, R r) {
  // precondition: is_sorted(first, current, r) && current is a valid iterator
  typedef typename std::iterator_traits<I>::value_type T;
  T value = *current;
  while (first != current && r(value, *predecessor(current))) {
    *current = *predecessor(current);
    --current;
  }
  *current = value; 
  return current;
}


template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void linear_insertion_sort(I first, I last, R r) {
  if (first == last) return; 
  I current = first;
  ++current;
  while (current != last) {
    linear_insert(first, current, r);     
    ++current;
  }
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I 
void selection_sort(I first, I last, R r) {
  while (first != last) {
    I min = std::min_element(first, last, r);
    std::swap(*first, *min);
    ++first;
  }
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I 
void stable_selection_sort(I first, I last, R r) {
  while (first != last) {
    I min = std::min_element(first, last, r);
    rotate_right_by_one(first, ++min);
    ++first;
  }
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort_suffix(I first, I last, R r) {
  // precondition: one of -
  // (1) a valid range before first is sorted with respect to r and contains a sentinel
  // (2) *first is a sentinel
  if (first == last) return; 
  I current = first;
  ++current;
  while (current != last) {
    linear_insert_with_sentinel(first, current, r);     
    ++current;
  }
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
I linear_insert_with_sentinel(I first, I current, R r) {
  // precondition: is_sorted(first, current, r) &&
  //               current is a valid iterator &&
  //               first != current &&
  //               !r(*current, *first)
  typedef typename std::iterator_traits<I>::value_type T;
  T value = *current;
  
  while (r(value, *predecessor(current))) {
    *current = *predecessor(current);
    --current;
  }
  *current = value; 
  return current;
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort(I first, I last, R r) {
  if (first == last) return; 
  I current = first;
  ++current;
  if (current == last) return;
  // create a sentinel 
  I min = std::min_element(first, last, r);
  rotate_right_by_one(first, ++min);
  insertion_sort_suffix(current, last, r);
}

template <typename I, typename R>
// I is BidirectionalIterator
// R is WeakStrictOrdering on the value type of I 
void insertion_sort_unstable(I first, I last, R r) {
  if (first == last) return; 
  I current = first;
  ++current;
  if (current == last) return;
  // create a sentinel 
  I min = std::min_element(first, last, r);
  std::swap(*first, *min);
  insertion_sort_suffix(current, last, r);
}

template <typename I, typename N, typename R>
// I is BidirectionalIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I 
I linear_insertion_sort_n(I first, N n, R r) {
  if (n == N(0)) return first;
  I current = first;
  ++current;
  N i(1);
  while (i < n) {
    // invariant: is_sorted_n(first, i, r) && std::distance(first, current) == i
    linear_insert(first, current++, r);
    ++i;
  }
  return current;
}

#endif
