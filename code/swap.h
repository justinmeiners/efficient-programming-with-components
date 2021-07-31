#ifndef SWAP_H
#define SWAP_H

template <typename T>
// requires T is Semiregular
inline
void swap(T& a, T& b) {
  T tmp(a);
  a = b;
  b = tmp; 
}

template <typename T>
// requires T is UnsignedIntegral
inline
void swap_xor(T& a, T& b) {
  if (&a != &b) {
    a = a ^ b;
    b = a ^ b;
    a = a ^ b;
  }
}

#endif
