16. Optimizing stable sort
==========================

## The measure of a good programmer

With algorithms and components you have to know the bunch.
It's like a toolchest.
A good woodworker doesn't just have one kind of tool.
You have a bunch of tools, whether making a door or whatever,
and then you find the right tools.
The *goodness, or best measure of a programmer
is the number of tools, or algorithms, he is fluent with*.
There is an erroneous view that good programmer is someone 
who can invent new algorithms.
This is good, but most people don't need to.
A carpenter doesn't invent new tools.
You can be an exceptionally good, or world class carpenter, without
ever introducing a new woodworking tool.
One of the things you need to know is existing tools.

I constantly hear, "I never needed __ algorithm".
This might be true. 
A carpenter doesn't use every tool in a given day.
Nevertheless, they have a large tool chest for those
cases when they need it.
Larger the tool chest, better off you are,
easier it is going to be to solve problems.
Moreover, if the tools are known to everyone,
they can understand what you do.

This is why I try to come up with a set of systematic components.
I think programming is a wonderful thing if you know many algorithms
and data structures and just use them.


## How good is our stable sort?

The merge sort algorithm we wrote last week is in 
many respects good code.
It's a good example of a stable sort.
It's a good basis on which we could get fast results and figure out how to make it very fast.
We will do measurements.

The current algorithm has `log(n)` recursive levels.
At every level we have a merge which is `O(n log(n))`.
so the overall complexity is `O(n log^2(n))`.
It's better than bubble sort and shell sort.
So how bad is it?
I made a test harness[^pc-info]:

    Sorting double from 8 up to 2097152 elements generated with iota at: Sun Jun 27 20:29:54 2021
        size	stable	inplace	
           8	    12	    18	
          16	     8	    25	
          32	     8	    35	
          64	     9	    40	
         128	     9	    49	
         256	    12	    66	
         512	    14	    94	
        1024	    23	   124	
        2048	    34	   150	
        4096	    46	   175	
        8192	    54	   197	
       16384	    63	   218	
       32768	    68	   234	
       65536	    73	   255	
      131072	    77	   276	
      262144	    87	   355	
      524288	    92	   321	
     1048576	   100	   343	
     2097152	   102	   364

Stable here is [`std::stable_sort`][cpp-stable-sort]
which is highly optimized library code.
So it tells us we can do better.
The numbers are nano-seconds per element.
Our merge sort is called `inplace`.
We start off ok, and then gradually get slower and slower in comparison.
Obviously, as the size increases you have to do more work.
It's not linear.
But, ours is clearly 3-4x slower than that.

[^pc-info]: AMD Ryzen 5 2400G (8 core, 3.6 GHz). GCC 9.3.0

[cpp-stable-sort]: https://en.cppreference.com/w/cpp/algorithm/stable_sort

## A plan for improvement

We are going to write a faster one.
I am going to work with the class but I will not always tell you
the truth.
Let us derive together and have a conversation but if you go on a wrong path I might not
necessarily stop you.
In real life I'm not going to be there to stop you when you write code.
So let us try together.

### Requirements

First we will decide our requirements.
These might be more strict than possible,
so we can remove them later if needed.

1. Maintain stability.

    Otherwise we would just write quicksort.

2. Requires only `ForwardIterator`.

    `InputIterator` is the only weaker thing, and it's not enough for
    sorting.

3. Use limited extra memory.

    The requirement of no extra memory at all is bogus.
    With modern systems we often have more memory than we need.
    But, we often have to parallelize stuff,
    so, we cannot put too much data on the processor.
    We want data to be able to fit in the cache.
    One way to do that is by exposing control.

4. Fast.

    What would be good?
    Within 5-10% of the `std::stable_sort`.
    What would be great?
    Observe STL also has [`std::sort`][cpp-sort] because it's faster.
    If it's as fast as sort, that would be great.[^dream].

[^dream]: Alex: It has been my dream for decades 
    to make my stable sort as fast as my sort,
    at which point I could throw away the other and just have one sort.
    But, I make progress with stable sort, then I make progress with sort.

[cpp-sort]: https://en.cppreference.com/w/cpp/algorithm/sort

### Ideas to explore

So we are going to try to make it fast, using all kinds of brilliant algorithm
ideas.
Let me tell you one great secret.
When designing algorithms there is a huge space of possible inputs.
You might have really big structure.
Or very expensive comparisons.
Should you concentrate on those?
No. First, make your algorithm
work really fast for what I call a common case, such as `double`.
You don't know how big integers are, but
the size of double is fixed for centuries to come,
by IEEE standard.
But, integers, doubles, and pointers are all good.
*Always make your algorithms as fast as possible
for the most common case*.
Then, you can look at bigger things and make other tradeoffs.

What ideas can we come up with?

**Using insertion sort for small cases**

In [insertion sort][insertion-sort] you build up a sorted list by inserting elements
one at a time.
One part of the list is always sorted.
We take an element which is not inserted,
and linear search to find where it belongs.

    [  sorted       | not sorted ]

What is the complexity of insertion sort?
It's `O(n^2)`. 
What is the complexity of merge sort, in terms of comparisons?
`O(n log(n))`.
What is the specific coefficient for insertion sort?
The sorted portion is on average half the length of the original input.
In addition, on average when we an insert an element we
only have to go half the length before finding its location.
Therefore the complexity
is roughly: `n^2/4`.

Now we will solve for when it's better to do one than the other.

    n^2/4 < n log(n)
        n < 4 log(n)
      2^n < n^4

So the cutoff is `n = 16`:

    2^16 = 65536 = 16^4

That's what theory tells us.
What should we really do?
We should measure.
But, the theory tells us around which numbers to try (around 16).

[insertion-sort]: https://en.wikipedia.org/wiki/Insertion_sort

**Binary insertion sort**

We can also binary search to find where to insert the element.
That will also make the cutoff point larger.
Is it going to be dramatically better?
In insertion sort, we already have to do a linear complexity
shift to make room.

**Get rid of recursion**

Perhaps we can use `binary_counter`.

**Write an excellent merge**

Basically we need an algorithm that will say, 
"do this version if we have enough memory,
otherwise do that".
The term is *adaptive*.

You might think everybody can write a good merge.
If you Google "std merge" it shows you [this][merge-code]:
Since you're a normal programmer
you might say, "oh it's on the Web, therefore I can copy and paste and use
it in my code." 

    template <class InputIterator1, class InputIterator2, class OutputIterator>
    OutputIterator merge (InputIterator1 first1, InputIterator1 last1,
        InputIterator2 first2, InputIterator2 last2,
        OutputIterator result)
    {
      while (true) {
        *result++ = (*first2<*first1)? *first2++ : *first1++;
        if (first1==last1) return std::copy(first2,last2,result);
        if (first2==last2) return std::copy(first1,last1,result);
      }
    }

This is a broken piece of code.
An empty range will melt the computer.

[merge-code]: https://web.archive.org/web/20130812111552/https://www.cplusplus.com/reference/algorithm/merge/

**Faster rotate**

What is the complexity of `std::rotate`?
It's tricky, because it depends on the kind of iterators which you have.
With `RandomAccessIterator` the best theoretical algorithm
does `n + gcd(n1, n2)` assignments, not swaps.
On average GCD is small, but larger than one.
So we can get almost to `n` assignments, which is a lot better than `n` swaps.
For `ForwardIterator` it happens to be `n - gcd(n1, n2)` swaps.
It is roughly `n` for `BidirectionalIterator`.

As we will observe, we can use a faster rotate than the rotate in STL because we have this additional storage.
If you want to rotate and you have enough storage, then you only need `n + 1` assignments which is for sure less than `3n`.

### First steps

There's lots of things to do.
How should we go about it?
The problem with programming, specifically designing components
and decomposing the system is that you do not know what is right in isolation.
You never know what the correct interface is
until you see it in other algorithms,
and you see how those are used.
This is why you just have to try things, and ideas start emerging.
You might think it's an infinite process.
No it's not infinite, that's the wonderful thing about life.
It sort of terminates (I cannot prove it of course).
In practice if you start fitting things together you sort of discover what
you need to return,
what you need to pass,
what is the right thing to do, and that's what
I am trying to teach.

When should we try insertion point?
As a rule we want to fix the asymptotic complexity.
Doing insertion sort at the bottom won't help that.
Right now we have a problem with our asymptotic complexity.
It's `O(n log^2(n))`. We want to get rid of that square
as fast as possible.

I'm very lazy.
So, we saw how fast we can get using no memory.
What about if we had infinite memory?
How much extra memory do I really need?
I think we can do it in `n/2`.

