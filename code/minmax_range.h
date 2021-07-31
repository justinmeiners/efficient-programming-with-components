#ifndef MIN_MAX_RANGE_H
#define MIN_MAX_RANGE_H

#include "minmax.h"


template <typename I, typename Compare>
// I is a ForwardIterator
// Compare is StrictWeakOrdering on ValueType(I)
I min_element(I first, I last, Compare cmp) {
  if (first == last) {
    return last;
  }

  I min_el = first;
  ++first;

  while (first != last) {
    if (cmp(*first, *min_el)) {
      min_el = first;
    }
    ++first;
  }

  return min_el;
}

template <typename I, typename Compare>
// requires I is a ForwardIterator
// // and Compare is a StrictWeakOrdering on ValueType(I)
I max_element(I first, I last, Compare cmp) {
  if (first == last) return last;
  I max_el = first;
  while (++first != last) {
    if (!cmp(*first, *max_el)) {
      max_el = first;
    }
  }
  return max_el; 
}


template <typename I, typename Compare>
// I is a ForwardIterator
// Compare is StrictWeakOrdering on ValueType(I)
std::pair<I, I> minmax_element(I first, I last, Compare cmp) {
  if (first == last) {
    return std::make_pair(last, last);
  }

  I min_el = first;
  ++first;
  if (first == last) {
    return std::make_pair(min_el, min_el);
  }

  I max_el = first;
  I next = first;
  ++next;

  if (cmp(*max_el, *min_el)) {
    std::swap(min_el, max_el);
  }

  while (first != last && next != last) {
    // min_el contains the current min
    // max_el contains the current max
    // next and first and the next pair of elements to examine
    I potential_min = first;
    I potential_max = next;

    if (cmp(*potential_max, *potential_min)) {
      std::swap(potential_max, potential_min);
    }

    if (cmp(*potential_min, *min_el)) {
      min_el = potential_min;
    }

    if (!cmp(*potential_max, *max_el)) {
      max_el = potential_max;
    }

    ++next;
    first = next;
    ++next;
  }

  if (first != last) {
    // odd elements, one left over

    if (cmp(*first, *min_el)) {
      min_el = first;
      // first < min_el <= max_el, so we don't need to check the next case
    } else if (!cmp(*first, *max_el)) {
      max_el = first;
    }
  }

  return std::make_pair(min_el, max_el);
}

#endif
