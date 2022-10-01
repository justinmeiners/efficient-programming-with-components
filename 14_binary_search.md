14. Binary Search
==================

## Bisection in math

Bisection is an idea which is pretty ancient.
It was first used in a mathematical setting by [Lagrange][lagrange] who applied it for
finding roots of polynomials, that was around 1796.
Two people independently discovered it
in late 1810s.
The first is [Bernard Bolzano][bolzano]
the second is [Augustin-Louis Cauchy][cauchy].
They both invented the very famous theorem which is called [bisection][bisection].
Where does it appear in mathematics?
The [intermediate value theorem][ivt], if you have a [continuous function][continuous-function] `f` which goes from negative to positive, it has to cross zero.

                    ____f(b) > 0
                   /
                  /
            0 -- / --------
                /
     f(a) < 0 \/
               

     
How do you find zero by bisection?
You divide the interval `[a, b]` in half, and you see if the new endpoint
is above or below, then you shrink the interval:

    [a   c)    b]

`0` is on the left side if `f(c) >= 0` and otherwise
`0` is on the right side.
It is a great idea. 

[lagrange]: https://en.wikipedia.org/wiki/Joseph-Louis_Lagrange
[bolzano]: https://en.wikipedia.org/wiki/Bernard_Bolzano
[cauchy]: https://en.wikipedia.org/wiki/Augustin-Louis_Cauchy
[bisection]: https://en.wikipedia.org/wiki/Bisection_method
[bolzano-theorem]: https://en.wikipedia.org/wiki/Bolzano%E2%80%93Weierstrass_theorem
[ivt]: https://en.wikipedia.org/wiki/Intermediate_value_theorem
[continuous-function]: https://en.wikipedia.org/wiki/Continuous_function

## Partitions

We are still struggling on our path to binary search[^pauls-friend].
It deals with two things.
First, it deals with a monotonically non-decreasing sequence.
Second of all it has some
binary predicate which establishes `StrictWeakOrdering`
on the sequence and which allows you to compare elements.
This is too much, even for people who design APIs.
In order to write it correctly we need to reduce it to a simpler problem.

Even simpler than the notion of a sorted sequence
is a **partitioned sequence**.
A sequence is partitioned based on a predicate.
A sequence is partitioned if the predicate is true for some sub range of items, and then false for the rest[^order].

### Is partitioned

First we want a function to tell us
if a sequence is partitioned.
Who should go first? Good guys, or bad guys?
This is a tricky thing and STL gets it wrong.
When I did STL I thought good guys should naturally
come before bad.
Satisfying is good.
Don't you want satisfying things?

But it's wrong.
We want partition sequence to be sorted on the boolean values and since STL
assumes that ascending order is natural,
The right thing to do is make partition consistent[^changing-standard-difficult], false values go first.
But, for our course we will follow the standard.

    template<typename I, typename P>
    // I is InputIterator
    // P is unary predicate 
    // value_type of I == argument_type of P
    inline
    bool is_partitioned(I first, I last, P pred) {
      first = find_if_not(first, last, pred);
      first = find_if(first, last, pred);
      return first == last;
    }

Now for bounded ranges:

    template<typename I, typename N, typename P>
    // I is InputIterator
    // P is unary predicate 
    // N is integral type
    // value_type of I == argument_type of P
    inline
    bool is_partitioned_n(I first, N n, P pred) {
      std::pair<I, N> p = find_if_not_n(first, n, pred);
      p = find_if_n(p.first, p.second, pred);
      return p.second == N(0);
    }


[^changing-standard-difficult]: Alex: Changing something in this standard is impossible,
    because they just don't listen to arguments.
    Whatever arguments you give them, they just say, "It's the standard".

### Partition point

When we partition we will have true guys followed by false:

    T T T F F F
          ^

There is only one special thing, the partition point.
If we understand the partition point everything else will be very simple and there is no ambiguity.
`find_if` actually finds the partition point.
But, it does too many predicate applications.
We could do better if our range is at least forward iterable.

Even for singly linked lists such a thing is useful, because
if your predicate is very expensive,
while iteration is relatively fast[^slow-comparison-example],
you still could reduce the number of tests to `log(n)`.
As we shall see we have a very good bound on the number of traversal
operations which is literally `n`, not order of `n`.
So we can get it so it works for everything.
Then it works on arrays much much faster than linked lists. 

A distinguished computer scientist recently asked me, "what if we just make it linear. will it really affect practical algorithms".
The answer is yes, very badly.

The algorithm to find it faster is to test the middle.
How do we go to the middle?
Divide by 2.
Dividing numbers is easier so we will start with a counted range, instead of bounded.

    template<typename I, typename N, typename P>
    // I is ForwardIterator
    // P is UnaryPredicate 
    // N is integral type
    // value_type of I == argument_type of P
    inline
    I partition_point_n(I first, N n, P pred) {
      while (n) {
        N half = n >> 1;
        I middle = first;
        std::advance(middle, half);
        if (pred(*middle)) {
          ++middle;
          first = middle;
          n -= (half + 1);
        } else {
          n = half;
        }
      }
      return first;
    }

Why did I use a shift (`n >> 1`)? We know it's non-negative.
I'm a penny pincher.
Maybe the compiler will automatically do it for `n / 2` maybe it will not.
Now it will.

How many `++` operations do we do?

    n/2 + n/4 + ... + = n.

We are traversing more than linear search on the average case.
We are also not trying to be lucky and find equal.

    template<typename I, typename I, typename P>
    // I is ForwardIterator
    // P is UnaryPredicate 
    // value_type of I == argument_type of P
    inline
    I partition_point(I first, I last, P pred) {
      return partition_point_n(first, std::distance(first, last), pred);
    }


[^pauls-friend]: Alex: Paul McJones has a good friend [Butler Lampson][lampson] who is a Turing award winner.
    We went to lunch and he told us binary search is the 
    only algorithm a programmer needs to know.
    I think sort should be there too, but we'll take his opinion.
 
[^slow-comparison-example]: In writing my Master's thesis
    I actually came across a comparison operator which is very expensive to evaluate,
    called the Dehornoy ordering for [braid groups][braid-research].
    This provides a total ordering which I used for sorting, removing duplicates, and other algorithms in STL style.
    In this case having
    algorithms that carefully optimized the number of comparisons made a significant different in performance.

[lampson]: https://en.wikipedia.org/wiki/Butler_Lampson
[braid-research]: https://github.com/justinmeiners/braid-rank-thesis

## Upper and lower bound

We need to talk a little bit about sorted ranges.
A precondition to binary search is not that the range is partitioned,
but it is sorted.
Again, we have a range and it could be counted or bounded
How do we know a range is sorted?
If you say *monotonically increasing*
then you would be wrong.
*Non decreasing* things are sorted.
Increasing is too much.
If I want to sort my coins and there are
two pennies however much I want to sort,
I'm not going to have an ascending sequence.
What we need to guarantee is that for no pair of elements
`x_i`, `x_j` where `j > i`
that `x_i > x_j`.

But, that's too much to check.
We need to rely on this amazing property that if we check adjacent elements and
the property holds,
then it will hold for all the elements.
Why? Because of transitivity.
This again shows mathematics is actually important.
If you have a non-transitive relation it might not work.
You might say, "well everybody knows that".
I could give you examples of famous people who do not quite get it.
We have transitivity from a requirement of `StrictWeakOrdering`.

### Is sorted

Let's write `is_sorted`.
We know an empty range is sorted.
As long as we didn't reach the end, and we didn't
find a counterexample (not suddenly decreasing)
we can go on.

    template <typename I, typename R>
    // I is ForwardIterator
    // R is WeakStrictOrdering on the value type of I
    inline
    bool is_sorted(I first, I last, R r) {
      if (first == last) return true;
      I previous = first;
      while (++first != last && !r(*first, *previous)) previous = first;
      return first == last;
    }

When you design a library for yourself, friends, or the world, provide all the versions. 
So if you have a comparison operator, provide a version where you don't need
to pass the relation:

    template <typename I>
    // I is ForwardIterator with a totally ordered value type
    inline
    bool is_sorted(I first, I last) {
      typedef typename std::iterator_traits<I>::value_type T;
      return is_sorted(first, last, std::less<T>());
    }

There are a few choices which humankind forced upon us.
Increasing is better than decreasing. 
1, 2, 3. Not 3, 2, 1.
Natural numbers go up.
The second choice is if we should use `<` or `<=`,
strict or non-strict.
I made a choice that `<` is the primitive one.


**Exercise:** There is an STL function called [`std::adjacent_find`][cpp-adjacent-find]
    and there is a profound relation between `is_sorted` and `adjacent_find`
    but you're going to discover it yourself.
    Write a program that uses `std::adjacent_find` and 
    try to figure out this relationship.

[cpp-adjacent-find]: https://en.cppreference.com/w/cpp/algorithm/adjacent_find

## Binary search with partition points

How do we get from partition point to lower bound?
Let us assume we have a sorted range, of something.
Somebody important comes with an element `a` in the same domain.
I claim there are two points
which `a` establishes in this sorted sequence.
Let us assume that they are integers and `a = 5`.
`5` gives us two iterators in this sequence.
The beginning of the range containing `5` and the
end of the range,
and it could be empty.
Consider:

    2 3 5 5 6 7
        ^   ^

We get iterators pointing to the first `5`,
and just beyond.
How could we describe the property of the first iterator?
We have a dichotomy.
Everybody to the left is less than `5`.
In other words, it is the partition point[^partition-point].
This is why we don't want equal because equal leads
to all these bad binary search that return `-1`,
because they only look for `5`. 

As far as I could ascertain, these names are invented by me but I think they're good names,
`upper_bound` and `lower_bound`, there are two.
So what property does the upper bound have?
It is the first element which is greater.
Both lower bound and upper
bound split our range into two ranges.
So in some sense we actually have 3 ranges:

    [first [lower)   [upper)  last)

     1. [first, lower)
     2. [lower, upper)
     3. [uppper, last)

You can actually find them both together
a little faster, than separately.
There is a function [`std::equal_range`][cpp-equal-range]
which does that.

[^partition-point]: Framing binary search as finding the partition point solves an important theoretical problem.
    Suppose you want to search an array of records by a particular field.
    You can of course make a record having arbitrary values for all the other
    fields besides the one you care about,
    but it would be nice to provide just the key you want.
    For example:

        binary_search(
            people.begin(),
            people.end(),
            "bob",
            [](Person a, const char* name){ a.name < name }
        );

    But what is the theoretical basis for a function comparing a key to a person record?
    It's not a `StrictWeakOrdering` as  `Name` and `Person` are not elements of the same domain.
    In this case the comparison function is no longer an ordering at all, or even an operation.
    If we condense the key and comparison into a predicate, and find the partition point, then this problem goes away.

    It appears the C++ standards committee was confused about this for some time.
    See ["Binary Search with Heterogeneous Comparison"][binary-search-standards-1]
    and ["Binary search requirements overly strict"][binary-search-standards-2] for further discussion.

[binary-search-standards-1]: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2001/n1313.html
[binary-search-standards-2]: https://cplusplus.github.io/LWG/issue270
[cpp-equal-range]: https://en.cppreference.com/w/cpp/algorithm/equal_range
[cpp-binary-search]: https://en.cppreference.com/w/cpp/algorithm/binary_search

First let's implement a function object
which is our predicate for partitioning.
It is defined by `P(x) = a < x`
for a fixed element `a` and a comparison operator.

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

Now we can write `lower_bound`, which is the version
of "binary search" that we want[^binary-search]:

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

We implement similar functions for `upper_bound`
found in the final code.


[^binary-search]: Alex: If you remember there was this
    grand review one day where the committee threw out a bunch of useful functions.
    Well they inserted some too.
    One is called [`std::binary_search`][cpp-binary-search].
    A friend asked me, "where is binary search?"
    I explained all this `upper_bound`, `lower_bound`, stuff
    to him,
    and he said, "but, where is binary search?".
    So, I did it for him.
    Who can argue with a best friend.
    Will I ever use it? No.

## Project: Partitioning

So we have worked a lot with partitions, but
how do we actually partition a range?
A few lessons ago we saw the first example of sorting.
We are sorting linked lists, and we are sorting them well.
We will see many other examples of sort and sort like function.
Often people 
have an array, or a list, or whatever they like,
and they want to distinguish between good things and bad things.
So, they say, "how will I do this? I'll sort".
This is not necessarily a good solution because they are solving a simple problem,
the problem of partitioning a sequence, with the help
of expensive `n log(n)` algorithms.

I want you to think about this partitioning,
especially in terms of our wonderful `binary_counter` device.
However, we also want the partition to be **stable**.
You want to move all the bad guys up front and good guys to the tail end.
You will return an iterator pointing to the partition point
separating good from bad.
This partition is *stable if the relative order of good guys and bad guys
is maintained*,
meaning if I have an alphabetically sorted list of employees and I
want to divide them by say gender,
if I partition them, they will still be alphabetically sorted.

Of course, you don't have extra memory
I claim our marvelous device will allow you to do it.
But, we have to figure out how.
If you try to use something like [bubble sort][bubble-sort].
It won't give you a very good solution.
It will be stable, but it's quadratic.

Start with linked list where the problem is easy,
then try to do it on an array.

[bubble-sort]: https://en.wikipedia.org/wiki/Bubble_sort

## Code

- [partition.h](code/partition.h)
- [test_binary_search.cpp](code/test_binary_search.cpp)



