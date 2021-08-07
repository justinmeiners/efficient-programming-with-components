7. Minimum selection on ranges
====================================

## The standard is not a limitation on ideas 

When I joined A9 I wrote a little generic function and it was not using STL iterators.
It used some other iterator like thing, but, totally different.
We wrote it, tested it, sent it in and one of
the wise people who works for us looked at it and sent back
the reply, "you guys have to learn about STL iterators",
which I really appreciate.
In Silicon Valley you have to be careful about saying you're
an expert. You might be talking to the guy who invented it.

That guy's attitude is that if
there is something in the standard, that's the only thing you should use.
This is false.
Use STL only when it fits. 
If you come up with some other idea (many ideas in the world)
try it.


## Minimum element in a range

Now we want to apply `min` to a range.
Suppose we want to find an item with smallest price.
We need an interface which allows us to take as many items as you like and find the minimum.
In STL it's called [`std::min_element`][minelement]

The algorithm is very simple, but there are details we need
to learn about ranges.
We will show an implementation and then talk about it:

    template <typename I, typename Compare>
    // I is a ForwardIterator
    // Compare is StrictWeakOrdering on ValueType(I)
    I min_element(I first, I last, Compare cmp) {
      if (first == last) {
        return last;
      }

      I min_el = first;
      ++first;

      while (first != last) {
        if (cmp(*first, *min_el)) {
          min_el = first;
        }
        ++first;
      }

      return min_el;
    }


[minelement]: https://en.cppreference.com/w/cpp/algorithm/min_element

### Iterator conventions

We need to talk about iterator conventions.
Notice that `min_element` doesn't return the value itself,
but the iterator pointing to the minimum element.
Why?
We probably want to update the value.

Suppose, I'm a manager I want to look for the
worst-performing guy and then fire him (joke).
I don't want his value, I want a handle on him.
I want an iterator.
Do you see my point?
You very often want to do
things with what you find.

There is another reason.
The range might be empty[^emptyrange].
In which case we return last iterator.

`first` and `last` are maybe bad names, but that's what they are.
They are hard to change, because I called them that everywhere.
`last` actual doesn't mean last.
`last` means one after the last element.
In order to define a sequence you need to point  past the last.
Because you want to able to work with empty ranges.


            a b c d
    first   ^
    last    ^

Suppose `last` is actually last and you point to the same place.
That indicates a range of one element.
There is no way to do zero.



Later, we will look at algorithms for partitioning.
We want to partition good people from bad people.
After the partition we will return a pointer which separates good from bad.
There will be first, last, and middle.
The middle partitions between
the first bad and first good. 
But there may be no good, or no bad elements.
We need to able to return an empty range.

In C++ this is a standard convention some
people in the world of Java and Python are slowly realizing that maybe it
has something to do with mathematics not with C++.
But, it will take decades before people fully realize that you have to always go
passed the end.
Mathematically you need [semi-open intervals][half-open].
They are denoted like so:

    [i, j)

Or in our terms:

    [first, last)

[^emptyrange]: Many languages struggle with empty ranges because they prefer their algorithms
    to work with values instead of iterators.
    For example, they might return `nil` or a boolean which indicates
    whether the value was found or not.

    Of course, unless they are dealing with a nice language, this adds a few lines of code,
    or the possiblity that you forget to check for the nil case, and perhaps
    restricts the algorithm to reference types only.

[half-open]: https://mathworld.wolfram.com/Half-ClosedInterval.html 


### Forward iterator

What kind of thing is `I`.
We indicated it was a `ForwardIterator`.
There is also `InputIterator`.
Both are for iterating forward,
as opposed to random jumps.
What is an `InputIterator`?
Input iterators describe algorithms
which go through the river once[^heraclitus].
It's like they're reading stuff from the wire[^wire].
Imagine the various kinds of streams[^stdin].

But, in our algorithm we store a previous position in the variable `min_el`.
Therefore the things which go through the wire will not work.
So it must be a `ForwardIterator`.

[^heraclitus]: "No man ever steps in the same river twice." - Heraclitus

[^wire]: "Through the wire" refers to receiving data from a communication
    device, such as the internet.
    You read packets as they arrive, and they aren't stored anywhere,
    so once you read them, they are gone.

[^stdin]:
    Another example of the `InputIterator` concept is UNIX pipes.
    Consider the command:

        head -c 500 /dev/urandom | gzip

    Head will read 500 random characters, but that data isn't stored anywhere,
    it's immediately passed to `stdout`, 
    which is `stdin` for [gzip][gzip].
    gzip reads data from `stdin`.
    It can only read data as it comes in.
    It can't seek back earlier in the input.
    Once it is read, it is gone.

    This is one of the reasons why gzip is so useful.
    It compresses data on the fly, without being able to see the incoming data,
    or re-analyzing what came before.

[gzip]: https://linux.die.net/man/1/gzip

## Finding min and max together

How many comparisons do we need to to find the min of five elements?
Four.
In general why do we need `n - 1` comparisons.
Why no more? We don't need to compare an element with itself.
Why no fewer?
Maybe we could do it
in `n - 2` with a clever algorithm.
The simple argument to
remember is that `n - 1` guys have to lose.
We're finding a winner in our competition.
If person didn't play in a competition, if he didn't lose,
we cannot eliminate him.
We need to eliminate all but one.

How many comparisons if we need to find minimum and maximum together?
Obviously we could do `2n - 2`,
what about fewer[^minmax]?
The idea is very simple.
Assume that  we worked up to the middle of 
of our range and we have a running min and a running max.
The temptation is take the next element compare him with min and with max.
That's very sad because very often we will do two comparisons and then discount the other.
So for one element we will be doing two comparisons.
The trick is to pick two new elements,
the next element, and the one after next.
We know nothing about them.
We compare them with one comparison.
Then we know that one of them is a potential min
and the other is a potential max.
Then we compare potential min with the current min
and potential max with current max.
So we need to do three comparisons for every pair, so roughly
speaking we need `3n/2`.

Let's do it.
This can be a little bit tricky.
Do not start writing things from the beginning because
you don't know what goes in the beginning.
Try to write code from the middle.
Try to write the body of the loop.
That's where I described the algorithm
to start with.
Then go back and make the loop invariants on which we depend true.

    template <typename I, typename Compare>
    // I is a ForwardIterator
    // Compare is StrictWeakOrdering on ValueType(I)
    std::pair<I, I> minmax_element(I first, I last, Compare cmp) {
      if (first == last) {
        return std::make_pair(last, last);
      }

      I min_el = first;
      ++first;
      if (first == last) {
        return std::make_pair(min_el, min_el);
      }

      I max_el = first;
      I next = first;
      ++next;

      if (cmp(*max_el, *min_el)) {
        std::swap(min_el, max_el);
      }

      while (first != last && next != last) {
        // min_el contains the current min
        // max_el contains the current max
        // next and first and the next pair of elements to examine
        I potential_min = first;
        I potential_max = next;

        if (cmp(*potential_max, *potential_min)) {
          std::swap(potential_max, potential_min);
        }

        if (cmp(*potential_min, *min_el)) {
          min_el = potential_min;
        }

        if (!cmp(*potential_max, *max_el)) {
          max_el = potential_max;
        }

        ++next;
        first = next;
        ++next;
      }

      if (first != last) {
        // odd elements, one left over

        if (cmp(*first, *min_el)) {
          min_el = first;
          // first < min_el <= max_el, so we don't need to check the next case
        } else if (!cmp(*first, *max_el)) {
          max_el = first;
        }
      }

      return std::make_pair(min_el, max_el);
    }

Example:

    int a[] = { 1, 2, 3, 4, 5, 4, 3, 2, 1, 8, 7, 6, 2 };
    size_t n = sizeof(a) / sizeof(int);
    auto pair = minmax_element(a, a + n, std::less<int>());

This algorithm was invented by [Ira Pohl][pohl][^pohl] of UC Santa Cruz, in the mid-seventies.
He also proved it was optimal.
It's also practically good.
It was [added to][cpp-minmax] the standard in C++11.

## Code

- [minmax_range.h](code/minmax_range.h)
- [test_minmax.cpp](code/test_minmax.cpp)


[pohl]: https://users.soe.ucsc.edu/~pohl/bio.htm
[cpp-minmax]: https://en.cppreference.com/w/cpp/algorithm/minmax_element

[^minmax]: First time find the min (`n - 1` comparisons). Then find the max (`n - 1` comparisons).
           `(n - 1) + (n - 1) = 2n - 2`.

[^pohl]: Alex has written more about Ira and this algorithm [here](http://stepanovpapers.com/IraPohlFest.pdf)
            
