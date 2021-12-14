13. Searching
===============

## History of binary search

The first time binary search was described as an algorithm,
it was done by a great American computer scientist and engineer [John Mauchly][mauchly].
Of course, you have never heard of him.
He was the guy who invented first general purpose computer, but we
don't remember people like that.
In 1946 he gave a brilliant series of
lectures at the [Moore School][moore-school] at [Pennsylvania University][penn] 
on programming.
For the first time,
he described things like merge, merge sort, and binary search.
This is not a bad thing to be the first person to describe.
He designed [ENIAC][eniac] which should make him very famous.
Indeed, he did some very fundamental work.

Then comes this interesting fact (from "The Art of Computer Programming")
It takes about 15 years for people to come up with binary search
which sort of works for all possible inputs.
Apparently people didn't have trouble coding binary search when the length is
of the form `2^(n-1)`.
Because it's easy, you take the middle element and
then both sides will be of the same form
and you can keep dividing.
Apparently people couldn't do it.
Knuth claims that the first correct implementation was done by 
[D.H. Lehmer][lehmer].
He is someone you should know about
as a very great computer scientist.
He did amazing amount of work on computational number theory,
things like sieves for discovering large primes and many other important things.
Among other things, he published a binary search which at least always terminated.

I actually disagree with Knuth slightly
and claim that the first correct binary search was published roughly at the same time,
but a couple of years after,
by a German computer scientist.
Once again, he is unjustly forgotten.
He does not appear on Wikipedia[^bottenbruch-not-on-wiki].
His name is [Herman Bottenbruch][bottenbruch].
His claim to fame is he was one of the people who invented [Algol 58][algol-58], the predecessor of Algol 60.
He is one of the people who tried unsuccessfully to convince American
delegates to Algol 58 committee that they should introduce block structures.
He was one of the inventors of blocks.
American representatives which included such brilliant people as [John Backus][backus]
and [Alan Perlis][perlis] actually rejected it as too hard to implement.
They didn't know how to do stacks.
But sadly enough he doesn't get much credit, especially credit for correct binary search.
We will be actually studying his version.

[^bottenbruch-not-on-wiki]: He does [now][bottenbruch]!

[mauchly]: https://en.wikipedia.org/wiki/John_Mauchly
[moore-school]: https://en.wikipedia.org/wiki/Moore_School_of_Electrical_Engineering
[penn]: https://en.wikipedia.org/wiki/University_of_Pennsylvania
[eniac]: https://en.wikipedia.org/wiki/ENIAC
[lehmer]: https://en.wikipedia.org/wiki/D._H._Lehmer
[bottenbruch]: https://en.wikipedia.org/wiki/Hermann_Bottenbruch
[algol-58]: https://en.wikipedia.org/wiki/ALGOL_58
[backus]: https://en.wikipedia.org/wiki/John_Backus
[perlis]: https://en.wikipedia.org/wiki/Alan_Perlis

## bsearch is wrong

If we think about merging two sequences of roughly the same length,
or rather exactly the same length `n`, the expected number of comparisons
is going to be `2n - 1`.
From which follows a conjecture.
If we have sequences
of size `n` and size `m` the number of comparisons should be `n + m - 1`.
Not every conjecture is true however, this one is definitely false.
Here is a simple counter example.
Take a sequence of length 1000
and a sequence of length 1.
We only need `log(1000)` because
we can binary search for its index.

So there is a fundamental possibility
for using binary search for merging, dramatically reducing the number of comparisons.
`log(n)` is much smaller than `n`.

You might think we can just use binary search from
a standard library, such as C [`bsearch(3)`][bsearch].
Sounds like a plausible idea.
It was written by great UNIX guys.
They know something about programming, so let us see what they provide us with,
by quoting the man page:

    void* bsearch(
        const void* key,
        const void* base,
        size_t nmemb,
        size_t size,
        int (*compare)(const void*, const void*)
    );

Notice it takes all these parameters,
and it's a little messy because it's C.
Components are hard for them[^bsearch-generics-hard].
Nevermind what it takes.
What's interesting is what it returns.

> returns a pointer to a matching member of the array, or NULL if no match is found.

So for our merge, it will most often return `NULL`.
At which point, you will have to do linear search.
So observe, ancient interface,
done by brilliant people,
in the standard library and it's utterly useless.

Even if, we are so fortunte as to get a pointer to an element back.
Does it help with merge? Especially if we want to make it stable?
No.

> If there are multiple elements that match the key, the element returned is unspecified.

You will read similar things in many algorithms books.
It's a typical story for binary search.
Even when the book is written by famous people.
I'll show you how to write it.

[^bsearch-generics-hard]: C does not have `template` or any other type safe form of generics.
  This makes it difficult to write reusable components in the way Alex teaches.
  As a workaround, one can use `void*` as a pointer to any type.
  This is the method used by `bsearch` and `qsort`.

### What is correct code?

Here comes another philosophical point.
*What does wrong mean?*
*What does incorrect mean?*
At school they told you that the program is incorrect when *it doesn't satisfy its specifications*.
Well, then `bsearch` is a correct program.
I looked at the source, it does do what it promises to do.
It will return `NULL`.
I wish it were not correct.
I wished it returned something useful.

Correctness is a deeper concept than
just satisfying specification.
Well in reality, as you guys know,
it must be deeper 
because you haven't got any specifications.
When you write code, it's not that you are given specifications and need to encode them.
I suspect that has never happened in your life, nor will it happen in any foreseeable future.
But you still have to attempt to do something which is correct.
Of course the people who advocate writing specifications will say yes, first 
you will write specification, and then implement specification.
But, it's not going to help.
Because, if you write wrong specification, you are the same guy who is going to write the implementation.
Most likely it will not make it correct.

So it's a deeper thing.
You have to establish correctness from more fundamental principles.
*The program is correct if it returns desirable information*,
if it does what it's supposed to do in some absolute sense.
It's very hard to prove it.

I think one of the lessons of this particular lecture is
how hard simple things are.
lots of very bright people cannot give it a correct interface.
same with `bsearch`.

You might say, "Alex just talks about his beef with the standard committee."
No.
What I'm trying to tell you is that
when you write things like that in your code,
There will be some other guy using your code.
Always think about that other guy.
The great flaw in most code I see is there is no consideration
for the other guy.
People think, "oh it works, so it's done."
My dream is that we all write code thinking about other people.
Then you say, "well, then I have to do more work."
This is the beauty of sharing.
You might have attended kindergarten
and had a teacher that taught you it's good to share toys.
She was right.

[bsearch]: https://man7.org/linux/man-pages/man3/bsearch.3.html

## Linear search

It would be very contrary to the way I do things to
start with binary search.
How could we do binary search if we cannot do linear search?
In STL it is called [`std::find`][cpp-find] or `std::find_if`[^name].
Let's see how to write it.
We can assume we know how to do it,
and start from the top,
or we could assume we don't know what we are doing,
which is usually the case when starting new things.
I seldom start writing code from the signature.
I don't know what the signature is.
I typically have some algorithmic idea,
so I start with that,
often an inner loop.
Then write code inside out.

    while (first != last && ... find the element...) ++first;

Now write the signature:

    template <typename I, typename P>
    // I is an InputIterator
    // P is a unary predicate
    I find_if(I first, I last, P pred) {
      // [first, last) is a valid range.
      while (first != last && !pred(first)) ++first;
      return first;
    }

`I` is an `InputIterator` instead of a `ForwardIterator`
because it is single pass.

### Trimming the standard

One of the mistakes which frequently happens is people
use the principle of [Occam's Razor][razor]
and say, "we need to only have one `find_if`".
That's what happened.
After I submitted STL it had many fine functions,
but Bjarne was very afraid that STL was too large and would not be accepted, as is.
(It wasn't that enormous at that point.)
He said, "why don't I come to Palo Alto (I was at HP Labs)
and bring along bunch of other standard committee people and we
will trim it". 
Trimming was a sad thing.
Imagine somebody coming with a knife and cutting pieces of your flesh.
One of the things he said was there should be only one `find_if`.

[razor]: https://en.wikipedia.org/wiki/Occam%27s_razor

### Helper functions

So what else used to exist?
Everytime I program I put this function back.
It takes about 30 seconds,
it's called `find_if_not`.
Sometimes you want guys who are good,
and sometimes bad.
Couldn't you negate a predicate and use the same function?
Well, I did, I added [`std::not1`][cpp-not1]
and [`std::not2`][cpp-not2].
But, it's annoying.
Fortunately, I think the
standard committee in C++11 put it back in the standard.
But they didn't do all of them,
and some of them are not correct.     

    template <typename I, typename P>
    // I is an InputIterator
    // P is a unary predicate
    I find_if_not(I first, I last, P pred) {
      // [first, last) is a valid range.
      while (first != last && pred(first)) ++first;
      return first;
    }

Somebody asked me in 1994
why I didn't write `std::all_of`, `std::none_of`, `std::any_of`
which are now in C++11.
My attitude was I write algorithms,
not wrappers.
They are just wrappers[^wrapper]. 

    template <typename I, typename P>
    // I is an InputIterator
    // P is a unary predicate
    inline
    bool all_of(I first, I last, P pred) {
      return find_if_not(first, last, pred) == last;
    }

    template <typename I, typename P>
    // I is an InputIterator
    // P is a unary predicate
    inline
    bool none_of(I first, I last, P pred) {
      return find_if(first, last, pred) == last;
    }

    template <typename I, typename P>
    // I is an InputIterator
    // P is a unary predicate
    inline
    bool any_of(I first, I last, P pred) {
      return find_if(first, last, pred) != last;
    }

[cpp-not1]: https://en.cppreference.com/w/cpp/utility/functional/not1
[cpp-not2]: https://en.cppreference.com/w/cpp/utility/functional/not2

## Bounded and counted ranges

Once upon a time I believed ranges come in two kinds[^eop-range-kinds].

1. **Bounded ranges**: Ranges bounded by a first and last iterator/pointer.
2. **Counted ranges**: Ranges constructed from a first pointer/iterator and an 
    interger `N`.

Which one is better?
Both are good, and both are different.
You cannot say one is better than the other.
It depends on the algorithm.
There used to be more bounded range algorithms in the STL, but they were taken out.
For example we have [`std::copy`][cpp-copy] and
[`std::copy_n`][cpp-copy-n]
both are really convenient.

Let's look at the interface for `std::copy_n`.

    template<typename InputIt, typename Size, typename OutputIt>
    OutputIt copy_n(InputIt first, Size count, OutputIt result);

This is actually wrong.
You have to return where the output iterator advances to,
because it might be gone after the function operates
internally, and there is no way to get it back.
But, it's not enough.
It needs to also return where the first ends.
There is a general principle, you should be able to restart the algorithm
where you left off.
You copy something, then you want to copy more.
So we need to think carefully about what you return.
You might think everyone gets it, but
that code was reviewed by hundreds of people,
and not noticed.

So let's try writing `find_if_n`

    template <typename I, typename N, typename P>
    // I is an InputIterator
    // N is an integral type
    // P is a unary predicate
    std::pair<I, N> find_if_n(I first, N n, P pred) {
      while (n != N(0) && !pred(first)) {
        --n;
        ++first;
      }
      return std::make_pair(first, n);
    }

[^name]: Alex: "The name is stolen from Common Lisp [`find`][clhs-find].
    Always try to borrow from some place.
    Originality is frowned upon,
    especially for naming.
    Everyone loves to make non-standard names."

[^wrapper]: This is an amusing comment because
    Alex just got done talking about why `find_if_not`
    was such a helpful contribution
    and how we should consider the needs of the user
    and give them various convenience interfaces for algorithms.

[^eop-range-kinds]: These two kinds of ranges are discussed in depth in "Elements of Programming" chapter 6.

[cpp-find]: https://en.cppreference.com/w/cpp/algorithm/find
[clhs-find]: http://clhs.lisp.se/Body/f_find_.htm
[all-of]: https://en.cppreference.com/w/cpp/algorithm/all_any_none_of
[cpp-copy]: https://en.cppreference.com/w/cpp/algorithm/copy
[cpp-copy-n]: https://en.cppreference.com/w/cpp/algorithm/copy_n

## Advance and distance functions

How does [`std::advance`][cpp-advance] work?
It was introduced by me to allow
us to do long or fast thing depending on iterator type.
For a pointer it will translate to one instruction.
It's going to be fast.
In the case of a linked list, it's going to be linear time.
Here is how it works:

    template<typename I, typename N>
    inline
    void advance{I& first, N n) {
      advance(first, n, std::iterator_traits<I>::iterator_category);
    }

    template<typename I, typename N>
    inline
    void advance{I& first, N n, std::input_iterator_tag) {
      while (n > 0) {
        ++first;
        --n;
      }
    }

    template<typename I, typename N>
    inline
    void advance{I& first, N n, std::random_access_iterator_tag) {
      first += n;
    }

The dispatch between these is clearly done at compile time.
I could have had every iterator have `+=` and `-` operators.
My thinking was that people have an expectation that those symbols are fast.
Making it linear time will confuse many people.[^eop-advance-distance]

Now let's implement [`std::distance`][cpp-distance]:

    template<typename I>
    inline
    typename std::iterator_traits<I>::difference_type distance{I& first, I& last) {
      return distance(first, last, std::iterator_traits<I>::iterator_category);
    }

    template<typename I>
    inline
    typename std::iterator_traits<I>::difference_type distance{I& first, I& last, std::input_iterator_tag) {
      typename std::iterator_traits<I>::difference_type n;
      while (first != last) {
        ++first;
        ++n;
      }
      return n;
    }

    template<typename I>
    inline
    typename std::iterator_traits<I>::difference_type distance{I& first, I& last, std::random_access_iterator_tag) {
      return last - first;
    }

[^eop-advance-distance]: Alex: I am not saying I was right, because when we were writing
    "Elements of Programming", Paul and I, we decided to abandon advance and distance,
    and just say that depending on iterator category,
    the complexity of these operators change.

[cpp-advance]: https://en.cppreference.com/w/cpp/iterator/advance
[cpp-distance]: https://en.cppreference.com/w/cpp/iterator/distance

## Code

- [algorithm.h](code/algorithm.h)
- [search.h](code/search.h)



