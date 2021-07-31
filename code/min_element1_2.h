#ifndef MIN_ELEMENT1_2_H
#define MIN_ELEMENT1_2_H

#include <cstddef>
#include "algorithm.h"
#include "binary_counter.h"
#include "list_pool.h"

template <typename Compare>
class compare_dereference
{
private:
  Compare cmp;
public:
  compare_dereference(const Compare& cmp) : cmp(cmp) {}
  template <typename I>
  bool operator() (const I& x, const I& y) const {
    return cmp(*x, *y);
  }
};


template <typename T, typename N, typename Compare>
class op_min1_2 
{
private:
  Compare cmp;
  list_pool<T, N>* p;
public:
  typedef typename list_pool<T, N>::list_type list_type;
  typedef std::pair<T, std::pair<list_type, list_type> > argument_type;

  op_min1_2(const Compare& cmp, list_pool<T, N>& pool) : cmp(cmp), p(&pool) {}

  argument_type operator()(const argument_type& x, 
                           const argument_type& y) {
    if (!cmp(y.first, x.first)) {
      p->free(y.second);
      return std::make_pair(x.first, p->push_back(x.second, y.first));
    } else {
      p->free(x.second);
      return std::make_pair(y.first, p->push_front(y.second, x.first));
    }
  }
};

template <typename I, typename Compare>
// requires I is a ForwardIterator
// and Compare is a StrictWeakOrdering on ValueType(I)
std::pair<I, I> min_element1_2(I first, I last, Compare cmp) {
  if (first == last || successor(first) == last) {
    return std::make_pair(first, last);
  }


  typedef compare_dereference<Compare> compare_type;
  typedef op_min1_2<I, size_t, compare_type> op_type;
  typedef binary_counter<op_type> counter_type;
  typedef typename list_pool<I, std::size_t>::iterator iterator;
  typedef typename op_type::argument_type  result_type;

  list_pool<I, std::size_t> pool;
  pool.reserve(256);
  compare_type cmp_deref(cmp);
  op_type op(cmp_deref, pool);
  counter_type counter(op, std::make_pair(last, pool.empty_queue()));
  counter.reserve(32);

  while (first != last) counter.add(std::make_pair(first++, pool.empty_queue()));
  result_type min1_list = counter.reduce();
  I min1 = min1_list.first;
  I min2 = *std::min_element(iterator(pool, min1_list.second.first), iterator(pool), cmp_deref);
  return std::make_pair(min1, min2);
}

#endif
