#ifndef MIN_MAX_H
#define MIN_MAX_H

template <typename T, typename Compare>
// Compare is StrictWeakOrdering on type T
inline
const T& min(const T& a, const T& b, Compare cmp) {
  if (cmp(b, a)) {
    return a;
  } else {
    return b; 
  }
}


template<typename T>
// T is TotallyOrdered
struct less {
  bool operator()(const T& a, const T& b) const {
    return a < b;
  }
};

template <typename T>
const T& min(const T& a, const T& b) {
  return min(a, b, std::less<T>());
}

template<typename T, typename Compare>
// requires Compare is a StrictWeakOrdering on T
inline
const T& max(const T& a, const T& b, Compare cmp) {
  if (cmp(b, a)) {
    return a;
  } else {
    return b;
  }
}


template<typename T, typename Compare>
// requires Compare is a StrictWeakOrdering on T
inline
void sort2(T& a, T& b, Compare cmp) {
  if (cmp(b, a)) {
    swap(a, b);
  }
}


#endif
