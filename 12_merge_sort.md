12. Merge Sort
=================

## The discovery of generic programming

I have to tell you a story about fish.
I was young once.
The year was 1976.
Some of you were not even born.
I was a good programmer.
I was writing operating systems and doing a good job.
But, I aspired to greater things.
When you are young you aspire. 
There was an opening in one very prestigious institute, [Soviet Academy of Science][academy] to work
on the design of a parallel computer.
I wanted this job really badly.

A friend of mine who
worked at this Institute recommended me to this group.
I knew I was going to go
there for for an interview and I'd have to say some wonderful things or they
would not give me a job.
Right before my interview,  terrible thing happened.
I ate some raw fish and it was very tasty but within
 eight hours my temperature was 103.
I was in a bad situation.

I'm in the hospital flying above my bed.
That happens to you when you have high fever.
But,  what I'm thinking about is this upcoming interview about parallel computers.
I know nothing about parallel computers not now, nor
ever, but I really want the job.
So I need to come up with some brilliant idea.
So I'm floating above the bed in space and thinking.
Then I suddenly realize how to add four numbers in parallel I said oh I could add them
in parallel if I add the first two and the second two in parallel:

         +
        /  \
       /    \
      +      +
     / \    / \
    x   y  z   w

I had this vision of this huge addition tree.
When you're sick amazing things happen in your mind.
Then I realized the second great thing, while still floating.
It could be multiplication and the same thing will work.
I started realizing more and more
functions  will work `min`, `max`, but division will not work.
Then I realized it's good to be sick.
It has to do something which I almost forgot called abstract algebra.
How is this related?
I have been teaching the whole course[^math-study] so I remember that
there is this thing called associativity.
I realized that this idea will work as long as the operation is associative.

This became my central theme.
how could I talk about associative operations?
how could I write algorithms like that?
When I came to United States somewhere along the way,
maybe in Austria, I realized that merge was associative
It was a very big deal because I never even thought about merge in terms of `+` and `*`, but merge is associative.
Then I realized  I could do merge sort with parallel reduction.
That's the summary.
Alex, what have you been doing all your life? This.
You might say, "that's not good enough".
Probably not.

I never talked about that discovery, during the job interview.
I did get the job and it wasn't particularly good.
It wasn't good at all.
But, things happen[^story].


[academy]: https://en.wikipedia.org/wiki/Russian_Academy_of_Sciences
[prolog]: https://en.wikipedia.org/wiki/Prolog
[horn-clause]: https://en.wikipedia.org/wiki/Horn_clause

[moscow-state]: https://en.wikipedia.org/wiki/Moscow_State_University

[^math-study]: Alex studied math at 
    [Lomonosov Moscow State University][moscow-state] from 1967-1972.
    He was a professor in the United States, so I believe this was a research
    or graduate level degree (although he does not appear in Math genealogy, or perhaps his name was Americanized).
    Based on my reading, I believe his area of research is number theory.
    I believe he is alluding to teaching an abstract algebra course as part of
    his graduate or post-graduate studies.

[^story]: This story can also be found in [this interview](http://stepanovpapers.com/LoRusso_Interview.htm).

## Iterators as a concept

Generic programming means
to me finding algebraic structures on which algorithms are defined and writing them
like that.
The second idea is that it will be terribly nice if using beautiful
programming at that level wouldn't cost us anything.
Anybody can do beautiful things if it's very slow.
This is why we design languages like [Prolog][prolog].
They don't care as long as they could write it as [Horn clauses][horn-clause], or
things like that. 
So it's the combination of two things:

1. Doing programming in mathematical theories
2. not losing efficiency

We are slowly hinting more at concepts.
How is a concept different from a class?
How is a concept different than an abstract class
or interface in java?
One way to see this is try to think of how to specify
the concept of an iterator using them.
Consider that iterators have a `value_type`.
In an interface, every argument must have a fixed type.
Interfaces specify *identical interfaces*.
A concept defines *similar interfaces*.
The concept of a pointer doesn't define 
the same interface for every type.
The `*` operator can point to `int`, or `double`, or who knows what.
*Concepts have variance on the types.*

Think also about `==`.
It is a binary predicate.
If you use inheritance, you can only modify the type of the first argument.
The second argument stays the same.
In Java you can kind of fix it by doing lots of casting.
But, the other problem is more serious as you can't fix the fundamental variance on `value_type`.

People say, "but couldn't there be a correct inheritance?
Of course, yes.
But it's not the inheritance we have in C++ or Java.

Because we don't have concepts, when we write
functions which take an iterator as an argument you cannot say what 
requirements it has.

### Kinds of iterators

There are several [iterator concepts][cpp-iterator-concepts] in STL[^sgi],
all of which we have mentioned before:

- [`InputIterator`][cpp-input-iterator]: can only advance forward and once advanced, iterators pointing to previous elements become invalid. 
- [`ForwardIterator`][cpp-forward-iteartor]: can advance forward and have iterators pointing to previous elements.
- [`BidirectionalIterator`][cpp-bi-iterator]: can move iterators forward and backward.
- [`RandomAccessIterator`][cpp-random-iterator]: can advance iterators by arbitrary steps in constant time (like pointers).

[cpp-iterator-concepts]: https://en.cppreference.com/w/cpp/iterator
[cpp-input-iterator]: https://en.cppreference.com/w/cpp/named_req/InputIterator
[cpp-forward-iteartor]: https://en.cppreference.com/w/cpp/named_req/ForwardIterator
[cpp-bi-iterator]: https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator 
[cpp-random-iterator]: https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator 

[^sgi]: This is a subject for which the [SGI Documentation][sgi-iterator-concepts]
    can be a better resource.

[sgi-iterator-concepts]: https://www.boost.org/sgi/stl/Iterators.html
### Linked iterator

There are some people who say, "concepts aren't important because Alex already exhausted them.
there are just four kinds of iterators and that's all."
There is a "great" person who goes around and gives talks saying this.
I'm not going to name him[^andrei].

What I'm going to do now is introduce a new kind of iterator,
just to spite this distinguished person.
We want to extend the concept of an iterator with a new concept of a `LinkedIterator`.
It is a `ForwardIterator` which in addition to
the normal operations:

- `++`: advance
- `==`: equality
- `*`: dereference

It has one additional operation which allows
you to set the successor.
If you have an iterator and you have another one,
you can just make the second iterator the successor
of the first.
You can literally connect them.
Of course, the standard model is a linked list.

To make our `list_pool` iterator a `LinkedIterator`
we simply add the following function:


    void set_successor(iterator x, iterator y) {
        x.pool->next(x.node) = y.node;
    }

We want it to be a function not a member
because primitive/built-in
types don't have member functions.
Furthermore, in the case of an iterator,
it might be a pointer, not a class.
In general, I don't like member functions.
If you have the option of:
    
    x.foo()

Or 

    foo(x)

There is at least one provable advantage.
It has one fewer character[^character].
But, since we think generically, a better
question is whether the function could operate
on a built-in type instead of a class.

[^andrei]: I suspect he is referring to [Andrei Alexandrescu](https://en.wikipedia.org/wiki/Andrei_Alexandrescu) who gave a talk
    ["Iterators must Go"][iterators-must-go].

[^character]:
    This argument doesn't hold up if there are arguments:

        x.foo(y)

    Versus:

        foo(x, y)

    I suppose you could drop the space:

        foo(x,y)

    Many languages such as Swift now also allow you to add member functions
    to primitive types like `int`.


[iterators-must-go]: https://accu.org/conf-docs/PDFs_2009/AndreiAlexandrescu_iterators-must-go.pdf

### Linked iterator is "unsafe"

As long as you don't do it, the topology remains.
But, if you do, things change.
This is a very unsafe operation.
You can even make circular lists.
Some people say this is very bad.
It should not be allowed.
We have to come up with language mechanisms for not doing this evil.
These are the people who believe that there are some syntactic rules that can make programming good.

Are they right?
Why not in the case of circular lists?
They are very useful.
Of course, if you try to get to the end,
you will not be successful.
But, it is a legitimate data structure.

A long time ago when we programming in Lisp,
there were people saying, "never use `rplcd`" (set successor).
They were wrong.
Use whatever is given to you.
But, be wise.
You can write bad code just with `+`.

    while (true) a = a + 1;

That doesn't terminate.
Well maybe just `while (true)` is bad.

    a = 1;
    while (a > 0)  { do stuff }

There many ways of writing bad code
and syntax can't help you.
No smart pointer or syntactic constraint
will make a bad programmer into a good programmer.
It will just replace one sort of bugs with another.
Bad programmers are very creative,
they find amazing things.

## Reverse linked ranges

To understand how this concept works
we will look at a basic algorithm.
If I hadn't already shown you `set_successor`
we can learn it's use from this algorithm.
It takes two lists, reverses the first,
and attaches it to the second.
It's a very important list algorithm.
In scheme `(append! (reverse! x) tail)`
or `(nconc (nreverse x) tail)`
in Common Lisp.

    template <typename I>
    // requires I is LinkedIterator
    I reverse_linked(I first, I last, I tail) {
      while (first != last) {
        I next = first;
        ++next;
        set_successor(first, tail);
        tail = first;
        first = next;
      }
      return tail;
    }

## Merging sorted lists

### Simple merge

The following function merges two sorted lists,
using the algorithm we just wrote.

    template <typename I, typename Compare>
    // requires I is Linked Iterator
    I merge_linked_simple(I first1, I last1, I first2, I last2, Compare cmp) {
      I result = last1;
      while (first1 != last1 && first2 != last2) {
        I tmp = cmp(*first2, *first1) ? first2++ : first1++;
        set_successor(tmp, result);
        result = tmp;

      return reverse_linked(result, last1, first1 == last1 ? first2 : first1);
    }

Notice that a good thing about iterators
is it helps us avoid writing `nil`/`null`.

It's called simple because it's simplistic.
Why?
In terms of complexity it takes two passes.
We have to go through the lists, and merge them,
roughly `n` successors on the way there.
Then we do the same number doing the `reverse_linked`.
Also note that algorithmically there is no good reason why we evaluate

    while (first1 != last1 && first2 != last2)

On every single iteration, because only the one iterator
we moved in the loop could become empty.
We will come back to this.

**Exercise:** Write a theoretically more efficient merge which does not do this extra comparison (solved just below).

### Improved merge 

I clearly teach you all the wrong things.
I tell you not to use
inheritance, smart pointers, or read certain books.
Now I'm going to teach you to use `goto`.
The greatest authority in computer science wrote a famous letter to
communications of ACM called, ["Go To Statement Considered Harmful"][harmful].
There is is nothing harmful in the computer.
The computer is good.
It has an instruction called [JMP][jump] (or branch).
If it's there, why not use it?

If you are going to use `goto`, it's probably
because you have something like a finite state machine.
The state machine goes between fundamental states.
You probably learned about them in school, but they never told
you it relates to `goto`.
What are the states which we need for this algorithm?
Which list won the comparison and advanced.
Either the 1st, or the 2nd.

    template <typename I, typename Compare>
    // I is a linked forward iterator
    std::pair<I, std::pair<I, I> >
    merge_linked_non_empty(I first1, I last1, I first2, I last2, Compare cmp) {
      I head, tail;
      if (cmp(*first2, *first1)) {
        head = first2;
        tail = first2;
        ++first2;
        goto winner2;
      } else {
        head = first1;
        tail = first1;
        ++first1;
        // goto winner1;
      }
    winner1:
      if (first1 == last1) goto empty1;
      if (!cmp(*first2, *first1)) {
        tail = first1;
        ++first1;
        goto winner1;
      } else {
        set_successor(tail, first2); 
        tail = first2;
        ++first2;
        // goto winner2;
      }
    winner2:
      if (first2 == last2) goto empty2;
      if (cmp(*first2, *first1)) {
        tail = first2;
        ++first2;
        goto winner2;
      } else {
        set_successor(tail, first1);
        tail = first1;
        ++first1;
        goto winner1;
      }
    empty1:
      set_successor(tail, first2);
      return std::make_pair(head, std::make_pair(first2, last2));
    empty2:
      set_successor(tail, first1);
      return std::make_pair(head, std::make_pair(first1, last1));
    }

Let's go through the states.
Right away,
we need to determine who the winner is.
For performance this part doesn't matter
because it's not in the inner loop.
`winner1` means the smaller element
came from the 1st range, and similarly
for `winner2`.
We compare just the one that advanced.
The two states are symmetric.

I return 3 iterators.
Why?
It's specific to linked lists/iterators.
If I want to take the resulting list
and attach it, I need to be able to modify
the successor of the final node.
But, if we return the end, it's probably
`nil` so I don't have it.
Just return you all the information.
The caller can do whatever they please.
If they want to ignore it, ignore it.

Note that it assumes the lists are nonempty,
which is perfectly fine for our sort,
which is not going to merge empty lists[^non-empty].


[jump]: https://en.wikipedia.org/wiki/JMP_(x86_instruction)
[harmful]: papers/goto-harmful.pdf


[^without-goto]:
    Alex: "Can you write it without `goto`?
    I  have this in code going back to 1985.
    I wrote it then in Scheme without `goto`,
    but it had other efficiency problems.
    Since then I have published the code, multiple times.

    The first is in my [Ada][ada] book on generic libraries
    ("The Ada Generic Library Linear List Processing Packages").
    I got such angry letters, especially from Holland,
    saying, "don't you know that `goto` is harfmul?".
    I couldn't find another solution".

[^non-empty]:
    In Elements of Programming Alex often follows the pattern
    of creating a function which works well for some given assumptions
    (such as the list being nonempty) and then creates a wrapper
    which does additional checks or work to ensure the assumptions
    are met.
    This makes the algorithm more modular and faster
    for those other components which can ensure the assumptions.
    


[ada]: https://en.wikipedia.org/wiki/Ada_(programming_language)

### Is it worth it?

We now have two programs.
One of them is oh so very simple.
The other one is elegant, but long and does a  minimal number of operations.
Which one should we use in practice?
We need to do a lot of experiments to establish certainty
of what we are doing[^test-result].

**Exercise:** Measure and determine whether it's actually faster than our simple merge.

Can we think of any more performance improvements?
In terms of operations we have cut it to the bare bone.
But, one thing is [branch mispredictions][branch].
Modern processors try to predict which
way a condition is going to go.
If you look at our branches, the probability
of going one way or the other is 50 percent.
That's literally the worst thing which which could happen.
I'm entering the territory where I haven't done work yet.
I haven't tried see whether some kind of predication avoidance of `goto`
could be done.

**Exercise:** Try to write a version of this algorithm which is more friendly
    for branch prediction.

[branch]: https://en.wikipedia.org/wiki/Branch_predictor

[^test-result]:
    In my test (Intel i5, 4 core, g++ 9.3.0)
    I noticed about a 15-20% improvement
    sorting one million integers.

## Merge sort

Now that we can merge lists and we have our binary counter,
we can write merge sort.
For our machinery, we need to wrap this operation in an object.
There is no code, it's just a way of invoking our merge function.
It has 5 arguments, and we need a binary function.

    template <typename I, typename Compare>
    // I is Linked Iteratork
    struct mergesort_linked_operation
    {
      typedef I argument_type;
      I nil;
      Compare cmp;
      mergesort_linked_operation(I nil, const Compare& cmp) : nil(nil), cmp(cmp) {}
      I operator()(I x, I y) { return merge_linked_simple(x, nil, y, nil, cmp); }
    };

The purpose of this is not to learn to sort lists.
It is of course a useful skill.
The purpose is to show you this code.
Observe it is the same machine as we had before.

    template <typename I, typename Compare>
    // I is Linked Iterator
    I mergesort_linked(I first, I last, Compare cmp) {
      mergesort_linked_operation<I, Compare> op(last, cmp);
      binary_counter<mergesort_linked_operation<I, Compare> > counter(op, last);
      counter.reserve(16);
      while (first != last) {
        I tmp = first++;
        set_successor(tmp, last);
        counter.add(tmp);
      }
      return counter.reduce();
    }

It is very efficient, and doesn't do any kind of list splitting[^bad-code].
If you don't find it's beautiful I have nothing to teach you.
We aren't on the same wavelength.
In the modern world, you meet somebody
somebody and
say, "[Bach][bach] is a great composer".
He says, "nah, [Lady Gaga][gaga] is much more gifted".
It's a free world.
People are allowed to say whatever they like.
Some  people will say,  "oh this is not object-oriented", or "this is not functional",
which it isn't and I'm very proud of it.
But, in some sense this is literally the essence of my life work, this piece of
code. 
That's where it started.
That's where it ends.
The majority of people do not get it.
The majority of computer scientists do not get it.
There is absolutely no indication that getting it will make you rich.

[^bad-code]: 
    Alex: If you want to see a really bad program
    see [Patrick Henry Winston][winston]'s
    book ["LISP 1st Edition"][lisp-book].
    Look at his sorting algorithm for list.
    It posses many remarkable properties including using `n log(n)` extra storage.
    Good example of a famous person at a respectable school
    publishing something terrible.
    Published does not mean good.

[lisp-book]: http://people.csail.mit.edu/phw/Books/LISPBACK.HTML
[winston]: http://people.csail.mit.edu/phw/index.html
[bach]: https://en.wikipedia.org/wiki/Johann_Sebastian_Bach
[gaga]: https://en.wikipedia.org/wiki/Lady_Gaga

## Code

- [list_algorithm.h](code/list_algorithm.h)
- [test_merge_sort.cpp](code/test_merge_sort.cpp)

