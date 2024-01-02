17. Adaptive merge sort
========================

## "temporary" buffers in STL

When I was implementing STL
I said that the OS vendor should know how much extra storage is available.
So, I'm going to have this function called `get_temporary_buffer`
which allocates as much physical memory is available at this moment.
It's the only outside hook which STL will use.
But, it is vendor specific, you cannot do it as a client.
There is actually no call in UNIX which tells you how much physical memory
you have, how much is used, it's just impossible. 
But, I needed to ship it, and in order to do that, I couldn't just require them to add a hook.
So I wrote the following thing:

    // ask for n, system gives you as much as it can, but not more than n.
    std::pair<void*, size_t> get_temporary_buffer(size_t n) { 
      // this is bogus code and needs to be replaced
      void* buffer = malloc(n);
      while (!buffer && n) {
        n = n >> 1;
        buffer = malloc(n);
      }
      return std::make_pair(buffer, n);
    }

So it binary searches for a buffer small enough to fit[^malloc-fail-return].
Is it a useful piece of code? No.
But, I had to ship.
Guess what happened after that.
You might think they didn't change it.
You're wrong, they did change it.
They removed my comment.

Every implementation, UNIX, Microsoft, Apple,
does the same binary search.
I have been telling this story for decades, nothing.
Therefore, my function always returns whatever you ask, because 
it just allocates [virtual memory][virtual-memory][^virtual-memory].
It used to be a problem when we had 16-bit address spaces.
But, we have 64 bit address spaces.

If it was correct, I think it would be useful.
You want as much physical memory as the system has.
There is virtual memory but virtual memory is actually useless unless it's backed by physical memory.
It's useful for remapping things[^memory-map].
But, it is a figment of imagination.
It does not exist.
As [Seymour Cray][cray] used to say, "you can't simulate what you do not have"[^cray-ref-request]. 
If your algorithm working set doesn't fit into physical memory,
it will not just [thrash][thrash], your program will not terminate,
because your memory starts working at the speed of a disk.
That's not good enough.

It just shows you how imperfect life is.

[^malloc-fail-return]: `malloc` returns `NULL` when it fails to allocate of the requested size.
    Alex's `get_temporary_buffer` function uses that as an indicator that the requested buffer was too large
    and continues attempting smaller and smaller buffers.

[^cray-ref-request]: I cannot find a reference to this quotation.

[^virtual-memory]: Virtual memory allows programs to allocate more memory than is physically available
    by saving and loading portions of memory to disk as needed.
    When memory is fully utilized the system starts working slower rather than simply crashing.

    Even though the total amount of virtual memory available on a system is very large,
    individual memory allocations are typically limited.
    For example, when testing this code on Linux, the system only 
    allows a program to allocate a buffer up to the total physical memory size.

    What this means is that Alex's implementation of `get_temporary_buffer` is not useful.
    It is equivalent to `malloc(n)` for anything but extremely large allocations.

    **Exercise:** Experiment with `get_temporary_buffer` on your machine. How large of an allocation will it give you?

[^memory-map]: Memory mapping files is a very useful application of virtual memory.
    When a program wants to interact with a file on disk it can instead request that
    the system map it to a range in memory.
    The file can then be manipulated by reading and writing to pointers as if it was a buffer instead of a file.
    In other words, the program can interact with the file, just like other data.
    See [mmap(2)](https://man7.org/linux/man-pages/man2/mmap.2.html) for details.

    Alex has used memory mapped files in his own code.

[cray]: https://en.wikipedia.org/wiki/Seymour_Cray
[thrash]: https://en.wikipedia.org/wiki/Thrashing_(computer_science)
[virtual-memory]: https://en.wikipedia.org/wiki/Virtual_memory

## Merge with buffer

The first thing we are going
to write is `merge_with_buffer`.
Let's assume that this buffer is big enough.
Eventually we will have to figure out how to deal with limited buffers.
Right, now let's assume whoever is going to call is going to assure it.
What we do is copy from the first range into our buffer,
then we merge back into the original buffer.

For now we can rely on [`std::merge`][cpp-merge],
but later we will write a better one.

    template <typename I, typename R, typename B>
    // requires I is ForwardIterator
    // requires R is StrictWeakOrdering
    // requires B is ForwardIterator
    void merge_with_buffer(I first, I middle, I last, R r, B buffer) {
        B buffer_last = std::copy(first, middle, buffer);
        std::merge(buffer, buffer_last, middle, last, first, r);
    }


Even though we aren't worried about it now, we can see the buffer
will need to be big enough to copy the entire left half in,
so about size `n/2`.

Note that the buffer doesn't have to match the type of the container.
We will probably use an array for buffer,
but `I` could be an iterator for a linked list.
This is a general principle, relax type requirements.

[cpp-merge]: https://en.cppreference.com/w/cpp/algorithm/merge

Let's grab our in-place sort from before and modify it.
It's identical to our other,
except it uses our new `merge_with_buffer`.
Should this function allocate the buffer?
No because it is recursive.

    template <typename I, typename N, typename R, typename B>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    I sort_inplace_n_with_buffer(I first, N n, R r, B buffer) {
      if (!n) return first;
      N half = n >> 1;
      if (!half) return ++first;
      I middle = sort_inplace_n_with_buffer(first, half, r, buffer);
      I last   = sort_inplace_n_with_buffer(middle, n - half, r, buffer);
      merge_with_buffer(first, middle, last, r, buffer);
      return last;
    }


Note we put the buffer argument at the end, because
we are extending the interface of the previous sort.

Now to use it in our framework we need a more convenient interface.
We have too many parameters, so we need to somehow get rid of all of them.
We write a wrapper.

    template <typename I>
    // I is ForwardIterator
    inline
    void sort_inplace_with_buffer(I first, I last) {
        typedef typename std::iterator_traits<I>::value_type T;
        typedef typename std::iterator_traits<I>::difference_type N;
        N n = std::distance(first, last);
        std::vector<T> buffer(n >> 1);
        sort_inplace_n_with_buffer(first, n, std::less<T>(), buffer.begin());
    }

### Performance test

Let's give it a try in the same timing framework we had before[^pc-info].

    Sorting double from 8 up to 2097152 elements generated with iota at: Mon Jun 28 21:02:41 2021
            size	stable	inplace	with_buffer	
               8	    14	    20	    16	
              16	     8	    28	    19	
              32	     8	    39	    19	
              64	    10	    48	    22	
             128	    10	    59	    25	
             256	    13	    78	    29	
             512	    17	   107	    36	
            1024	    27	   136	    39	
            2048	    34	   147	    49	
            4096	    45	   172	    59	
            8192	    54	   192	    67	
           16384	    62	   209	    75	
           32768	    67	   227	    81	
           65536	    72	   245	    86	
          131072	    76	   263	    92	
          262144	    82	   282	    97	
          524288	    88	   303	   104	
         1048576	    94	   323	   109	
         2097152	    98	   343	   115

Look at how fast that is.
It's already within 10% of our goal.
It shows us the spectrum of what's possible.

[^pc-info]: AMD Ryzen 5 2400G (8 core, 3.6 GHz). GCC 9.3.0

## Adaptive merge

We play the copy and paste game with our work from before.
The function `merge_inplace_left_subproblem`
and the right variant, do not need to be changed,
so they can be included.

    template <typename I, typename N, typename R, typename B>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    void merge_adaptive_n(I  f0,   N  n0,
                          I  f1,   N  n1,   R r, B buffer, N buffer_size) {
      // precondition std::distance(f0, f1) == n0
      // precondition is_sorted_n(f0, n0, r) && is_sorted(f1, n1, r)
      if (!n0 || !n1) return;
      if (n0 <= buffer_size) {
        I last = f1;
        std::advance(last, n1);
        merge_with_buffer(f0, f1, last, r, buffer);
        return;
      }
      I f0_0, f0_1, f1_0, f1_1;
      N n0_0, n0_1, n1_0, n1_1;
      if (n0 < n1)  merge_inplace_left_subproblem(f0,   n0,
                                                  f1,   n1,
                                                  f0_0, n0_0,
                                                  f0_1, n0_1,
                                                  f1_0, n1_0,
                                                  f1_1, n1_1,
                                                  r);
      else         merge_inplace_right_subproblem(f0,   n0,
                                                  f1,   n1,
                                                  f0_0, n0_0,
                                                  f0_1, n0_1,
                                                  f1_0, n1_0,
                                                  f1_1, n1_1,
                                                  r);

      merge_adaptive_n(f0_0, n0_0, f0_1, n0_1, r, buffer, buffer_size);
      merge_adaptive_n(f1_0, n1_0, f1_1, n1_1, r, buffer, buffer_size);
    }

Now we know the drill to turn this into sort.
I will just show the sort so we can see the buffer allocation:

    template <typename I>
    // I is ForwardIterator
    inline
    void sort_1_8(I first, I last) {
      typedef typename std::iterator_traits<I>::value_type T;
      typedef typename std::iterator_traits<I>::difference_type N;
      N n = std::distance(first, last);
      std::vector<T> buffer(n >> 3);
      sort_adaptive_n(first, n, std::less<T>(), buffer.begin(), N(buffer.size()));
    }


### Performance test

How does this one do?
Worse than before,
but we are also using about 10x less memory.

    Sorting double from 8 up to 2097152 elements generated with iota at: Mon Jun 28 21:32:08 2021
            size	stable	inplace	merging	1_8th	
               8	    13	    19	    13	    20	
              16	     7	    24	    12	    23	
              32	     7	    38	    17	    27	
              64	     9	    44	    17	    25	
             128	     9	    54	    21	    27	
             256	    12	    72	    25	    29	
             512	    15	    99	    29	    34	
            1024	    25	   129	    37	    41	
            2048	    36	   158	    48	    51	
            4096	    46	   181	    60	    62	
            8192	    56	   209	    71	    73	
           16384	    64	   225	    80	    83	
           32768	    93	   240	    84	    88	
           65536	    74	   261	    90	    92	
          131072	    78	   280	    97	   108	
          262144	    96	   314	   101	   104	
          524288	    91	   322	   107	   110	
         1048576	    99	   343	   113	   116	
         2097152	   102	   403	   120	   123

## Code

- [merge.h](code/merge.h)
- [test_sort.cpp](code/test_sort.cpp)
- [test_sort.h](code/test_sort.h)
- [test_temp_buffer.cpp](code/test_temp_buffer.cpp)


