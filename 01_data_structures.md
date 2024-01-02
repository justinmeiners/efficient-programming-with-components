1. Choosing data structures and algorithms
==================================================

## Reflections on Trusting Trust

[Ken Thompson][ken] did many wonderful things. Probably more
than any programmer, he influenced the programming style which we have right
now. While he did not invent C, that was done by his friend [Dennis Ritchie][dennis],
he invented the programming style which underlies C.
Dealing with pointers,
knowing how pointers are subtracted, and stuff like that, all comes from Ken Thompson.

Believe it or not, the best and brightest at that time were heavily on the
march to get rid of pointers. Absolutely brilliant people who would give
[Turing award][turing-award] speeches.
[Tony Hoare][hoare] is the case in point saying that pointers
have to be abolished[^hoare-speech].
The idea was that pointer is illegitimate thing. 
Language designers were fully convinced that
even if you provide pointers nobody should call them pointers.
They would call them something like [access types][access-types].
They have to prohibit iterations, like you could never subtract pointers.
[Niklaus Wirth][wirth], the designer of Pascal, was very certain that you should never allow
subtraction between pointers.
We still have it at least in some languages.
I know that our Java friends do not believe
in subtracting pointers and subtraction of pointers.
But, pointers are still at least partially with us.

So Ken is an absolutely great man in many respects.
His career started, not with UNIX, but when he was freshly out of
school coming up with the brilliant practical algorithm for matching regular expressions[^regex-algorithm].
Every time you write [grep][grep] or something like that you're most likely
exercising code written by Ken in the late 60s.

There is so much practical reliance on regular expressions.
But they were invented
believe it or not by theoreticians, specifically [Stephen Kleene II][kleene] who was a logician.
So Ken made them practical[^minsky].

Then he did UNIX, this totally brilliant operating system on which we all rely.
All of our livelihoods come from Ken, in one shape or form.
Do you remember the [fortune cookie program][fortune]?
One of the fortune cookies was: "it is all Ken's fault"[^fortune-quote].
We have to remember that.

After he does that, just to demonstrate that he can do anything, he decides to show
people how to play [computer chess][ken-chess].
Mind it, at that point as far as I know he
barely knew the moves.
There was total dominance by Russian chess playing
program [Kaissa][russia-chess] and two or three years he builds this specialized hardware,
totally revolutionizes the approach to just playing.
Of course nobody remembers
what Kaissa was.
He is a guy who could enter in a
totally new field and do something beyond, not just world class, but beyond
world class in a short period of time.

In 1984 he was given the Turing award.
It was shared with Dennis and he gave a beautiful speech which I highly
recommend that you read called ["Reflections on Trusting Trust"][reflections] which talks
about many things. But, I'll use just one little episode in the beginning which is
very important from my point of view.
He says that he was very blessed with collaborators specifically with Dennis[^dennis].
Then he continues that during many
years of their collaboration not once they would attempt to do the same thing.
They had this good sense to work on complementary things.
Except once.
Both of them needed to write an assembly program with about
20 lines of code and continues Ken, "I checked, it was character by character
identical".
It sounds like mystical thing right, two people coming together,
this is a Vulcan mind-meld (joke).

But, there is something profound there.
I actually think that it's less amazing than Ken makes it be.
That is a central point of what I am trying to teach here.
I actually had such mind-meld with several of my colleagues after we worked together for a while.
For example, it's literally true that when [Dave Musser][musser] and I were working together long-distance,
we would write the same thing.
It was invariably, character by character, identical (even though we were not as good as Ken and Dennis).

I claim that if we are really turning
programming into professional discipline then the story of Ken and Dennis will
not be a miracle.
We should be writing basic code, character by character, identical.
Imagine how wonderful it would be if you could understand what someone else wrote.

[^minsky]: Alex recommended to me Marvin Minsky's ["Computation: Finite and Infinite Machines"][minsky-computation] 
    to learn more about these topics. 
    It is a fantastic book which explores theory of computation, including finite state machines, neural networks,
    and Turing machines from a philosophical and mathematical perspective.

[^fortune-quote]: I have been unable to find any reference to this quote.
    Some older fortune files
    such as [this file][plan-9-fortune] from plan-9 contain similar quotes,
    such as "Maybe I should have screwed up"

[^dennis]: Alex: In my opinion Dennis wasn't a genius like Ken,
     but obviously first class.

[^teaching-programming]: Alex: At Stanford there's one guy who knows, but he's an emeritus (Donald Knuth).

[^hoare-speech]: From the 1980 Turing award lecture ["The Emperor's New Clothes"][hoare-speech].

[^regex-algorithm]: Ken's algorithm generates a finite state machine 
   which can recognize a given regular expression.
   See ["Regular Expression Matching Can Be Simple And Fast"][regex-fast].

[fortune]: https://en.wikipedia.org/wiki/Fortune_(Unix)
[dennis]: https://en.wikipedia.org/wiki/Dennis_Ritchie
[kleene]: https://en.wikipedia.org/wiki/Stephen_Cole_Kleene
[russia-chess]: https://en.wikipedia.org/wiki/Kaissa
[musser]: https://en.wikipedia.org/wiki/David_Musser
[ken-chess]: https://www.bell-labs.com/usr/dmr/www/ken-games.html
[reflections]: papers/trusting-trust-thompson.pdf
[ken]: https://en.wikipedia.org/wiki/Ken_Thompson
[minsky-computation]: https://dl.acm.org/doi/book/10.5555/1095587 

[grep]: https://linux.die.net/man/1/grep
[plan-9-fortune]: http://fortunes.cat-v.org/plan_9/
[regex-fast]: https://swtch.com/~rsc/regexp/regexp1.html

[turing-award]: https://en.wikipedia.org/wiki/Turing_Award
[hoare]: https://en.wikipedia.org/wiki/Tony_Hoare
[hoare-speech]: https://www.cs.fsu.edu/~engelen/courses/COP4610/hoare.pdf
[wirth]: https://en.wikipedia.org/wiki/Niklaus_Wirth

[access-types]: http://goanna.cs.rmit.edu.au/dale/ada/aln/13_access_types.html

## Hello, World

Whomever really wants to learn, will learn, and that is a challenge because it is my experience that if we take the intersection of what
a room of programmers know it's going to be an empty set. 
That doesn't mean that you as an individual person doesn't know things, but intersection is going to be relatively small.

Unfortunately we've got to a point where nobody teaches programming.
Because there's no professor of computer science who has any idea how to program[^teaching-programming].
Part of what I am trying to do here is to start discussing
little things like, how do we name variables?
Why is it important? We want to get to the point where
everybody writes a consistent code, recognizable code.
This is why I want to go slow and develop so that we
all agree[^conventions].

We have to start with a very very simple program. Most of you recognize it, right? [Hello World][hello-world].

    #include <iostream>

    int main() {
      std::cout << "Hello, world!" << std::endl;
    }

There is no return value.
Do you think it will compile? 
It does.
C++ treats `main` as a special function; meaning when the control goes
through the last brace, zero is returned.
Because the standard UNIX convention, which became Universal
convention, that on success you return zero. 
The language actually allows you to do things like that.

One warning, [`cout`][cout] stuff works pretty much like how you think it works.
However a friend of mine wrote something like:

    std::cout << f() << g();

Lo and behold depending on the compiler, different results were printed.
The order of evaluation of arguments to a function [is not defined][cpp-eval-order]
and the operator `<<` is just a function call[^special-form].
Never depend on the arguments to a function
being evaluated in a given order. 

The person in question actually immediately declared that he found the compiler bug.
Compilers do have bugs but they are not as common as people believe.

[^conventions]: Alex clarifies in later lectures that he is somewhat convention neutral.
    Whenever he works with a new group, he wants to reach consensus about style
    as quickly as possible.
    The organization and team should follow the same conventions.
    Clearly some conventions he has an argument for (when to use classes vs structs)
    but others he say doesn't matter at all, as long as it's consistent
    (which line to put the brace on).

[^about-stl]: [STL][stl] (standard template library) was of course originally written by the teacher Alex Stepanov.
    The original library was written for SGI, but then later integrated into the standard library.
    However, the version available in most implementations is much more complex and less consistent than Alex's original work.
    I have archived the [SGI STL code][stl-code] which probably contains more of Alex' original code than other that is available.

    The [SGI STL documentation][stl-docs] has also been archived and is a very useful reference alongside modern references,
    especially for understanding concepts.

[^special-form]: Alex: `<<` is not some [special form][lisp-special-form] (like the notion in Lisp), it is just a function call.

[stl]: https://en.wikipedia.org/wiki/Standard_Template_Library
[hello-world]: https://en.wikipedia.org/wiki/%22Hello,_World!%22_program
[cout]: https://en.cppreference.com/w/cpp/io/cout
[cpp-eval-order]: https://en.cppreference.com/w/cpp/language/eval_order
[lisp-special-form]: https://mitpress.mit.edu/sites/default/files/sicp/full-text/book/book-Z-H-10.html#%_sec_1.1.6
[stl-docs]: https://www.boost.org/sgi/stl/
[stl-code]: https://github.com/justinmeiners/sgi-stl

## Number of unique elements

This whole talk about performance started me and some coworkers
investigating problems and involved very long queries.
When we narrowed things down we always found something
pertaining to incorrect use of STL[^about-stl].
It's usually a one liner and the most egregious.

The most amazing thing is the following one liner which I will start in the
beginning of the course, because it was just so important.
We could weave the whole fabric of the course around this one little example.
There was a range of integers. For example:

    int a[] = { 1, 3, 1, 4, 1, 5 };

The person wanted to find out how many of these integers are unique.
The person decided to write the following code[^headers]: 

    #include <iostream>
    #include <set>

    int main() {
      std::set<int> set_of_ints(a, a + 6);
      std::cout << set_of_ints.size() << std::endl;
    }

This is perfectly good. It computed the thing.
But it is actually very slow.

### Equality vs ordering

The algorithm for [`set`][cpp-set] [^about-set] is [red black tree][red-black] which is
universally stated in every book to be one of the best implementation of sets.
Do we have to believe textbooks? We will see.
In any case, we'll learn to count number of operations.
We will observe that indeed it is going to do `O(n log(n))` number of comparisons.
But, it is awfully slow.
Why?
Well, it has to do sort, but without actually sorting.
Otherwise, you really cannot find out how many unique elements are in a sequence of integers.
This is a paradoxical thing since finding unique elements is much
easier operation[^table-algo].

It seems that finding unique elements does
not require ordering, it just requires equality.
But, what we are actually doing is optimizing a search or find.
Equality gives us linear search while sorting gives us binary search so we can find much much faster.
One of the amazing things which we will discover is that ordering is very important.
Things which we could do with ordering cannot be effectively done just with equality[^cl-mistake].

If we are going by the book we will say sorting is good as long it does approximately `O(n log(n))` operations.
This is good for the books.
It's actually not good for our code, because Big O
could have an arbitrary coefficient.
It could be `50(n log(n))`[^big-oh],
The effective coefficient, not just in terms of number of operations,
but in terms of time for this is very very
large. How large? We will be doing measurements together of this very problem.
But, let me tell you, it slows down between factor of 22 and a factor of 50 on large data sets.
That is a lot.

[^about-set]: The set data structure is inspired by [mathematical sets][set-math], which contain elements (all of which are distinct) in no-particular order.

[^table-algo]: Alex alludes to there being a linear time solution if we have extra memory available.
    One possible algorithm is using a lookup table.
    For example, one could allocate an array with `2^32` entries and count how many times integers are found only once.

[^big-oh]: To say a function `f` is `O(g)` it's only required that `f` be bounded
    by a constant multiple for all inputs beyond some point.
    Formally, if `f(x) <= Ag(x)` for all `x > M`, where `A` and `M` are constant real numbers
    (see "The Art of Computer Programming" 1.2.11).

    Alex will tend to say "order of" to distinguish `O(f)` from exactly `f`.
    In this section, Alex is observing that there is no requirement for `A` to be small.

[^headers]: Alex: You didn't use to need `#include <set>` when I first did STL.
    I had one gigantic file called `stl.h`
    At that time people said it's utterly unacceptable, you have twenty thousand lines of code.
    If you think about it nowadays it appears to be very funny. Twenty thousand is nothing.
    That's why `stl.h` was replaced with a gazillion little headers.
    Sometimes I have no idea what they are.

[^cl-mistake]: Alex: [Common Lisp][cl] 
    made a similar mistake.
    Some of you think it was a great language.
    They carefully designed a bunch of algorithms which work on arbitrary sequences
    and one of the algorithms was called [`remove-duplicates`][clhs-duplicates] and it relied on equality.
    It actually would go through `N` times removing things.
    They did not sort. 
    They use equality.
    It did work and it worked exceptionally well for most of the applications.
    In Lisp you usually have a list with 5 elements so it works just fine. 
    The moment you go to larger lists things stop working.
    Quadratic algorithms are slow.

[clhs-duplicates]: http://clhs.lisp.se/Body/f_rm_dup.htm
[cl]: https://en.wikipedia.org/wiki/Common_Lisp


### Correct solution

How should we solve the problem? Could we replace it with a one-liner?
There is an STL function just for this.
It is called [`std::unique`][cpp-unique][^spelling].
What does it do?
It goes through a range and removes all non-unique elements,
giving back a range of unique elements.
Of course the presupposition is that the input range is sorted.
It will work if the range is not sorted either.
It will just not eliminate all equal elements.
It returns a pointer to the element past the last element of the range.
This is a standard STL convention, that we will study.

For example given:

    1 2 2 2
        ^
The returned pointer is indicated.
So let's use it

    std::sort(a, a + 6);
    std::cout << std::unique(a, a + 6) - a << std::endl;


Why am I not going to use [`std::distance`][cpp-distance] here (instead
of subtracting the pointer `a` from the result of `std::unique`)?

    std::distance(a, std::unique(a, a + 6));

There is no need.
Never use a function which is intended to be used in a
general case, when you sit deeper inside some specific case. 


[^spelling]: Alex: Ken Thompson changed the spelling of unique ([uniq(1)](https://man7.org/linux/man-pages/man1/uniq.1.html)).
    My great contribution to computer science was to restore it (joke).

### Use the correct STL data structures 

Whenever people have problems with STL, they ask me questions.
They show me their examples of using STL.
The most common thing is something like map of maps:

    std::map<..., std::map<...> >

You sit there and you wonder. Before STL, nobody in his right mind
would dare to say "I'm going to have a self-balancing tree which at every node
has a self-balancing tree", because it will take them five
years to implement it.

It's maybe one of STL's strong points, that such a construction is so trivial.
You could not just have map of maps,
you could have maps of vectors of list of maps of sets.
And in one line.
Will it work? Yes, for some definition of work.
Will it be fast? No.
It cannot be fast because you have this enormous complex stuff.
Its complexity is hidden but it's still there.

Miracles are not possible.
If you ask somebody to create a very
complex data structure that's what you're going to get.
You're going to get problems with node allocation.
You are going to get problems with rebalancing.
You're going to get problem with whatever these advanced data
structures do.
These problems are going to get worse and worse.
You're going to get a [cache miss][cache] on every step through the set.

As our computers become faster and faster and faster they're getting slower and
slower and slower[^slower].
Meaning that going to the main memory is very slow.
You want to have locality of reference. You want all your
data that you are working on in the local cache. If you have a huge set or map it's not
going to be like that. 


Let me tell you a very simple piece of advice.
If you remember just one thing out of this course it should be this:

1. Whenever you can, use [`std::vector`][cpp-vector].
2. If you cannot, find a way so you can.

Avoid any data structures except arrays. "Well aren't there exceptions?"
No, not for you.
Because typically advanced data structures are slower than simple data
structures.
Data structures which appear to be alright when textbook writers
wrote their books, are no longer all right now.
Computers change so much.
Things no longer work.

If `set` is bad? Why is it in the standard?
Think about it.
If you do all of your insertions first, and then start searching,
you do not need a `set`.
The set is a very specific data structure which is needed only
when you have a very specific workload.
If you have a thing which grows
and shrinks constantly dynamically.
Then you have to have a `set`.
You need a `set` only if you need a thing which does not move things around.
As long as something gets into a `set` and it is not erased the pointer, it is fixed.
For example, him sitting in this chair is in the `set`.
As long as he's in this set he will not move from his chair.
You could find him in constant time.
It's a very useful thing except most people do not use set for that.

[^slower]: This performance description is based on several trends in computer architecture.
    The first is that memory, especially [main memory][memory-speed] hasn't increased
    in read/write speed relative to the increase in processor speed.
    So reading and writing to memory is relatively expensive.

    Second, and related is that to compensate CPUs now have many layers of caches
    with layers increasing in speed and decreasing in size as they get closer to the 
    CPU.
    The CPU does a lot of guessing to anticipate what data needs to be in the cache
    for a given section of code. 
    Data structures and algorithms which jump over the address space tend to mess this up
    so the CPU must pause and load data in and out of cache.


[cpp-vector]: https://en.cppreference.com/w/cpp/container/vector
[cache]: https://www.cs.cornell.edu/courses/cs3110/2014sp/lectures/26/memory.html
[memory-speed]: http://gec.di.uminho.pt/discip/minf/ac0102/1000gap_proc-mem_speed.pdf

## Two fundamental performance metrics

There is a great book all of you should have and all of you should aspire to read
(but actual reading that we don't know...)
The book I am referring to (there's only one book
like that in computer science) [Art of Computer Programming][aoc], written by [Donald Knuth][knuth].
It's a very imperfect book but it's the greatest book we got.

One of the things which I want to convince
you of is that we want to be professionals.
As professionals we want to have certain things on our bookshelves even if we don't read them.
Lawyers have all this legal stuff in their office, they don't know
anything about what's inside.
But, they know they have to put it on the shelves.
We at least have to aspire to the same level of professionalism.
We put Knuth on the shelf.

When we started reading Knuth we observed
that even a very long time ago, Knuth started doing measuring operations.
That is, when he would describe an algorithm he would not just say Big O, that came later
(in spite of the fact that he introduced Big O into analysis of
algorithms) but in his book he would carefully annotate how many operations.

I want to get us to a point where we start learning how to
measure things and when we measure algorithms there are two ways we could
measure:

1. Operation counts
2. Measure time

Both of them are important.
Because if you count operations you could have 
analysis of algorithm in terms of abstract operations not depending on the
specific type.
This is very good, we have to have that.
But then we also need to have times for specific commonly used types.
So these are two things which we need to do.
Our goal is to be able to measure this problem with `set` and with sort.
But, first we need to learn more about C++.

[red-black]: https://en.wikipedia.org/wiki/Red%E2%80%93black_tree
[cpp-set]: https://en.cppreference.com/w/cpp/container/set
[cpp-distance]: https://en.cppreference.com/w/cpp/iterator/distance
[cpp-unique]: https://en.cppreference.com/w/cpp/algorithm/unique
[knuth]: https://www-cs-faculty.stanford.edu/~knuth/

[aoc]: https://en.wikipedia.org/wiki/The_Art_of_Computer_Programming
[set-math]: https://mathworld.wolfram.com/Set.html

## Code

- [test_hello_world.cpp](code/test_hello_world.cpp)
- [test_count_unique.cpp](code/test_count_unique.cpp)
