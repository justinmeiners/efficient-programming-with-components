18. Binary insertion sort
===========================

## The Organ Grinder

I would like to occasionally introduce three minutes of culture.
I used to tell you stories but right now I decided
I'll just occasionally share a song or something like that
which would indicate what mood I'm in.
This is a very great song by [Franz Schubert][schubert] but it also
perfectly reflects what will go on with the course in how I feel.
The song is called ["The Organ Grinder"][winter-journey] (Der Liermann).
The singer is [Dietrich fischer-dieskau][dietrich] maybe the greatest
leader, or art song singer of the last 50, 60, or 70 years.
He started singing in the late forties.
Let us spend a couple of minutes and listen to it... ([Video here][organ-grinder])

[schubert]: https://en.wikipedia.org/wiki/Franz_Schubert
[dietrich]: https://en.wikipedia.org/wiki/Dietrich_Fischer-Dieskau
[organ-grinder]: https://www.youtube.com/watch?v=sIIS-UgixGE
[winter-journey]: https://www.britannica.com/topic/Winterreise

## Strategy

Let's try our [insertion sort][insertion-sort] idea.
We are going to learn more about insertion sort then 
you ever wanted to know.
First we will review the basic idea of algorithm.
Always start with a picture:

    |   sorted piece |  unsorted piece |

We start with an empty range on the left which is the sorted portion.
We basically want to grow it, one element at a time,
while ensuring it stays sorted.
By repeating it inductively, eventually the whole range is sorted.
So, main idea is to pick an element in the unsorted piece,
find where the element goes, and insert it there.

### Insertion sort variations

How many algorithmic versions of insertion sort are there?
Finding where it should go could be done with either:

1. Linear search
2. Binary search

There is another version which was invented,
as was everything else, by [Tony Hoare][hoare].
He realized that in the inner loop of
insertion sort you have to do two things:

1. You have to guard that you're not crossing by size 
2. Guard that you're not crossing the first

This makes the insertion sort do two comparisons per cycle.
You could have an insertion sort with a guard,
assume that somebody puts (by hook or by crook)
the smallest element first.
In other words, if you can guarantee
smallest is up front, then you can simplify the inner loop, just going through the algorithm.
Since we wrote the machinery for binary search in previous lessons,
let's start by writing that.

[hoare]: https://en.wikipedia.org/wiki/Tony_Hoare

### When is insertion sort useful?

When should one use insertion sort?
This is an interesting point we should discuss.

1. We already talked about when `n` is small.
  How small? We already proved it was when `n = 16`.
  Is it the exact? No, it's not.
  But, it's a good rule of thumb.

2. If we just have a few things
  to add to a sorted list, that would be good.
  In other words, most of the list is sorted,
  but 16 or so elements are out of order.

3. Insertion sort is going to move an element
  from where it is, to where it should be,
  one step at a time.
  So another case is when the average distance
  from where it is, to where it should be,
  is small.
  It's "nearly sorted".

There are some considerations where you want to look at the relative cost but they are not important for asymptotic assessment.
A quadratic algorithm,
regardless of the ratio between move and compare
is still a quadratic algorithm.

### Naming insertion sort function

Unfortunately, STL does not have insertion sort.
Should it?
Yes, it should.
But, they threw it out from the public library[^sorts-in-stl].
At least put it in your library.
It might not be called insertion sort.
Maybe we should call it something else.
What's a good name?
This is not a bogus question.
Finding a good name is important,
because we want to lead people to use it when
these three conditions are met.
Maybe, `sort_almost_sorted`.
Only few of them are out of order or it's just perturbed
everywhere, but not by much.
However, we will find it works well in that case only when the thing
to sort isn't enormous.
So instead we will settle on `binary_insertion_sort`.

Naming is extremely hard, but very important.
The goal is to name components so people can actually understand
what they mean, it helps people.
We have to discuss nomenclature.
Respectable sciences spend most of their time discussing 
nomenclature.
Chemists, physicists, they know what to call things.
It's only computer science that doesn't.

I have to tell you a story [Sean Parent][parent]
shared with me.
When STL was introduced, people at Apple decided to try it.
They tried it and found it absolutely unacceptable because they replaced their `list` with STL
`std::list` and everything became extremely slow.
The problem is their list
was what is still called a vector.
They didn't realize linked lists are called "linked lists".
It sort of works, you know, slowly.

[insertion-sort]: https://en.wikipedia.org/wiki/Insertion_sort
[parent]: https://sean-parent.stlab.cc/papers-and-presentations/

[^sorts-in-stl]:
    Alex: Of course, STL still has insertion sort on the inside.
    It has to.
    What happened during the standardization process,
    is they took something which was in the library and was used by the library, and threw it out.
    The argument was, "we already have too many sorts".
    Is it a good argument?
    No, you need to have as many sorts as people might need
    when they do practical things.
    How many sorts are in STL?

    1. [`std::sort`](https://en.cppreference.com/w/cpp/algorithm/sort) the fastest sort.
    2. [`std::stable_sort`](https://en.cppreference.com/w/cpp/algorithm/stable_sort), this is merge sort, the one we are trying to write.
    3. [`std::partial_sort`](https://en.cppreference.com/w/cpp/algorithm/partial_sort) sort the first thousand, out of a million,
        something you frequently do in search engines.
    4. [`std::nth_element`](https://en.cppreference.com/w/cpp/algorithm/nth_element).
        Not quite a sort, but it's sort related.
        What it does is pin, for example the 30th percentile
        element, and put all the smaller before, and then all the larger.
        If I want to find another one, I can pin again, and sort inbetween, etc.

## Binary insertion sort

First, let's see how we will use it,
specifically where we can stick it in our code.
We added a minimum size for which our merge switches
algorithms and uses the extra buffer.
Now, we can add a similar check to our sort to switch
algorithms.

    const size_t INSERT_SORT_CUTOFF = 16;

    template <typename I, typename N, typename R, typename B>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    I sort_adaptive_n(I first, N n, R r, B buffer, N buffer_size) {
      if (!n) return first;
      if (n < INSERT_SORT_CUTOFF) return binary_insertion_sort_n(first, n, r); // HERE
      N half = n >> 1;
      if (!half) return ++first;
      I middle = sort_adaptive_n(first, half, r, buffer, buffer_size);
      I last   = sort_adaptive_n(middle, n - half, r, buffer, buffer_size);
      merge_adaptive_n(first, half, middle, n - half, r, buffer, buffer_size);
      return last;
    }


(Recall, that we proved 16 was a good cutoff.)
The standard C convention for old people is that `ALL_CAPS` means it's a macro.
We will use this for a constant here[^macros-comment].

We have a few decisions to make for insertion sort.
Should we use `upper_bound` or `lower_bound` for binary search?
If we use `lower_bound` and we have equal guys at the end,
then it will move too far beyond them.
So we want `upper_bound`.

Since our sort only requires `ForwardIterator`,
we should aim to make insertion sort use `ForwardIterator`.
Using `ForwardIterator` is actually a piece of cake for the binary search,
but we have to be careful.
You might want to use the `upper_bound` we wrote together.
But, remember it calls `std::distance` which is linear for `ForwardIterator`.
So let's use `upper_bound_n`

What we will first write is a function for finding where an element
goes and placing it there.
Then we will structure our loop[^for-loop] around that.

    template <typename I, typename N, typename R>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    I binary_insert_n(I first, N n, I current, R r) {
      // precondition: is_sorted(first, current, r) && current is a valid iterator
      //            && std::distance(first, current) == n
      I insertion_point = upper_bound_n(first, n, *current, r);
      rotate_right_by_one(insertion_point, ++current);
      return insertion_point; 
    }

`rotate_right_by_one` will be discussed in just a bit.
It's important to return here, in case someone
else wants to use this function.

To write the loop that calls this function,
I suggest that we carefully write invariants.
We have the range:

    [first, i)

It's semi-open, so `i` is past the end.
What we want is:

    is_sorted_n(first, i, r) && std::distance(first, current) == i

That's the invariant on which we rely.


    template <typename I, typename N, typename R>
    // I is ForwardIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    I binary_insertion_sort_n(I first, N n, R r) {
      if (n == N(0)) return first;
      I current = first;
      ++current;
      N i(1);
      while (i < n) {
        // invariant: is_sorted_n(first, i, r) && std::distance(first, current) == i
        binary_insert_n(first, i++, current++, r);
      }
      return current;
    }

[^macros-comment]: Alex:
        I think you should read Bjarne's book "Design and Evolution of C++".
        It's a short book and very instructive.
        This is the book I hope he will revise, because it only goes up to like 92.
        In any case, the last chapter is dedicated to the preprocessor.
        It has an epigraph: "CPP must be destroyed" - Cato.
        [*Carthago delenda est*][latin-carthage] ("Carthage must be destroyed").
        It's still not destroyed, but maybe one day.
        

[latin-carthage]: https://en.wikipedia.org/wiki/Carthago_delenda_est
            

## Rotate


### Rotate for bidirectional iterators

Once we find where it goes, how do we make room for it?
We "rotate" to the right by one.
If it was a bidirectional iterator there is a beautiful algorithm.
Copy is the wrong thing, because it will overwrite everything
with the same value.
What we want is copying from the back.
The algorithm is called: [`std::copy_backward`][cpp-copy-back].

    template <typename I>
    // I is BidirectionalIterator
    void rotate_right_by_one(I first, I last, std::bidirectional_iterator_tag) {
      typedef typename std::iterator_traits<I>::value_type T;
      T butlast = last;
      --butlast;
      T x = *butlast; 
      std::copy_backward(first, butlast, last);
      *first = x;
    }


### Rotate for forward iterators

For forward iterator we have to shift all the elements up,
we move one out of the way, to make room,
and continue up the array until we find an empty place to put it.

I think the problem is quite instructive not just because
it's a useful algorithm, which it is, but because of the method for
deriving it.
Before coding, let us do a bit of mathematics.
You can always "haircut"
code, but remember mathematics?
I used to talk about it before they told me to switch to programming.
Deriving mathematically is a good thing.

We want to rotate a sequence right by one.
If we have an empty sequence. What do we do?
We are done.
If we have a one-element sequence `a_0` and we want to rotate it,
how do we do it?
Done.
That allows us to consider an inductive solution.
Somehow, by hook or by crook we have an algorithm which knows how to shift
`n` things,
such as the range:

    a_{0}, ..., a_{n-1}

Then the question is, how could we get an algorithm for

    a_{0}, ..., a_{n-1}, a_{n}

How do we add one additional element?
After the shift the first `n` elements (leaving `a_{n}` fixed) we have:

    a_{n-1}, a_{0}, ... a_{n-2}, a_{n} 

What do we need to do to solve the problem?
Just swap `a_{n-1}` and `a_{n}`.
In general, swap last and first.

Here is an example with 3 elements:

    1 2 3

We first rotate the one element range `1`. It's all done,

    [1] 2 3

To rotate the two element range,
swap the first element, and the one following our range:

    [2 1] 3
        

Now we have the first two rotated.
To rotate the full sequence
we once again swap the first and last:

    [3 1 2]

Done!
It might not be the fastest, but it is going to be much more elegant.

    template <typename I>
    // I is ForwardIterator
    void rotate_right_by_one(I first, I last, std::forward_iterator_tag) {
        if (first == last) return;
        I current = first;
        while (++current != last) std::swap(first, current);
    }

Let's write a dispatch for both versions,
it will compile to no code[^concepts].

    template <typename I>
    inline
    void rotate_right_by_one(I first, I butlast, I last) {
      rotate_right_by_one(first, butlast, last, typename std::iterator_traits<I>::iterator_category());
    }


[^concepts]: Alex: Someday we will get concepts
    in the C++ standard and not have to write these things.
    But that will be at least 5 years and I won't be programming.
    I'm like an old man planting an apple tree.

### Should we support forward iterator?

Someone brought up that `ForwardIterator` doesn't
really make sense for this algorithm[^stl-forward-iterators],
because if we have something like a linked list, we don't need to rotate
or shift elements around,
we can just insert it where it belongs.
That's a good idea, but maybe some measurements
will show us otherwise.
We already implemented optimal linked list sort.
Later we need to compare whether it's actually
faster to use our list sort, or to use our method
we develop.

Why do you think I say that?
List sort destroys locality.
If at every `cdr` (next), you get to a different cache
line, that's a problem.
In our sort we constantly re-link
next, so eventually you get to a point where everything is scattered
all over memory.

STL used to have a sentence in the container
section which the standard committee threw out.
Use a vector.
This is a true statement.
Unless you are absolutely positive
you need something else, use a vector
or a C array[^sutter-advice].


[^for-loop]: Alex: Could we use a `for` loop instead of a `while`?
    Yes, but I hate `for` loops.
    Why? Because the semantics have changed about 6 times,
    since I started C++, `while` loops never changed.


[^stl-forward-iterators]: Alex:
    I actually drop this requirement in STL and require
    `RandomAccessIterator`, for all the sorts.
    It wasn't the standard committee's fault just me.
    I am not sure if I agree with myself.

    The reasoning went like so.
    Most people of course, don't know anything.
    Therefore if you give them things which sort
    `ForwardIterator`s, they will attempt to use
    them on things like linked lists.
    It's so much slower that they would be better off copying into a vector,
    sort stuff there, and then copy back.

    I was a "nanny".
    I was making decisions by saying,
    "I know how to do it in the more general case.
    But, I will not let programmers do it because
    they are immature."
    This "nanny" control is not necessarily a good thing.
    I am of two minds here.
    I am trying to not be a nanny here.
    I'm trying to show you the spectrum.
    Sometimes you have to sort linked lists because you have no extra memory.
    It doesn't happen often.
    It probably will never happen in your life.
    But, it just might for at least one of you.

[^sutter-advice]: Alex: Because they threw it out, people like
    [Herb Sutter][sutter] used to recommend to the world
    to use [`std::deque`][cpp-deque] (see ["Using Vector and Deque"][vector-and-deque]).
    I'm not making it up.
    He thought, that it's better because it supports more operations.
    He was wrong, I wrote both `std::vector` and `std::deque`.

[vector-and-deque]: http://www.gotw.ca/gotw/054.htm
[cpp-copy-back]: https://en.cppreference.com/w/cpp/algorithm/copy_backward
[cpp-deque]: https://en.cppreference.com/w/cpp/container/deque
[sutter]: https://en.wikipedia.org/wiki/Herb_Sutter


## Code

- [binary_insertion_sort.h](code/binary_insertion_sort.h)
- [test_insertion_sort.cpp](code/test_insertion_sort.cpp)


