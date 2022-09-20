19. Linear insertion sort
==========================

## Thank you, noble art.

Last time we started with ["The Organ Grinder"][organ-grinder] (Der Leiermann)
which explains how it feels to stand outside in the cold with an empty tray
being barked at by stray dogs.
That's how it feels when I start here (joke).

Today, I decided to play another short song which addresses
another aspect of what we do.
This is a song by [Schubert][schubert] again, called ["To Music"][about-music]
(An die Musik).
It's a very short song which gives thanks to this great art which lifts us in
the grayest times of our lives.
That's basically the words.
In this video the words will be first introduced by a great English pianist [Gerald Moore][moore], who accompanied many great singers in Schubert songs.
[Elisabeth Schwarzkopf][elisabeth] was probably the greatest soprano of the last century.
She is also a beautiful woman.
Let us proceed... ([Video here][to-music]).

Schubert was 18 years old when he composed it.
*If I had a choice between founding Facebook or writing this song, as my lifetime
accomplishment, I would not hesitate one second, and it's not going to be the
Facebook*.
What was the point?
Why did I want to play this song?
Because I actually believe, against all odds, and in spite of the many terrible evidence to
the contrary, that the words which I used there for music; the noble art, actually
applies to programming.
In spite of all the terrible things which I
had to witness over my life, I am grateful.
I share the same attitude.
The song concludes with the words, "thank you, noble art."
The only thing I could say is thank you.
It was wonderful that I was able to write code.
I think we need to learn to share this attitude.

[schubert]: https://en.wikipedia.org/wiki/Franz_Schubert
[moore]: https://en.wikipedia.org/wiki/Gerald_Moore
[about-music]: https://en.wikipedia.org/wiki/An_die_Musik
[elisabeth]: https://en.wikipedia.org/wiki/Elisabeth_Schwarzkopf
[to-music]: https://www.youtube.com/watch?v=Bm_AKMV0ME0
[organ-grinder]: https://www.youtube.com/watch?v=sIIS-UgixGE

## Linear insertion sort

It's not clear whether binary insertion sort is the right algorithm to use at all.
If we have `BidirectionalIterator`, we could use regular insertion sort which is a
very good algorithm and we'll need it later in the course.
It's a very important component to be used in multiple places.
For example, when implementing quicksort.
Plus, we will be able to investigate
several interesting techniques and discover one deep problem on this very
simple sorting algorithm.
Binary search is not always a good thing to do.

Remember insertion sort is good when things are almost sorted.
They are pretty close to where they should be.
Binary search is going to poke far away.
It's going to do a little poking, logarithmic poking,
but it's going to poke potentially far away.
Since in many cases we are going to be using
insertion sort in the context where things are pretty close,
doing linear insertion might be a good thing to do.
Linear search is of course best on arrays that are already sorted.
You just look to the next guy, and do nothing.
It's one comparison per element.

### Linear insert

How do you make code beautiful?
We find the main loop invariant.
In order to be able to do the right thing, we need to see what we need to accomplish.

Our goal, like binary insert, is to 
insert an element into the portion
at the front of the range.
We first copy the element `value` out to insert,
and create a hole where it was left.
We then move the hole from the right,
as far left as possible (requiring `BidirectionalIterator`).

    [            | ]
    ^            ^
    first        hole

When are we allowed to move the hole?
What are the conditions?

1. `hole != first`. If this happens, we cannot move any further.
2. `value < prev(hold)`.

If both hold, we continue to move left.
Eventually, one of the conditions will not hold.
We can even prove it:
There are only finitely many things in the range,
so after so many iterations it will be exhausted (these termination proofs are not usually very profound).

In our code we will call `hole`, `current`:

    template <typename I, typename R>
    // I is BidirectionalIterator
    // R is WeakStrictOrdering on the value type of I 
    I linear_insert(I first, I current, R r) {
      // precondition: is_sorted(first, current, r) && current is a valid iterator
      typedef typename std::iterator_traits<I>::value_type T;
      T value = *current;
      while (first != current && r(value, *predecessor(current))) {
        *current = *predecessor(current);
        --current;
      }
      *current = value; 
      return current;
    }

When `first == current` at the start, it will copy `*current` to a temporary
variable and put it right back.
Would a check be better to avoid this?
As we have talked about before, this is a case that seldom happens
whereas adding an explicit check would slow down every other case.

Of course, we need to define predecessor:

    template <typename I>
    // requires I is BidirectionalIterator
    inline
    I predecessor(I x) { return --x; }

### Traditional insertion sort

Now linear insertion sort is about identical to binary insertion sort,
we just use `linear_insert`, instead of `binary_insert`.

    template <typename I, typename N, typename R>
    // I is BidirectionalIterator
    // N is Integral
    // R is WeakStrictOrdering on the value type of I 
    I linear_insertion_sort_n(I first, N n, R r) {
      if (n == N(0)) return first;
      I current = first;
      ++current;
      N i(1);
      while (i < n) {
        // invariant: is_sorted_n(first, i, r) && std::distance(first, current) == i
        linear_insert(first, current++, r);
        ++i;
      }
      return current;
    }

Let's write the version for bounded ranges.
It's very easy.
As a base case in our induction, an empty range, and a one
element range, are both sorted.

    // I is BidirectionalIterator
    // R is WeakStrictOrdering on the value type of I 
    void linear_insertion_sort(I first, I last, R r) {
      if (first == last) return; 
      I current = first;
      ++current;
      while (current != last) {
        linear_insert(first, current, r);     
        ++current;
      }
    }


## Sentinel insertion sort

I think we can optimize it further.
You might argue we don't need to.
But, let me tell you one of the most humiliating times in my life.
I was giving a talk at [Stanford][stanford].
A certain professor walked into the talk
and when I showed my code, roughly like what we just wrote (it was a little different, but same idea), I said, "it's obviously optimal".
This professor interrupted
and said, "no it's not".
The trouble is his name was Donald Knuth.
When somebody like Don Knuth says that your code is not optimal,
you are in a difficult situation.
His argument was that we do this conditional check:

    first != current

`n` times, when we don't need to.
If you're really into performance you have to put a [sentinel][sentinel] in the back.
I was using sentinels before, but from that point on I decided
to make an effort to use them more.

This is a valid argument.
We are not here to impose some theoretical conditions on algorithms.
We are here to take existing efficient algorithms and find how to express them.
We have to write whatever we write to enable this code to work.
What this means is that we sometimes have to reject or ignore
other notions of "good software engineering" in order to get
the efficient algorithms.
This is a very simple loop, so literally every instruction counts.
Could we eliminate that condition?
Can we assure that we don't need to perform it?
We cheat by changing the precondition.
What condition would allow us not to check?

Somehow we assure that the first element is always
smaller than the one we want to insert.
So copy paste our previous `linear_insert`
and let's rewrite the precondition.

      // precondition: is_sorted(first, current, r) &&
      //               current is a valid iterator &&
      //               first != current &&
      //               !r(*current, *first)

Now we can remove the condition:
 
    template <typename I, typename R>
    // I is BidirectionalIterator
    // R is WeakStrictOrdering on the value type of I 
    I linear_insert_with_sentinel(I first, I current, R r) {
      // precondition: is_sorted(first, current, r) &&
      //               current is a valid iterator &&
      //               first != current &&
      //               !r(*current, *first)
      typedef typename std::iterator_traits<I>::value_type T;
      T value = *current;
      
      while (r(value, *predecessor(current))) {
        *current = *predecessor(current);
        --current;
      }
      *current = value; 
      return current;
    }

[stanford]: https://en.wikipedia.org/wiki/Stanford_University

### Application to quicksort

Now we need to write a new insertion sort
which guarantees this condition.
Before we get there, let's see if we can
make 1 or 2 additional components out of it that will help us sort.

Eventually we hope to study a very important algorithm called quicksort.
A long time ago the person who invented it, [Tony Hoare][hoare]
observed that quicksort becomes inefficient towards the end
of recursion, when you start doing partitions of very small things.
He suggested that we run insertion sort down there, at the lower
levels of recursion.
Originally people thought they would go down recursively and call insertion
sort every time we reach a small subrange.
But, Hoare observed you really don't need to call insertion sort many times.
You can just stop the recursion when quicksort sorts things up to a certain size,
and then, at the end, run one pass of insertion sort, over the whole thing.
Because when things are almost where they should be, insertion sort is effective.
Quicksort can guarantee that eventually everyone will be within some threshold of their final destination.

Let's assume we are sorting a million elements.
After we are done with quicksort we have a threshold,
say `k`, and all the elements are partitioned.
The first partition will be somewhere within the first k elements,

    [      |            ...            ]
    ^      ^                        
    first  first + k

What I realized is that the elements in that first sub range `[first, first + k)`
are all natural sentinels for all the others.
What we can do is use non-sentinel insertion sort for the first sub range.
But, then we can take the smallest and use it as a sentinel for all past it.

For example, when we stop quicksort early we might have:

    [ 1, 3, 5 | 2, 11, 17 ... ]

5 is not a sentinel for the right side, because 2 is smaller.
1 is the sentinel.
The line drawn in that range is not necessarily a quicksort partition.
If it were, we couldn't have 2 on the right side.
We have absoutely no idea where the real quicksort partitions are.
But, we know that there is a partition boundary within some threshold
(say left of the line).

Now that is quicksort, but we will design our components in a way to support it.
Let's write a function that assumes we have a prefix that is sorted
and contains a sentinel.

    template <typename I, typename R>
    // I is BidirectionalIterator
    // R is WeakStrictOrdering on the value type of I 
    void insertion_sort_suffix(I first, I last, R r) {
      // precondition: one of -
      // (1) a valid range before first is sorted with respect to r and contains a sentinel
      // (2) *first is a sentinel
      if (first == last) return; 
      I current = first;
      ++current;
      while (current != last) {
        linear_insert_with_sentinel(first, current, r);     
        ++current;
      }
    }

### Optimized insertion sort

So let us copy `linear_insertion_sort` and make our definitive `insertion_sort`.

    template <typename I, typename R>
    // I is BidirectionalIterator
    // R is WeakStrictOrdering on the value type of I 
    void insertion_sort(I first, I last, R r) {
      if (first == last) return; 
      I current = first;
      ++current;
      if (current == last) return;
      // create a sentinel 
      rotate_right_by_one(first, ++std::min_element(first, last, r));
      insertion_sort_suffix(current, last, r);
    }

Copy paste, and make the unstable version
by replacing rotate with swap.

    template <typename I, typename R>
    // I is BidirectionalIterator
    // R is WeakStrictOrdering on the value type of I 
    void insertion_sort_unstable(I first, I last, R r) {
      if (first == last) return; 
      I current = first;
      ++current;
      if (current == last) return;
      // create a sentinel 
      std::swap(*first, *std::min_element(first, last, r));
      insertion_sort_suffix(current, last, r);
    }

[sentinel]: https://en.wikipedia.org/wiki/Sentinel_value
[hoare]: https://en.wikipedia.org/wiki/Tony_Hoare

## Selection sort

This leads us to another classical sort,
it's very slow, but since it's classical and takes only a few lines we will discover it.

What's the idea of selection sort?
You find the min, put him in the first place.
You find the min of the remaining range, put him in the next place, and so on.
Could we write it?

    template <typename I, typename R>
    // I is ForwardIterator
    // R is WeakStrictOrdering on the value type of I 
    void selection_sort(I first, I last, R r) {
      while (first != last) {
        std::swap(*first, *std::min_element(first, last, r));
        ++first;
      }
    }

It's not stable, but it's not hard to fix.
The problem is `std::swap` might skip over lots of equal guys.

    template <typename I, typename R>
    // I is ForwardIterator
    // R is WeakStrictOrdering on the value type of I 
    void stable_selection_sort(I first, I last, R r) {
      while (first != last) {
        rotate_right_by_one(first, ++std::min_element(first, last, r));
        ++first;
      }
    }

Comparison is typically fast, but `swap` we tend to think of as slow.
Imagine the elements are buildings that need to be lifted up and carried to swap places.
The unstable `selection_sort` is actually amazing in that it just does `n - 1` swaps, always.
Merge sort, quicksort, they do like `n log(n)` swaps.
Is it practically important? No.
Not once have I needed selection sort.
So why do I talk about it?
It shows us how to create a sentinel.

## Preconditions are essential

There's this advertising company you might have heard in Mountain view called Google.
They have bright young things, and one of them decided that he wants to randomly shuffle a vector.
It's a reasonable thing.
He decided that the correct way of doing that is the following:
you implement a comparison function which throws a coin.
It randomly generates true or false.
Then take `std::sort` and pass this function to it, 
because it will obviously create a randomly shuffled thing.
Low and behold to everybody's amazement it caused
segmentation fault.
There were messages throughout Google saying STL is totally broken.
Obviously, because it brings Google down.
Let's argue why he shouldn't do what he did. 

1. There is an algorithm in STL called [`std::random_shuffle`][cpp-random-shuffle].
   Why not use that?

2. Somebody more advanced, would say, even if it worked, it wouldn't
   be a uniform random shuffle[^uniform-shuffle].
   It is screwed up, but it requires probability theory or Knuth.
   These people at Google just don't read.
   The brightest people do not need to read (joke).

3. My dear friend Dave Musser who was on sabbatical at Google ventured to post
   that he did not satisfy the preconditions.
   Random true/false is not a weak strict ordering,
   or any ordering whatsoever.
   He tried to explain, but they said no.
   It should work with whatever I pass to it.

As you can imagine, we cannot rely on any properties, like sentinel with this
going on.
For a while there were a bunch of postings on the internet saying,
do not use `std::sort` because it requires `WeakStrictOrdering`.
It's provably the weakest possible requirement.
I thought it was good, but they turned it around and said no.
Use `std::stable_sort`.
I still see this in code, people use it when they don't need
stability because they read these discussions.

Apparently it is an expectation of a modern
programmer that you don't have to satisfy any precondition.
Things should do something and never cause a segmentation fault.
It is a tricky thing.
Nowadays I wonder.
What should we do when we build components?
Should we assume that we build them the fastest way and carefully
specify preconditions?
Or should we build idiot-proof (Google quality) components?
This is a difficult question.
I do not know the answer.

## Final project

Write the fastest version of stable sort that you can, utilizing the ideas in these tools.
We have all the algorithmic ideas we need.
But, I invite you to read books.
I invite you to test and measure.
If you want you can even go read old STL code I wrote.
It's a competition! Consider teaming up and sharing ideas.

## Code

- [insertion_sort.h](code/insertion_sort.h)
- [test_insertion_sort.cpp](code/test_insertion_sort.cpp)


[cpp-random-shuffle]: https://en.cppreference.com/w/cpp/algorithm/random_shuffle

[^uniform-shuffle]: A uniform shuffle of a range of elements `x1 ... xn`
    is an algorithm which produces a random permutation of the elements,
    in a manner such that all possible permutations are equally likely.
    Since there are `n!` permutations, each permutation should occur
    with probablilty `1/n!` (See "The Art of Computer Programming" 3.4.2).

