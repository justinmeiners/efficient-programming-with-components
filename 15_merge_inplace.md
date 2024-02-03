15. Merge inplace
===================

## Reinventing things

Another sad story (like the fish story)
is finding the algorithm for merging sorted lists.
This is a notoriously hard problem.
Many people worked on it.
The first solution was done by a Russian computer scientist
[Alexander Kronrod][kronrod],
but it wasn't stable.
There was another solution by a wonderful Argentinian computer scientist [Luis Pardo][pardo] (Knuth's student).

So, I was thinking and thinking and thinking and once I was waking up in the phase between Twilight,
when you wake up but still see dreams but you're not quite asleep.
I saw the algorithm on the board, it does happen.
I was ecstatic.
I think it was 1984.
What do you do if you find a really beautiful algorithm?
If you're me, you call your friends.
So I called [Dave Musser][musser] and said, "this is absolutely gorgeous" and he agreed.
I started implementing and doing measurements.
Meanwhile, he starts telling faculty around him.

Here comes the bad news.
One of his colleagues, [Erich Kaltofen][kaltofen] who is a distinguished specialist in computer algebra, came to Dave and said
"Yeah it's nice, but there are these two Polish guys Dudzinski and Dydek who published it in 1981[^on-a-stable-minimum-storage-merge] (two years before)."
It was very sad.
But, we often reinvent things.
Meanwhile Knuth heard about it from his friend [Vaughan Pratt][pratt] who told him about it.
So he gives the attribution to his friend, and not the two polish guys[^merge-sort-exercise].

As an algorithm itself, it's utterly useless.
But sometimes algorithms published by theoreticians can be used by us, after appropriate modification.

[^merge-sort-exercise]: See exercise 5.5.3 (Pg. 390) in Volume 3 of "The Art of Computer Programming".
    The algorithm itself and the attribution to Vaughan Pratt is in the solution (Pg. 701).

[^on-a-stable-minimum-storage-merge]: ["On a Stable Minimum Storage Merging Algorithm"](papers/on-stable-merge.pdf). Information Processing Letters. 1981.

[kronrod]: https://en.wikipedia.org/wiki/Alexander_Kronrod
[pardo]: https://www.genealogy.math.ndsu.nodak.edu/id.php?id=47194
[musser]: https://en.wikipedia.org/wiki/David_Musser
[kaltofen]: https://kaltofen.math.ncsu.edu/
[pratt]: https://en.wikipedia.org/wiki/Vaughan_Pratt

## Merging adjacent lists

We got into all of this because we were thinking about merge. 
We already learned to merge linked list (chapter 12).
I introduced this thing called `goto`.
Today I'm going to teach you some other bad programming practices because it's always
pleasant to introduce techniques which are known to be bad in general when
they're good in particular.

We're going to go about things in a funny way.
Normally when I teach merge,
we then realize it can be used for sorting.
We write merge sort and then we realize that merge sort needs extra memory,
and say "oh it's very unfortunate. Couldn't we find merge sort
that doesn't require extra memory?"
This time around, I decided to follow a different path.
At first, we are going to avoid normal merge.
We're going to start with very beautiful, but somewhat slow code.
Sometimes it's actually good to start with slow code and refine it
into fast code.
Especially if it allows you to create something architecturally nice
and see connections.
So we are going to look at the much harder problem of in-place merge.
The problem is actually hard if you think about it.

### Interface

What is the interface?
It's not trivial.
Obviously if you have two separate lists/vectors,
you can't really merge in place.
So the interface assumes they are adjacent.
One thing you could do is just sort it.
There are problems.
Most sorts, besides merge sort,
are not stable, so stability will be lost.

For bounded (not counted) it will take three iterators:

    first, middle, last

But our algorithm will greatly benefit from counted ranges.
When do we need counted ranges? When we do bisection,
or something like binary search.
We will use two of them:
 
    [first1, count1)
    [first2, count2)

We have a precondition that `distance(first1, first2) == count1`.
Another precondition is that each range is sorted,
and our post condition is the entire range will be sorted.

### Algorithm

If you don't know how to do something, the old
advice is to look for divide and conquer.
The following is a graph of the two sorted ranges adjacent to each other.
The graph for each moves up and to the right to illustrate they are ascending.
When we have lots of variables naming doesn't work.
We have to use one letter names with indices, like math.

        n0     n1
       /      /
      /      /
     /      /
    /      /
    f0     f1

We will first bisect one of the ranges and pick a guy from the middle.
Then we ask, "where would it fit in the other sequence?"
Do we have a function for that?
We do. It's called `lower_bound`.

Assume we bisect the left.
Then let `f0_0 = f0` and `f0_1` be the bisection of the first interval.
Then `f1_1` is found from the right using `lower_bound`.
(that should take `O(log(n))` comparisons).

        n0     n1
       /      /
      /      /
      f0_1   f1_1
     /      /
    /      /
    f0_0   f1

Now we are going to rotate.
Rotating swaps elements in the range
`[f0_1, f1_1)` 
in such a way that `f1` becomes the first.
`[f0_0, f0_1)` and `[f1_1, n1)` remain fixed (see [`std::rotate`][cpp-rotate]).

                    n0     n1
                    /      /
                   /      /
                   f0_1  f1_1
      
     /     /
    /     /
    f0_0  f1

Now we know `f0_1` is in his rightful place (all lower are to the left and all greater are to the right),
so we will let `x = f0_1`,
as he won't be moved again 
So, we shrink the range by one step by assigning `f1_0 = f0_1 + 1`,
and then just figure out the lengths of the remaining
intervals:

                      n1_0     n1_1
                      /       /
                    f1_0     /
                    x      f1_1
     n0_0   n0_1
     /     /
    /     /
    f0_0  f0_1

Now notice that we have 4 adjacent sequences,
each of which is individually sorted, so we
can apply the algorithm recursively.

Here is the other insight.
Which range should we bisect?
The smaller one, because
we want to get to empty range as fast as possible.
So if it's on the left we should use `lower_bound`
If it's on the right we should use `upper_bound`.
Why?
To preserve stability we need to make sure equal
guys don't jump over each other.


[cpp-rotate]: https://en.cppreference.com/w/cpp/algorithm/rotate


### Implementation

If you write a function like this it has to return
a lot of stuff.
Here comes the solution which we are proud of,
which is returning things through the reference arguments.
Here we take 2 ranges,
and return 4 of them.

    template <typename I, typename N, typename R>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I
    inline 
    void merge_inplace_left_subproblem(I  f0,   N  n0,
                                       I  f1,   N  n1,
                                       I& f0_0, N& n0_0, 
                                       I& f0_1, N& n0_1, 
                                       I& f1_0, N& n1_0, 
                                       I& f1_1, N& n1_1,
                                       R r) {
      // precondition std::distance(f0, f1) == n0
      // precondition is_sorted_n(f0, n0, r) && is_sorted(f1, n1, r)
      // precondition n0 > 0
      // precondition n1 > 0
      f0_0 = f0;
      n0_0 = n0 >> 1;
      f0_1 = f0;
      std::advance(f0_1, n0_0);
      f1_1 = lower_bound_n(f1, n1, *f0_1, r);
      f1_0 = std::rotate(f0_1, f1, f1_1);
      n0_1 = std::distance(f0_1, f1_0);
      ++f1_0;
      n1_0 = (n0 - n0_0) - 1;
      n1_1 = n1 - n0_1;
    }

Note that this `std::rotate` is the C++11 version which returns an iterator
rather than `void`[^rotate-return].

Now implement the right sub-problem,
it is basically the same idea.
The fundamental difference is upper bound,
and switching sides.

    template <typename I, typename N, typename R>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I
    inline
    void merge_inplace_right_subproblem(I  f0,   N  n0,
                                        I  f1,   N  n1,
                                        I& f0_0, N& n0_0, 
                                        I& f0_1, N& n0_1, 
                                        I& f1_0, N& n1_0, 
                                        I& f1_1, N& n1_1,
                                        R r) {
      // precondition std::distance(f0, f1) == n0
      // precondition is_sorted_n(f0, n0, r) && is_sorted(f1, n1, r)
      // precondition n0 > 0
      // precondition n1 > 0
      f0_0 = f0;
      n0_1 = n1 >> 1;
      f1_1 = f1;
      std::advance(f1_1, n0_1);
      f0_1 = upper_bound_n(f0, n0, *f1_1, r);
      ++f1_1;
      f1_0 = std::rotate(f0_1, f1, f1_1);
      n0_0 = std::distance(f0_0, f0_1);
      n1_0 = n0 - n0_0;
      n1_1 = (n1 - n0_1) - 1;
    }

Now we combine them in a function that does no work.

    template <typename I, typename N, typename R>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    void merge_inplace_n(I  f0,   N  n0,
                         I  f1,   N  n1,   R r) {
      // precondition std::distance(f0, f1) == n0
      // precondition is_sorted_n(f0, n0, r) && is_sorted(f1, n1, r)
      if (!n0 || !n1) return;
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

      merge_inplace_n(f0_0, n0_0, f0_1, n0_1, r);
      merge_inplace_n(f1_0, n1_0, f1_1, n1_1, r);
    }

[^rotate-return]: Alex: When I first put `std::rotate` in STL it returned `void`.
    In 1995 I discovered what it should return and how to do it efficiently.
    When you rotate you return what the new middle is.
    It took literally 20 years.

### Naming things

There is a function in STL incorrectly called [`std::inplace_merge`][cpp-inplace]
Why is the name incorrect?
Paul discovered it while working
on the index to our book (Elements of Programming) and discovered
a general principle which escaped me for decades.
"inplace merge" 
is a bad name for a function?
Why it should be `merge_inplace`.
It should come next to merge in the index.
People who look into the index want to see all the versions.
Am I a total idiot?
I guess, I did not see it.

I'm just emphasizing a very important naming principle
which is to think about indexing. 
think about indexing things you know like
`find_if`, not `if_find`.
Suffixes should be sorted in the order of importance.

[cpp-inplace]: https://en.cppreference.com/w/cpp/algorithm/inplace_merge

## Sort from merge

Now we can sort, using our merge mechanism.
It's really easy.
The first statement is that an empty list is sorted.
The second is that a one element list is sorted.
Now, we just call it recursively and ask
it to sort itself.
Nobody does any work except `merge_inplace_n`[^inductive-algorithm].

    template <typename I, typename N, typename R>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    I sort_inplace_n(I first, N  n, R r) {
      if (!n) return first;
      N half = n >> 1;
      if (!half) return ++first;
      I middle = sort_inplace_n(first, half, r);
      I last   = sort_inplace_n(middle, n - half, r);
      merge_inplace_n(first, half, middle, n - half, r);
      return last;
    }

It's a good algorithm.
It's stable.
It uses no extra storage.
Whether that's really needed or not, we don't know.
It has `log(n)` levels.
At every level we have a merge which is `O(n log(n))`.
so the overall complexity is `O(n log^2(n))`.
It's better than bubble sort and shell sort.

There is another strength of this algorithm,
which you won't know unless you think about generic programming.
What's the requirement on the iterator?
Just forward iterator.
We can sort anything.

But, it also sucks.
Where does it suck? Deep down in the call tree.
At the top it's good.
But, we are doing too much work.
We will talk more about this in the next
lesson.

**Exercise:** The requirement to use no extra storage is a little unrealistic.
    Try to write a version that is similar, but utilizes extra storage.

[^inductive-algorithm]:
    If you are not familiar with recursion or mathematical induction
    this kind of code can look tricky.
    The key property is that every iteration the problem space is made smaller,
    in a way that it preserves essential properties of the original problem.

    In this case, we assume we can sort both halves. That wouldn't solve the
    problem normally, except that the merge algorithm gives us a way to combine them
    to get a solution for the full input space.



## Code

- [merge_inplace.h](code/merge_inplace.h)
- [test_merge_inplace.cpp](code/test_merge_inplace.cpp)
