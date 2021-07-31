#ifndef PARTITION_H
#define PARTITION_H

#include "search.h"

// the semantics of the standard: 
// elements satisfying the predicate are before elements that do not
template <typename I, typename P>
// I is InputIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool is_partitioned(I first, I last, P pred) {
  I first_false = find_if_not(first, last, pred);
  I next_true = find_if(first_false, last, pred);
  return next_true == last; 
}

// the semantics of the standard: 
// elements satisfying the predicate are before elements that do not
template <typename I, typename N, typename P>
// I is InputIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
bool is_partitioned_n(I first, N n, P pred) {
  std::pair<I, N> first_false = find_if_not_n(first, n, pred);
  std::pair<I, N> next_true = find_if_n(first_false.first, first_false.second, pred);
  return !next_true.second; 
}

// the semantics of the standard: 
// elements satisfying the predicate are before elements that do not
template <typename I, typename N, typename P>
// I is ForwardIterator, N is Integral, P is UnaryPredicate
// value type of I is the same as argument type of P
I partition_point_n(I first, N n, P pred) {
  // precondition: is_partitioned_n(first, n, pred)
  while (n) {
    N half = n >> 1;
    I middle = first;
    std::advance(middle, half);
    // [first, n) == [first, half) U [middle, n - half)
    if (pred(*middle)) { 
      n -= (half + 1); 
      first = ++middle; 
    }
    else { 
      n = half; 
    }
  } 
  return first;
}

// the semantics of the standard: 
// elements satisfying the predicate are before elements that do not
template <typename I, typename P>
// I is ForwardIterator, P is UnaryPredicate
// value type of I is the same as argument type of P
inline
I partition_point(I first, I last, P pred) {
  // precondition: is_partitioned(first, last, pred)
  return partition_point_n(first, std::distance(first, last), pred);
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I
bool is_sorted(I first, I last, R r) {
  if (first == last) return true;
  I previous = first;
  while (++first != last && !r(*first, *previous)) previous = first;
  return first == last;
}

template <typename I>
// I is ForwardIterator with a totally ordered value type
inline
bool is_sorted(I first, I last) {
  typedef typename std::iterator_traits<I>::value_type T;
  return is_sorted(first, last, std::less<T>());
}

template <typename I, typename N, typename R>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I
bool is_sorted_n(I first, N n, R r) {
  if (!n) return true;
  I previous = first;
  while (n && !r(*++first, *previous)) {
    previous = first;
    --n;
  }
  return !n;
}

template <typename I, typename N>
// I is ForwardIterator with a totally ordered value type
// N is Integral
inline
bool is_sorted_n(I first, N n) {
  typedef typename std::iterator_traits<I>::value_type T;
  return is_sorted_n(first, n, std::less<T>());
}

template <typename R, typename T>
// R is StrictWeakOrdering
// T is an argument type of R
class lower_bound_predicate
{
private:
  R r;
  const T* a;
public:
  lower_bound_predicate(const R& r, const T& a) : r(r), a(&a) {}
  bool operator()(const T& x) { return r(x, *a); }
};

template <typename I, typename N, typename R>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I
inline
I lower_bound_n(I first, N n, 
		const typename std::iterator_traits<I>::value_type& a, R r) {
  // precondition: is_sorted_n(first, n, r)
  typedef typename std::iterator_traits<I>::value_type T;
  return partition_point_n(first, n, lower_bound_predicate<R, T>(r, a));
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I
inline
I lower_bound(I first, I last, R r, 
	      const typename std::iterator_traits<I>::value_type& a) {
  // precondition: is_sorted(first, last, r)
  return lower_bound_n(first, std::distance(first, last), r);
}

template <typename I, typename N>
// I is ForwardIterator with a totally ordered value type
// N is Integral
inline
I lower_bound_n(I first, N n, 
		const typename std::iterator_traits<I>::value_type& a) {
  // precondition: is_sorted_n(first, n)
  typedef typename std::iterator_traits<I>::value_type T;
  return lower_bound_n(first, n, std::less<T>());
}

template <typename I>
// I is ForwardIterator with a totally ordered value type
inline
I lower_bound(I first, I last, 
	      const typename std::iterator_traits<I>::value_type& a) {
  // precondition: is_sorted(first, last)
  return lower_bound_n(first, std::distance(first, last));
}

template <typename R, typename T>
// R is StrictWeakOrdering
// T is an argument type of R
class upper_bound_predicate
{
private:
  R r;
  const T* a;
public:
  upper_bound_predicate(const R& r, const T& a) : r(r), a(&a) {}
  bool operator()(const T& x) { return !r(*a, x); }
};


template <typename I, typename N, typename R>
// I is ForwardIterator
// N is Integral
// R is WeakStrictOrdering on the value type of I
inline
I upper_bound_n(I first, N n, 
		const typename std::iterator_traits<I>::value_type& a, R r) {
  // precondition: is_sorted_n(first, n, r)
  typedef typename std::iterator_traits<I>::value_type T;
  return partition_point_n(first, n, upper_bound_predicate<R, T>(r, a));
}

template <typename I, typename R>
// I is ForwardIterator
// R is WeakStrictOrdering on the value type of I
inline
I upper_bound(I first, I last, R r, 
	      const typename std::iterator_traits<I>::value_type& a) {
  // precondition: is_sorted(first, last, r)
  return upper_bound_n(first, std::distance(first, last), r);
}

template <typename I, typename N>
// I is ForwardIterator with a totally ordered value type
// N is Integral
inline
I upper_bound_n(I first, N n, 
		const typename std::iterator_traits<I>::value_type& a) {
  // precondition: is_sorted_n(first, n)
  typedef typename std::iterator_traits<I>::value_type T;
  return upper_bound_n(first, n, std::less<T>());
}

template <typename I>
// I is ForwardIterator with a totally ordered value type
inline
I upper_bound(I first, I last, 
	      const typename std::iterator_traits<I>::value_type& a) {
  // precondition: is_sorted(first, last)
  return upper_bound_n(first, std::distance(first, last));
}



#endif
