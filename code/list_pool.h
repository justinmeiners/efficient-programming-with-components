#ifndef LIST_POOL_H
#define LIST_POOL_H

#include <vector>
#include <cstddef>

// Requirements on T: semiregular. 
// Requirements on N: integral
template <typename T, typename N = std::size_t>
class list_pool {
  public:
    typedef T value_type;
    typedef N list_type;

  private:

    struct node_t {
      value_type value; 
      list_type next; 
    };

    std::vector<node_t> pool; 

    node_t& node(list_type x) {
      return pool[x - 1]; 
    }
    const node_t& node(list_type x) const {
      return pool[x - 1]; 
    }

    list_type new_list() {
      pool.push_back(node_t()); 
      return list_type(pool.size());
    }

    list_type free_list;

  public:
    typedef typename std::vector<node_t>::size_type size_type;

    list_type end() const {
      return list_type(0);
    }

    bool is_end(list_type x) const {
      return x == end();
    }

    bool empty() const { 
      return pool.empty();
    }

    size_type size() const {
      return pool.size();
    }

    size_type capacity() const {
      return pool.capacity();
    }

    void reserve(size_type n) {
      pool.reserve(n);
    }

    list_pool() {
      free_list = end();
    }

    list_pool(size_type n) {
      free_list = end();
      reserve(n); 
    }

    T& value(list_type x) {
      return node(x).value;
    }

    const T& value(list_type x) const {
      return node(x).value;
    }

    list_type& next(list_type x) {
      return node(x).next;
    }
    const list_type& next(list_type x) const {
      return node(x).next;
    }

    list_type free(list_type x) {
      list_type cdr = next(x); 
      next(x) = free_list; 
      free_list = x; 
      return cdr; 
    }

    list_type free(list_type front, list_type back) {
      if (is_end(front)) return end();
      list_type tail = next(back);
      next(back) = free_list;
      free_list = front;
      return tail; 
    }


    list_type allocate(const T& val, list_type tail) {
      list_type list = free_list; 
      if (is_end(free_list)) {
        list = new_list();
      } else {
        free_list = next(free_list);
      }
      value(list) = val; 
      next(list) = tail;
      return list; 
    }

    // operations on queues:
    // pop_front, push_front, push_back and free, etc 

    typedef std::pair<list_type, list_type> pair_type;

    bool empty(const pair_type& p) { return is_end(p.first); }

    pair_type empty_queue() { return pair_type(end(), end()); }

    pair_type pop_front(const pair_type& p) {
      if (empty(p)) return p;
      return pair_type(next(p.first), p.second);
    }

    pair_type push_front(const pair_type& p, const T& value) {
      list_type new_node = allocate(value, p.first);
      if (empty(p)) return pair_type(new_node, new_node);
      return pair_type(new_node, p.second);
    }

    pair_type push_back(const pair_type& p, const T& value) {
      list_type new_node = allocate(value, end());
      if (empty(p)) return pair_type(new_node, new_node);
      next(p.second) = new_node;
      return pair_type(p.first, new_node);
    }

    void free(const pair_type& p) { free(p.first, p.second); }

    #include "list_pool_iterator.h"
};

template <typename T, typename N>
void free_list(list_pool<T, N>& pool, 
    typename list_pool<T, N>::list_type x) 
{
  while (!pool.is_end(x)) x = pool.free(x);
}



#endif
