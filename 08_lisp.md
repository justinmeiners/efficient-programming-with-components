8. Lisp-like lists
============================

## Lists in lisp and Scheme

A long time ago there was a programming language called [Lisp][lisp]
or for you younger folks [Scheme][scheme].
Scheme might have been wrong, but it was great[^community].
The whole language centers around very simple [linked lists][linked-list]
 which are based on three fundamental
operations[^sicp]:

1. [`cons`](http://www.lispworks.com/documentation/lw50/CLHS/Body/f_cons.htm): create a pair.
2. [`car`](http://clhs.lisp.se/Body/f_car_c.htm#car): get first element of pair.
3. [`cdr`](http://clhs.lisp.se/Body/f_car_c.htm#cdr): get second element of pair.

We're not going to use these terms and
we're going to extend our vocabulary from 3 to 4.
Lisp told us that there's this wonderful thing, without which we cannot live, called 
[garbage collection][gc].
We don't want garbage collection for
all the algorithms we want to use.
So we are going to add a 4th operation:

4. `free`: manually release/free a pair.

What we want to do is muck around with lists.
Meaning you can insert items in the middle, change pointers, connect this and that.
All of these operations are natural.
If you don't want to muck around, just use vectors.

But, we're going to build it so it's blindingly fast.
How are we going to do that? 
You want to avoid memory fragmentation.
If you have lists
with nodes spread all over  memory, every time
you access one, it is a cache miss.
Modern computer caches do not really help if you do long jumps.
We have lots of nodes,
but we want them to live in a little buffer even if we keep generating them back and forth.
If they reside in a small space we will never get a cache miss.


[^sicp]: Alex calls these "lists" without much explanation.
    In Lisp all lists are built out of pairs.
    In each pair, the first element (called the `car`) is the value of the list at this point.
    The second element (called the `cdr`) is a pointer to another pair, or `nil`.
    `nil` terminates the list.

    For example,
    if we write a pair as `(car . cdr)` with `.` deliminating the `car` and `cdr`,
    the list `1 2 3` can be constructed from three pairs: 

        (1 . (2 . (3 . nil)))

    See [chapter 2.2][sicp] of "Structure and Interpretation of Computer Programs"
    for a thorough introduction to Lisp lists.

    `car` and `cdr` are commonly called `head` and `tail` in other functional languages.
    Their names are historical artifacts of the hardware that early Lisp implementations used
    (see ["CAR and CDR Wikipedia page"][car-and-cdr]).

[^community]: Alex: I'm talking to an apparently non-existent Lisp community 
    because MIT is just a Python school now
    (see ["Programming by poking: why MIT stopped teaching SICP"][programming-by-poking]).


[gc]: https://en.wikipedia.org/wiki/Garbage_collection_(computer_science)
[malloc]: https://man7.org/linux/man-pages/man3/malloc.3.html
[sicp]: https://mitpress.mit.edu/sites/default/files/sicp/full-text/book/book-Z-H-15.html#%_sec_2.2
[lisp]: https://en.wikipedia.org/wiki/Lisp_(programming_language)
[scheme]: https://en.wikipedia.org/wiki/Scheme_(programming_language)
[linked-list]: https://en.wikipedia.org/wiki/Linked_list
[car-and-cdr]: https://en.wikipedia.org/wiki/CAR_and_CDR
[programming-by-poking]: http://lambda-the-ultimate.org/node/5335

### Why is malloc so slow?

We're also going to avoid [`malloc`][malloc]
because it is evil.
It used to be sort of alright, when I started working on STL in 1993.
But, even then I realized it was too slow to be used with node
based data structures.
So for any data structure of nodes, such as list, I would keep a pool of
nodes myself and manage them in a quick way. 

A few people, such as [Bill Plauger][pj-plauger] at Microsoft 
and others at [GNU][gnu] who followed their example, said that if they have a common
pool and they just do pointer movement then if you have multiple threads you
could have problems[^race].
Instead of solving the problem for the multi-threaded case
they decided to solve it in general.
They said, "first we're going to put locks on our malloc[^lock].
Then we're going to throw Alex's pool management away
and we're going to do full malloc."
Now malloc is function call with a lock, so it's a very heavy operation.

Because of this decision, all our lists are going to be thread safe[^microsoft-thread-safe].
People like us, who do not use threads (you don't use threads right?) pay for them.
They violated a fundamental
principle which Bjarne insists on.
*People should not pay for things they do not use*.
Everybody pays for the ability of multiple threads to do list allocations out of the
same pool. Which actually nobody does, but everybody pays.

[pj-plauger]: https://en.wikipedia.org/wiki/P._J._Plauger

[^microsoft-thread-safe]: Although `malloc` may lock, according to
    [this article](https://devblogs.microsoft.com/cppblog/concurrent-containers/),
    STL containers on Microsoft platforms do not attempt to 
    ensure thread safety with locks.
    
[^race]: All kinds of problems can arise from two threads modifying the same resource.
    When code executes concurrently, it's much more difficult to reason about control flow.
    One line does not immediately follow the other,
    so things can be overwritten or messed up in between statements.
    Another problem is called a [race condition][race].
    This is when a piece of code relies on one thread doing a task before another.

[^lock]: [Locks][lock] (often called mutexes in programming)
    are a mechanism for controlling access to a shared resource.
    To prevent multiple threads from running over each other,
    a lock ensures that only one thread can access or modify
    a shared resource at a time.
    Designing such a mechanism well is actually fairly difficult.
    (See "The Art of Multiprocesser Programming" by Herlihy and Shavit.)
    Locks tend to be slow because they pause threads until they are safe to proceed.
    In addition they usually communicate with the kernel.

    Many programming frameworks in the late 90s and early 2000s (especially Java and C#)
    decided that the way to support multithreaded programming was to protect
    every resource with locks,
    as if programs should share class instances
    across threads haphazardly.
    This trend is reflected in Alex's story.

    Since then, the error prone nature of concurrency and parallelism
    has encouraged more disciplined design and tools.
    One approach is to organize the program architecture around a few specific threads running
    for the duration of the program, with carefully controlled communication
    protocols.
    Another is to spawn threads only to compute pure functions,
    which do not have shared resource problems.

    Based on Alex's comments we can guess that 
    he would prefer processes to threads.
    Processes offer memory protection by default, with all the danger
    centralized in small shared portions.
    (See chapter 7 of "The Art of UNIX Programming")

[gnu]: https://www.gnu.org/
[lock]: https://en.wikipedia.org/wiki/Lock_(computer_science)
[race]: https://en.wikipedia.org/wiki/Race_condition

## List pool

A list pool is an object
with many outstanding lists inside.
Internally we will use one vector to implement many, many, lists.
These lists are not containers.
A container guarantees that when the container is gone, the values are gone too.
For these lists there is no guarantee like that.
For example, you could split this list
into two by setting a `cdr`.
There is no ownership and this is why I recommend not viewing them as containers.
STL containers are wonderful when you want them, but that's not the case here.

We're trying to get as close to Lisp as we can without building garbage
collection[^difference]. If you want to build garbage collection you can extend
this thing and build garbage collection too, but garbage collection
is overrated.


[^difference]: A significant difference between Alex's lists and those
    in Lisp is that they are homogenous,
    they can only store one type of value.
    In Lisp, heterogeneous lists are everywhere,
    especially nested lists, which are what
    allow code to be written in a list format. 

    For example the expression:
        
        (+ (* 1 2) 3)

    Is a valid piece of code in Lisp.
    It is also two lists nested together.
    The inner list is the symbol `*` followed
    by `1` and `2`.
    The outer list starts with the symbol `+`, etc.

    The complexity of allocating and managing memory
    for such structures was one of the motivations
    for inventing garbage collection.


We will implement `list_pool` as a class, with two types as template arguments.
`T` will be the values we want to store,
and `N` will be an index type.

    #include <vector>
    template<typename T, typename N>
    // T is semi-regular.
    // N is integral
    class list_pool {
        typedef N list_type;

        struct node_t {
          T value;
          N next;
        };

        std::vector<node_t> pool;
        list_type free_list;
    };

What should `N` be. Why not `size_t`?
Because it's 64 bits.
For our application we could probably
use `uint16` so our whole node fits in 32 bits.
But, we should define a default.

    typename N = size_t;

Now we are going to implement `cons`, `car`, `cdr`, and `free` as member
functions of `list_pool`, but we need appropriate names for a younger generation.

### Value (car)

We will rename `car` to `value`.
Note that because we can return `value` by reference,
it can be both read and modified.
So, it won't just be `car`, it will
also act as [`rplaca`][rplaca][^rplaca-explanation].

    T& value(list_type x) {
      return node(x).value;
    }

    const T& value(list_type x) const {
      return node(x).value;
    }


### Next (cdr)

Let's rename `cdr` to `next`.
Because of read and write it also acts as [`rplacd`][rplacd].

    list_type& next(list_type x) {
      return node(x).next;
    }

    const list_type& next(list_type x) const {
      return node(x).next;
    }

[rplacd]: http://clhs.lisp.se/Body/f_rplaca.htm

### Free

Now let's write `free`.
This operation appends a pair to list making it available for reuse in another allocation.

    list_type free(list_type x) {
      list_type cdr = next(x);
      next(x) = free_list;
      free_list = x;
      return cdr;
    }

[^free-in-lisp]
We make it somewhat more useful by returning something other than void.
Return the `next`, otherwise the user will have to save it before freeing.

### Allocate (cons)

Now we will write `cons`, it takes two arguments.
Where do nodes come from?
The free list, if it has room,
otherwise we make a new node from the pool.

    list_type allocate(const T& val, list_type tail) {
      list_type new_list = free_list;

      if (is_empty(free_list)) {
        new_list = new_node();
      } else {
        free_list = next(free_list);
      }

      // start with this part
      value(new_list) = val;
      next(new_list) = tail;
      return new_list;
    }

So we need to write the public function `is_empty`.

    bool is_empty(list_type x) const {
      return x == empty();
    }

Dual to this function, is one which gives you the `nil` or empty list.

    list_type empty() {
      return list_type(0);
    }

You might think, what about the `0`th item in the pool?
We will just index everything at `1`, so we don't lose
the first item.
If you use `-1` then our index type must be signed.

    list_pool() {
      free_list = empty();
    }

Now we write a few private node functions including `new_node`.

    node_t& node(list_type x) {
      return pool[x - 1];
    }

    const node_t& node(list_type x) const {
      return pool[x - 1];
    }

    list_type new_node() {
      pool.push_back(node_t());
      return list_type(pool.size());
    }

Our structure requires all lists in the pool to be `const`
or all of them to be non-`const`.
Typically `const` is just for handing someone something
to read.

### Free list helper

There is a simple rule to distinguish when
you should write a method/member function
and when to just make an outside function (free function).
Implement the simplest possible thing.
If you can do it outside, do it.


Let's implement a function for freeing an entire list,
not just a node.

    template <typename T, typename N>
    void free_list(list_pool<T, N>& pool,
        typename list_pool<T, N>::list_type x) {
      while (!pool.is_empty(x)) x = pool.free(x); 
    }


[^rplaca-explanation]: `rplaca` and `rplacd` are unfriendly abbrevations of
    "replace car" and "replace cdr" (or historically as "replace address" and "replace decrement". See "Lisp 1.5 Programmer's Manual").
    They are low-level functions for manipulating pairs in lists.
    In Scheme they correspond to `set-car!` and `set-cdr!`.
    In Common Lisp one typically uses the higher-level macro [`setf`][setf] for the same purpose.

[^free-in-lisp]: The assignment in this code is the same as `(setf (cdr x) free-list)` in Common Lisp, 
    or `(set-cdr! x free-list)` in Scheme.

[rplaca]: http://clhs.lisp.se/Body/f_rplaca.htm
[setf]: http://www.lispworks.com/documentation/lw50/CLHS/Body/m_setf_.htm


## List queue

We can use our list to implement a queue structure.
The queue will be defined by a list node
in the front, and one in the back.

    typedef std::pair<list_type, list_type> pair_type;

We often want to detect empty queues
and construct them:

    bool empty(const pair_type& p) { return is_end(p.first); }
    pair_type empty_queue() { return pair_type(end(), end()); }

You can remove an element from the front of the queue:

    pair_type pop_front(const pair_type& p) {
      if (empty(p)) return p;
      return pair_type(next(p.first), p.second);
    }

You can add an element to the front, or the back of the queue:

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

Now we can also free lists in constant time,
simply by attaching the end of our list to the free list.

    void free(const pair_type& p) { free(p.first, p.second); }


## Code

- [list_pool.h](code/list_pool.h)
- [list_pool_iterator.h](code/list_pool_iterator.h) (included by `list_pool.h`, but not discussed until Chapter 9)
