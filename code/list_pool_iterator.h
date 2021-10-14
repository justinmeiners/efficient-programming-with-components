struct iterator {
  typedef typename list_pool::value_type value_type;
  typedef typename list_pool::list_type difference_type;    
  typedef typename std::forward_iterator_tag iterator_category;

  typedef value_type& reference;
  typedef value_type* pointer; 

  list_pool* pool;
  typename list_pool::list_type node;

  iterator() {} // creates a partially formed value
  iterator(list_pool& p, list_pool::list_type node) :
    pool(&p), node(node) {}
  iterator(list_pool& p) : pool(&p), node(p.end()) {}

  reference operator*() const {
    return pool->value(node);
  }

  pointer operator->() const {
    return &**this;
  }

  iterator& operator++() {
    node = pool->next(node);
    return *this;
  }

  iterator operator++(int) {
    iterator tmp(*this);
    ++*this;
    return tmp;
  }

  friend 
    bool operator==(const iterator& x, const iterator& y) {
      // assert(x.pool == y.pool);
      return x.node == y.node;
    }

  friend
    bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

  // extends the interface to Linked Iterator:
  friend
    void set_successor(iterator x, iterator y) {
      // assert(x.p == y.p)
      x.pool->next(x.node) = y.node;
    }

  // extend the interface to Singly Linked List Iterator:
  // (not discussed in the course.)

  friend 
    void push_front(iterator& x, const T& value) {
      x.node = x.pool->allocate(value, x.node);
    }

  friend 
    void push_back(iterator& x, const T& value) {
      typename list_pool::list_type tmp = x.pool->allocate(value, x.pool->next(x.node));
      x.pool->next(x.node) = tmp;
    }

  friend 
    void free(iterator& x) {
      x.pool->free(x.node);
    }
};


