9. Iterators
====================

## History of iterators

When we wrote `list_pool` before, we didn't provide it with iterators,
because we are learning to be lazy.
Now is a good time to do it, so we can use
other algorithms and learn to write iterators right once
and for all.

Let me tell you a little about them.
Anybody who programs in C++ is forced to use `std::vector`
and those have iterators.
But, there are lots of people who do not quite understand what they are,
partially because iterators are called iterators.
Someone told me that once I shared with them the proper
name for them, they immediately understood.

There is a thing in computer science called iterators which first
appeared in the mid-seventies around 1975 or 1976 at MIT.
It was invented
by [Barbara Liskov][liskov] who was designing a language called [CLU][clu] and she was
interested by another "great and very successful language" (joke) called [Alphard][alphard].
It never existed.
It's a mythical language. It was never implemented.
But, many people at [CMU][cmu] got tenure because of it.
It has some interesting ideas, including the idea of a generator.
For those of you who know [Python][python], it is like an iterator in Python[^python-iterator].
Barbara Liskov said, "wouldn't it be nice to write something like: `for x in thing`".
Iterators allow you to do that[^clu-iterators].
It is like a generator in Alphard.
It is a procedure which returns multiple values, one by one.
It's a procedural abstraction.
In CLU you could only have one iterator to a data
structure and it extended the `for`.
It was a generalization of a control structure.

At the same time, I was working on how to do algorithms and I
introduced the notion of position.
A better name is coordinate, the name which Paul and I use
in our book "Elements of Programming".
A coordinate is some way of indicating where in the data structure you are.
It is not a control structure, it's just the pointer into a data structure,
or generalized notion of a coordinate.
It is something which allows me to navigate through the data structure in a
natural way[^coordinate-references].

Eventually I started talking to C++ guys and showed them coordinates
and they said, "we call it iterator".
Of course they didn't call what I had an iterator.
They had some C++ code where they
were trying to do CLU like iterators, heavy state procedural
things instead of lightweight pointers.
So, I chickened out.
I said, "Yes, that's exactly what you guys have".
I thought it was much better to win with the wrong name 
than lose with the right name, so the name is stuck.
It's in the standard. But again, the concept which it designates is not a
concept of iterator in CLU or iterator in Python or iterator in Java.
Our iterator is a generalization of a coordinate in a data structure.
It's a lightweight thing. It doesn't *do* anything,
it just *points* to something.

There are these arguments which I hear from people like the 
Boost[^boost] guys, who say "Iterators are all wrong. Let us go back
and do ranges."
Guess what? They're reinventing Barbara Liskov's iterators.
Just for the record, when I introduced my iterators
I was very familiar with the iterators in CLU.
Moreover I was even very familiar with Barbara herself
and with [Alan Snyder][snyder] who co-designed the iterators in CLU.
I didn't do what they did because I wanted to do something else.
It wasn't out of ignorance.
Maybe I was stupid, 
but I wasn't ignorant.

[^coordinate-references]: See chapter 7 of "Elements of Programming" on coordinate structures.
    An interesting discussion on the general idea of "coordinatisation"
    is found in chapter 1 of "Basic Notions of Algebra" by Shafarevich.
     
[^boost]: [Boost][boost] is a popular collection of C++ libraries, covering a wide range of uses,
     generally accepted as the next tool to reach for beyond the standard library.
     Many standard library features, such as smart pointers, were initially developed in Boost.
     Alex speaks positively of some parts (see [his foreword][alex-graph-foreword] for "The Boost Graph Library"), but others he is more critical of.

[^python-iterator]:
    Alex almost certainly means Python generators, not iterators, but I will describe both.

    An [iterator](https://wiki.python.org/moin/Iterator) in Python is any object which implements a method called `__next__()`.
    Unlike C++ iterators, the `__next__()` always returns another element of the sequence, not an iterator,
    so they do not resemble pointers or coordinates, neither are they comparable.
    They are most similar to `InputIterator` in that previous values in the sequence become inaccessible after advancing.
    The only thing special about the `__next__()` method is its compatibility with language constructs like `for` loops.

    A [generator](https://wiki.python.org/moin/Generators) in Python is a kind of `iterator` that is typically implemented
    as a function with some helpful syntax additions.
    In a generator function, the `yield` keyword is used to return the next value in the sequence.
    If an additional value is requested after yielding,
    the function will resume at the point of the previous call to `yield`.
    This makes writing complex sequences more natural, as the control flow operates like other code. 
    For example, the following returns square numbers:

        def square_nums(count):
            k = 0
            while k < count:
                yield k * k
                k += 1

    It can be used in a `for` loop:

        for x in square_nums(10):
            print(x)
        # 0 1 4 ...

[^clu-iterators]: See this [brief description of CLU iterators](http://web.mit.edu/ghudson/info/iterators).
    
[liskov]: https://en.wikipedia.org/wiki/Barbara_Liskov
[clu]: https://en.wikipedia.org/wiki/CLU_(programming_language)
[alphard]: https://en.wikipedia.org/wiki/Alphard_(programming_language)
[cmu]: https://en.wikipedia.org/wiki/Carnegie_Mellon_University
[python]: https://en.wikipedia.org/wiki/Python_(programming_language)
[py-iterator]: https://wiki.python.org/moin/Iterator
[boost]: https://www.boost.org/
[snyder]: https://dblp.org/pid/04/4444.html
[alex-graph-foreword]: http://stepanovpapers.com/siekforeword.pdf


## Affiliated types for iterators

We always need to distinguish between how we type
code in C++ and what the notion is behind it.
A key part of iterators is **affiliated types**.
Iterators point to values and you want to know what those values are.
Types don't work on their own.
They come in clusters, or connected families of types.
If you have a type `int*`, there is an affiliated type `int`.
The two types are related. It would be terribly nice if we had a
way to obtain `int` from `int*`. That is, if somebody
gives me a pointer type, I want a way to find out what type
it points to.

To do this we need this notion of **type functions** which accept one type
and return a different type.
This problem of needing to obtain affiliated types is not specific to C and C++.
It appears in Java and Python as well.
In spite of Python's [duck typing][duck] there's *still* a connection
between types, even if they are duck types.

So we need this notion of type functions,
but C doesn't let us do this,
and neither does C++.
Instead of type functions we are going to solve
this problem for iterators by using `typedef`s.

For an iterator, there are [5 types][cpp-iterator-traits] that we always need to define.
Three of them are primary, two are secondary.
Let's start with the primary:

1. `value_type`: the type of the value it points to.

2. `difference_type`: Difference between pointers ([`ptrdiff_t`][ptrdiff] in C).
    Unlike C we might need a different type. 
    The length between elements in a range depends on the range type.
    It is an integral type large enough to encode any valid
    range of a given iterator.

3. `iterator_category`:
    Once again we need to distinguish between how we type this in C++ and what
    the notion behind it is.
    The notion here is that there are different categories, or theories,
    of iterators: `ForwardIterators`, `RandomAccessIterators`,
    `InputIterators`, etc...

    In C++ (without concepts) we use tag types to designate the iterator categories.
    Every iterator uses a tag type to denote which theory it supports.
    The tag lets you do compile time dispatch[^compile-time-dispatch].

### Historical artifacts 

The fourth and fifth types to be defined are required only for historical reasons.
Microsoft said they were going to vote against STL unless it accommodated
multiple memory models.
At the time, they had tiny pointers, huge pointers and
far pointers.
They wanted STL to somehow work with all of them.
I had to figure out 
how they work.
The difference between far pointer and huge pointer is really
weird.
They are both 32 bits.
But, with far pointer if you add
one to it, and the two lowest bytes overflow,
they wrap without propagation to the upper bytes.
With a huge pointer, the carry *is* propagated to the upper bytes, but by adding
8 to them[^ms-dos-pointers].

So they demanded that I change the whole architecture to accommodate them.
Guess how they voted?
No.
Now we're stuck for the next hundred years with stuff which was included to placate people that
couldn't have been placated.

So what does an iterator return when you dereference it?
Normally a reference. It's an [`lvalue`][lvalue]
so you can modify it.
But, with far and tiny pointers, you don't know what the reference
type should be.
So now we need to provide it.

&emsp;4. `reference`: the type of a reference to the value. <br/>
&emsp;5. `pointer`: the type of a pointer to the value.

It's not particularly harmful, but it obfuscates things
and it provides "language experts" with steady employment.

[^ms-dos-pointers]: See ["A look back at memory models in 16-bit MS-DOS"][far-ptr-article]
    for a brief overview of these various pointer types.
    The more general concept behind them is [memory segmentation][memory-segmentation].

[^compile-time-dispatch]: Some algorithms can be implemented more efficiently for certain
    iterator categories. For example [`std::distance`][std-distance] can be
    implemented as a constant time algorithm for `RandomAccessIterators` but
    only a linear time algorithm for other iterator categories. The
    `iterator_category` tag allows the appropriate algorithm to be selected at
    compile time. This technique is known as [tag dispatch][tag-dispatch].

[duck]: https://en.wikipedia.org/wiki/Duck_typing
[ptrdiff]: https://en.cppreference.com/w/c/types/ptrdiff_t
[cpp-iterator-traits]: https://en.cppreference.com/w/cpp/iterator/iterator_traits
[std-distance]: https://en.cppreference.com/w/cpp/iterator/distance
[tag-dispatch]: https://quuxplusone.github.io/blog/2021/06/07/tag-dispatch-and-concept-overloading
[lvalue]: https://en.wikipedia.org/wiki/Value_(computer_science)#lrvalue
[far-ptr-article]: https://devblogs.microsoft.com/oldnewthing/20200728-00/?p=104012
[memory-segmentation]: https://en.wikipedia.org/wiki/Memory_segmentation

## List pool iterators

Let's define these types in the `list_pool` iterator.
What category is the iterator for `list_pool` from last chapter?
We need `ForwardIterator`, because there is no way in a singly linked list
to go backwards.
    
    #include <iterator>

    // class list_pool {
    // ...

         struct iterator {
           typedef list_pool::value_type value_type;
           typedef list_pool::list_type difference_type;
           typedef std::forward_iterator_tag iterator_category;
           typedef value_type& reference;
           typedef value_type* pointer;
         };

    // };

### Constructors

Let's write constructors for our iterator:

    iterator() {}
    iterator(list_pool& p, list_pool::list_type node) :
        pool(&p), node(node) {}

We should explicitly call a constructor when we can.
Default constructor shouldn't really be used
because it guarantees only a partially formed value.
It is only valid in that it can be destructed or assigned to.

This is convenience constructor:

    iterator(list_pool& p) :
        pool(&p), node(p.empty()) {}

### Dereference

What goes inside an iterator? Obviously
an index which lets us get nodes.

    list_pool* pool;
    list_pool::list_type node;

    reference operator*() const {
      return pool->value(node);
    };

    pointer operator->() const {
      &**this;
    };

The arrow operator could be automatically
generated. Sadly it isn't.

You may notice I have a lot of minor annoyances like this.
Whenever you enter a language 
you have to put aside thoughts that things
could be done differently.
If you work in C, you write C.

### Pre-increment, post-increment

When you increment, the iterator should
move to the next node.

    iterator& operator++() {
      node = pool->next(node);
      return *this;
    };


    iterator operator++(int) {
      iterator tmp(*this);
      ++*this;
      return tmp;
    };

`int` here doesn't do anything, it's
just to distinguish pre and post.
Post-increment could be automatically generated
and it would be criminal to do anything else.

### Equality

It's customary here to
pass the arguments by reference.
Why not by value?
Passing by value makes a copy.
At the metaphysical level, what is a copy?
A copy is something which is equal to the original.
*Equality is a more primitive thing*.
So let's try to define it without referring to copy.

    friend
    bool operator==(const iterator& x, const iterator& y) {
      // assert(x.pool == y.pool);
      return x.node == y.node;
    }

    friend
    bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

We could also complain about using `==` instead of `=`
for equality, as it violates mathematical tradition.
But, oh well.
When you took Algebra in grade school,
they used `=` and they used `x` and `y` and I think it's good.

**Exercise:** Experiment with list pool iterators by using
  a standard library algorithm on them, such as `find` or `copy`
  (see `test_list_pool_iterator.cpp` at the end of the chapter).
 
## Thoughts about iterator design

### Should we add safety guards?

Notice I sometimes write assertions in comments:

    // assert(x.pool == y.pool);

I don't use a real assert because it takes too long to check[^assert-modern-compilers].
There is nobody who should be comparing iterators from separate pools.
If he does, he deserves what he gets.
But, wouldn't it be good to guarantee safety?
No. It cannot be done.
Safe programming is bogus programming.
Programming has to be done right.
You have to write correct code.
Checks are good, but you can't
make wrong code correct with syntactic limitations.
Turing machines are fundamentally unsafe.

What about compiling debug mode?
It's mildly useful.
The only truly useful thing is to decompose
your program into clear subroutines and clear units
which you understand.
That's the only thing I know that works.

The reason to program in C++ is to have access to the machine.
You want to have these unsafe things called pointers.
If you want a language which hides the machine,
then use it. It has its advantages.
Python is good for writing scripts.
But don't write your operating system in Python.
Don't write your search engine.
[BASIC][basic] and [COBOL][cobol] were wonderful for what they are.
I wouldn't use COBOL to write an operating system.

[basic]: https://en.wikipedia.org/wiki/BASIC
[cobol]: https://en.wikipedia.org/wiki/COBOL
[cpp-assert]: https://en.cppreference.com/w/cpp/error/assert

[^assert-modern-compilers]: I think modern compilers have fixed this so you can 
    write checks with more confidence that they won't affect release builds.
    Specifically the standard has mandated some rules about when
    [assert][cpp-assert] is enabled.

### Why are forward iterators not comparable?

What is the basic example of a `ForwardIterator`?
A singly linked list.
Forward iterators do not have 'less-than' defined on them.
Is that a good idea?
You could do it for linked lists, but it's very expensive
and not even guaranteed to terminate[^swift-comparable].
But you're assuming "If I am less-than, then I am before".
This is only one interpretation.
Let me explain.

I cannot effectively implement
`<` on `ForwardIterator`, but I was very tempted to include it.
It would do a very simple thing, which is to just compare pointers and
return that comparison.
This would allow me to sort them.
This is an amazing thing.
I don't need the semantics of before or after to be able to use them for fast binary 
search, whether in a map, or whether in a sorted array.
For example, suppose I want to see if an iterator
is in the structure, such as inclusion in a set.
The only effective way is comparison.
I was torn because I knew I could not include it,
because most people would attempt to write
code in STL the old way they learned which is:

    while (i < j) 

They taught you that this was better code than STL style,
but it doesn't work on linked lists or map.
If I provide it for linked lists, people will 
write it, and it will compile and it will work.
(I don't know what that will do.)
So, that was not an option.


[^swift-comparable]: The [Swift][swift] standard library actually takes a lot of inspiration from C++ and Alex's work.
    For example, the protocol [`Collection`][apple-swift-collection] has an `Index` type which is equivalent to `ForwardIterator`,
    with some differences.
    One of these is the `Index` type [must be comparable](https://forums.swift.org/t/dropping-comparable-requirement-for-indices/3290),
    in order to support safety features like bounds checking.
    This restriction makes it difficult to write data structures like linked lists
    and probably makes Alex's pointer comparison trick impossible.

[swift]: https://en.wikipedia.org/wiki/Swift_(programming_language)
[apple-swift-collection]: https://developer.apple.com/documentation/swift/collection
    

### Everything on a computer is totally ordered

But, I still want to emphasize the fact that total ordering is a universally useful thing.
Fortunately on a computer
it is fully defined on every byte.
It's fully defined on every word as an integer.
It's even defined, somewhat on `double` (ignoring bad values).

Equality is naturally extendable to struct.
It was an oversight on the part of C and still C++ again.
The compiler should generate equality,
and it could generate inequality using
[lexicographical][lex] ordering.

So if you want to use iterators
on set, you can define a custom comparator.
One which compares the addresses of the elements to which the iterator points.
It has nothing to do with before and after,
but it establishes a total ordering.
A total ordering does not have to be topologically induced by the traversal.

[lex]: https://en.wikipedia.org/wiki/Lexicographic_order

**Exercise:** Extend the list pool iterator
    with the ability to modify the `next` of the node it points to
    (this is discussed and solved in chapter 12).
    
## Code
   
- [list_pool_iterator.h](code/list_pool_iterator.h)
- [test_list_pool_iterator.cpp](code/test_list_pool_iterator.cpp)
