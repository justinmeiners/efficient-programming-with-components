#ifndef SEARCH_H
#define SEARCH_H

#include <algorithm>
#include <iterator>

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
I find_if(I first, I last, P pred) {
  while (first != last && !pred(*first)) ++first;
  return first;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
I find_if_not(I first, I last, P pred) {
  while (first != last && pred(*first)) ++first;
  return first;
}

template <typename I, typename N, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
std::pair<I, N> find_if_n(I first, N n, P pred) {
  while (n && !pred(*first)) {--n; ++first;}
  return std::make_pair<I, N> (first, n);
}

template <typename I, typename N, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
std::pair<I, N> find_if_not_n(I first, N n, P pred) {
  while (n && pred(*first)) {--n; ++first;}
  return std::make_pair<I, N> (first, n);
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool all_of(I first, I last, P pred) {
  return find_if_not(first, last, pred) == last;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool none_of(I first, I last, P pred) {
  return find_if(first, last, pred) == last;
}

template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
bool any_of(I first, I last, P pred) {
  return find_if(first, last, pred) != last;
}

#endif // SEARCH_H
