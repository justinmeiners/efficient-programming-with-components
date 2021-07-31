#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <numeric>
#include <algorithm>
#include <iostream>

#define InputIterator typename
#define ForwardIterator typename
#define RandomAccessIterator typename
#define BidirectionalIterator typename
#define Integral typename

template <ForwardIterator I, Integral N> 
N iota(I first, I last, N start = N(0), N step = N(1)) {
    typedef typename std::iterator_traits<I>::value_type T;
    while (first != last) {
        *first = T(start);
        start += step;
        ++first;
    }
    return start;
}

template <RandomAccessIterator I>
void random_iota(I first, I last) {
    ::iota(first, last, 0);
    std::random_shuffle(first, last);
}

template <BidirectionalIterator I>
void reverse_iota(I first, I last) {
    ::iota(first, last, 0);
    std::reverse(first, last);
}

template <typename I>
void print_range(I first, I last) {
    while (first != last) {
        std::cout << *first << " ";
        ++first;
    }
    std::cout << std::endl;
}

template <typename I, typename N>
// I is InputIterator
// N is Integral
inline
void advance(I& first, N n) {
  // [first, n) is a valid range
  advance(first, n, std::iterator_traits<I>::iterator_category());
}

template <typename I, typename N>
// I is InputIterator
// N is Integral
inline
void advance(I& first, N n, std::input_iterator_tag) {
  // [first, n) is a valid range
  while (n--) ++first; 
}

template <typename I, typename N>
// I is RandomAccessIterator
// N is Integral
inline
void advance(I& first, N n, std::random_access_iterator_tag) {
  // [first, n) is a valid range
  first += n;
}

template <typename I>
// I is InputIterator
inline
typename std::iterator_traits<I>::difference_type 
distance(I first, I last) {
  // [first, n) is a valid range
  return distance(first, last, 
		  std::iterator_traits<I>::iterator_category());
}

template <typename I>
// I is InputIterator
inline
typename std::iterator_traits<I>::difference_type 
distance(I first, I last, std::input_iterator_tag) {
  // [first, n) is a valid range
  typename std::iterator_traits<I>::difference_type n(0);
  while (first != last) { 
    ++n; 
    ++first; 
  }
  return n; 
}

template <typename I>
// I is RandomAccessIterator
inline
typename std::iterator_traits<I>::difference_type 
distance(I first, I last, std::random_access_iterator_tag) {
  // [first, n) is a valid range
  return last - first; 
}

template <typename I>
// requires I is InputIterator
inline
I successor(I x) { return ++x; }

template <typename I>
// requires I is BidirectionalIterator
inline
I predecessor(I x) { return --x; }

template <typename I, typename N>
// requires I is RandomAccessIterator
// requires N is Integral
inline
I successor(I x, N n, std::random_access_iterator_tag) { 
  return x + n;
}

template <typename I, typename N>
// requires I is InputIterator
// requires N is Integral
inline
I successor(I x, N n, std::input_iterator_tag) { 
  while (n != N(0)) {
    ++x;
    --n;
  }
  return x;
}

template <typename I, typename N>
// requires I is InputIterator
// requires N is Integral
inline
I successor(I x, N n) {
  typedef typename std::iterator_traits<I>::iterator_category C;
  return successor(x, n, C());
}

template <typename I>
// requires I is BidirectionalIterator
void hill(I first, I last)
{
  I middle = successor(first, std::distance(first, last)/2);
  iota(first, middle);
  reverse_iota(middle, last);
}

template <typename I>
// requires I is BidirectionalIterator
void valley(I first, I last)
{
  I middle = successor(first, std::distance(first, last)/2);
  reverse_iota(first, middle);
  iota(middle, last);
}


#endif
