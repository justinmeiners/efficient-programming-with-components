5. Swap: a fundamental component 
=================================

## The magic spoon

*At this point in the course, Alex changes the classroom
format to require class members to write code and answer questions.
To facilitate this process, he passes around a "magic spoon" which
elects participants.*

I have this magical spoon.
This is truly a very important spoon.
It says "BTL", Bell Telephone laboratories.
When we were young Bell Telephone Company didn't
trust its researchers not to steal spoons, so that's why they marked it.
For all we know [Claude Shannon][shannon] and [Arno Penzias][arno]
and [Ken Thompson][ken] and [Dennis Ritchie][dennis] had their great inspirations after they touched this spoon.
It's a very rare spoon.
It might be the only one in the world.
At some point they got rid of them.
That was the last spoon and I stole it.
I couldn't resist.


[arno]: https://en.wikipedia.org/wiki/Arno_Allan_Penzias
[shannon]: https://en.wikipedia.org/wiki/Claude_Shannon
[dennis]: https://en.wikipedia.org/wiki/Dennis_Ritchie
[ken]: https://en.wikipedia.org/wiki/Ken_Thompson


## What is a component?

When we started this course we said it was about components.
What is a component? One view of a component
is you take a piece of code and rip it out, or something similar.
Well, no. That's not a component.
If you rip a piece of meat out of my leg,
it's not a functioning component.
It's a pound of flesh.

A component is something which solves a problem in a general way.
It's something which is not specific and could be used by all the
applications which need this particular problem solved.
Then comes another important question.
People come to me and say "why don't we use [Go][go], or [Scala][scala],
and many others?"
Let us discuss 
 what components are in terms of a programming language.
I claim that a programming language is suitable for component programming if it satisfies the
following two conditions.

1. You can describe general-purpose components.
2. Without losing efficiency.

### Relative and absolute efficiency

Obviously in any
[Turing complete][turing] language you can describe just about anything, but it might be very slow.
There are many languages which claim to possess powerful abstraction
facilities but if you start using these facilities everywhere,
for example deciding to use `integer` instead of `int32` everywhere (made up example)
your performance is going to collapse.
It's not that you are just going to be slow,
you're going to be *slow compared with the stuff written in the same language* without abstraction.
Efficiency is a two-fold concept:

1. A component is **relatively efficient** if when instantiated it's as efficient
as a non-generic non-component written in the same language.

2. A component is **absolutely efficient** if when instantiated it is as efficient as anything
which could be done on a given machine.
Basically you know it's as fast as assembly language.

These are two fundamental and different kinds of efficiency.

People say, "Alex you use C++ because you sold out to dark forces".
If I sold out to dark forces, I wouldn't be working at my age.
I didn't sell out, so why did I start programming in C++?
After all, I didn't start with C++.
I still program in C++ because as far as I could ascertain it's the only language which allows me
generality and absolute efficiency.
I can program as general as I like.
I can talk about things like [monoids][monoid] and [semi-groups][semi-group][^group-theory]. 
When it compiles I could look at assembly code and see it is good.
It is absolutely efficient.

[go]: https://en.wikipedia.org/wiki/Go_(programming_language)
[scala]: https://en.wikipedia.org/wiki/Scala_(programming_language)
[turing]: https://en.wikipedia.org/wiki/Turing_completeness
[monoid]: https://en.wikipedia.org/wiki/Monoid
[semi-group]: https://en.wikipedia.org/wiki/Semigroup

### Three tests of a language's ability to write components

So now we get to an interesting question,
 how do we know that a language is powerful enough to write components?
You might say a language is good, if you could implement a major operating system.
This is a hard test. 
It's a big project.
It also has a lot of ambiguity.
You obviously don't know whether you meant it in a general way or not.

A long time ago I came up with a very simple test
of whether a language is good enough.
I still use it to determine whether a language is suitable for what I want to do or not.
There are three programs which I need to
implement in a general way to know that the language is suitable. These three
programs are:

1. `swap`: takes two things and swaps them.
2. `min`: takes two things and figure out which one is smaller.
2. `linear search`. goes through a bunch of stuff and finds the one you want.

Aren't these too simple?
If we cannot do simple things, it is very unlikely we will be able to do hard things.
People always say, "I don't really know how to solve those problems, but I could do 
something much more complicated".
I say, look I'm not interested, because I want to see solutions to simple problems.
But people always think that exciting things have to be complicated.
I claim exciting things tend to be very simple and basic.

So you say, "Alex, why don't we use a new language?"
Go try implementing these three program in your favorite language.
Do them in a general way.
If they're at least relatively efficient, that is, they are not slower than specific things 
written in the language, then let us talk.
If you cannot do it, let us stick with C++.
I'm just explaining the reasoning behind my choice of C++.

## Swap

Let us look at these three programs.
Why are they important? 
Why is swap important? What does it deal with?
Apparently it's not self-evident.
Once upon a time I was talking to a very famous programmer,
supposedly the best programmer A9 ever had.
I told him about these three things and he looks at me
and said, "I never had to use swap in my life".
I don't know... I was very impressed because
you swap for sorting,
for reversing the sequence,
for rotating the sequence,
for all kinds of operations.
Basically if you do something with a
sequence, you swap.
So it is very important practically.
But it also happens to be very important
theoretically, because a long time ago when people were starting [group theory][group-theory]
they discovered that any permutation of a sequence could be generated out of swap[^permutation].
Swap is the most primitive operation.
The reason is sequence. And any other
permutation can be constructed out of swap.

But apparently not everyone (even famous programmers)
realized that. 
Well, he had to claim that the language he thought was
the greatest language was great, and since it couldn't do swap,
what do you do?
You deny the utility of swap.

[group-theory]: https://en.wikipedia.org/wiki/Group_theory

[^group-theory]: Groups, monoids, and rings are a few of the subjects of abstract algebra,
    a field which studies the fundamental properties of mathematical structures.
    The key idea is that many different mathematical objects appear to function similarly.
    Vectors and matrices can be "added" and "subtracted" just like integers.
    In what ways are they fundamentally the same?
    One explanation is that all of them form a group.
    Below is a formal definition:
    
    A **group** is a set `G` with a binary operation `* : G x G -> G` such that:

    1. `G` contains an identity element `e` in `G` such that `e * x = x * e = x` for all `x` in `G`.
    2. The operation `*` is associative. So `((x * y) * z)  = (x * (y * z))` for all `x, y, z` in `G`.
    3. Every element `x` in `G` has an inverse element `y` such that x * y = y * x = e.

    For example `G` might be the set of integers and `*` might be addition.

    1. `0 + x = x + 0 = x`
    2. `((x + y) + z) = (x + (y + z))`.
    3. `x + (-x) = (-x) + x = 0`.

    The process of discovering and using generic concepts is very similar.
    Alex introduces the basics of abstract algebra, from a programmers perspective,
    in his book "From Mathematics to Generic Programming".

[^permutation]: A [permutation](https://en.wikipedia.org/wiki/Permutation_group) 
    is a bijection (1-1, onto) map from a set to itself.
    For example we can define the following permutation on the first
    3 integers:

        f: { 1, 2, 3 } -> { 1, 2, 3 }
        f(1) = 2
        f(2) = 3
        f(3) = 1

    Permutations of a set form a group under the operation of composition.
    A basic theorem about permutations
    is that any permutation can be formed by composing transpositions (swaps).
    For example, the permutation above is a composition 
    of two swaps:

        f(n) = g(h(n))

        where g(1) = 3 and g(3) = 1
        and h(1) = 2 and h(2) = 1.

### General swap

Let's write it.
The only type requirement is `SemiRegular` because
we just do copy and assignment

    template<typename T>
    // T is Semi-Regular
    inline
    void swap(T& a, T& b) {
      T tmp(a);
      a = b;
      b = tmp;
    }

Why do we put `inline` on the line before?
It doesn't affect the type signature of the function, and when you search
for it, you don't want to worry about whether `inline` is on the front or not.

### Specialized swap

For some types, this `swap` will perform poorly.
Could you give an example of it being horribly inefficient?
What about a large container?
Consider:

    std::vector<int> a(1000000);
    std::vector<int> b(1000000);
    swap(a, b);

It will construct a temporary vector and copy
every single element into that temporary, and then back
(several million operations)[^move].

So we have generic code which works everywhere, except it's very slow.
What should we do if someone says, "I have a wonderful generic solution, very abstract, but it takes a million iterations when there should be three."?
Throw him out.
There is no excuse. 
Then he says, "Oh, but I could use [tropical semirings][tropical]".
Take tropical semirings and do something to him and them[^tropical].

If you think about the algorithm that needs to take place, 
it requires *knowledge of how vector actually stores data*.
The header (the vector itself) can be copied,
but the pointers to their contents should be swapped.

A central feature of a container is ownership of the elements.
So the elements and container go together.
For things of this kind, we need to write a special swap.

    template<typename T>
    // T is Semi-Regular
    inline
    void swap(std::vector<T>& a, std::vector<T>& b) {
      // swap headers of a and b
      // fix back pointers (for things like linked lists)
    }

It would be wonderful to be able to just type those comments and the 
compiler will do it for us. Sadly enough we're not there yet.

When we write a special version of this function,
it is called **partial template specialization**.
It's partial because we will write for all `std::vector<T>`,
and the `T` parameter is still generic.

[tropical]: https://en.wikipedia.org/wiki/Tropical_semiring

[^tropical]: Alex himself uses Tropical semi-rings to describe
    several algorithms in his book "From Mathematics to Generic Programming" (See chapter 8.6).
    So his issue here is not abstraction itself, rather that it can become too costly.

[^move]:
    Since C++11 this issue has been addressed by [move semantics](https://en.cppreference.com/w/cpp/language/move_constructor)
    and even the swap as we have written it may perform well.

    Essentially, the compiler can detect that we are doing an assignment while
    simultaneously throwing away the source data.
    It can then invoke a special move assignment operator or move
    constructor.
    This can then contain the logic Alex described for swapping pointers.

    Whether this is a good solution, or one that Alex would endorse isn't clear.
    On one hand, move semantics are a great source of confusion and complexity.
    One has to not only be able to read explicit code, but also
    infer the implicit details about how the compiler will treat statements which depends heavily on context.
    You also have to write move versions of many operators.

    On the other hand, it solves this problem generally, so that one does not need
    to write custom swaps for every data structure.
    It shifts the responsibility to the data structure author,
    instead of every algorithm which might use it.

## XOR swap

What if we don't have an extra memory location?
Could we write swap?
Yes, there is a [beautiful algorithm][xor-swap] using XOR[^xor].

    template<typename T>
    // T is UnsignedIntegral
    inline
    void swap_xor(T& a, T& b) {
      a = a ^ b;
      b = a ^ b; 
      a = a ^ b;
    }

**Exercise:** Prove `swap_xor` is correct. To do this you will need to discover
    some basic properties of `^`. (See solution[^xor-proof].)

What are the requirements for this algorithm?
Specifically, what types have an `XOR` operator `^`?
Could we use it on `int`? Yes, but it's a bad idea.
The language standard says that the
result of `XOR` for the sign bit is not defined.
If it is a positive integer you know what is going on for the sign bits.
When it's negative you have no idea.

So use it for `unsigned int`, or `char`.
Basically, unsigned integral types.
So it's not particularly useful.


But there is a case where it doesn't work,
which is weird because we have a proof it does work (if you did the exercise above).
In our proof we made the
small assumption that `x` and `y` are different objects.
Because if they happen to be the same object, the value it contains at
the end of this function will always be zero.
Every bit will be zapped completely totally and absolutely.

We could fix this by wrapping the body in:

    if (&a != &b) {
        ...
    }

Is it a good idea?
In this case, it's important for correctness.
But be careful.
We should never add it to the other swap
because it adds more work to the common case.

Sometimes we are very tempted to say, "if it's five, I have a fast path".
So you add to your code:

    if (a == 5) {
        // do fast path
    } else {
        // do normal path
    }

But if you optimize for 5, and you have more than three integers,
it will seldom be 5 and you will be doing the check all the time.

So two rules: 
    
1. Don't do things unless necessary

2. Don't optimize uncommon cases

[^xor]: The `^` symbol is bitwise [exclusive or](https://en.wikipedia.org/wiki/Exclusive_or).
    The expression `a ^ b` means `a` is true, or `b` is true, but not both.
    It is defined by the following truth table:

        a | b | a XOR b
        ----------------
        0   0   0
        1   0   1
        0   1   1
        1   1   0

[^xor-proof]: We need to use the fact that XOR is associative and commutative.

    Proof:
        1. a = (a ^ b)        b = b
        2. a = (a ^ b)        b = (a ^ b) ^ b
                                = a ^ (b ^ b)
                                = a ^ 0
                                = a

        3. a = (a ^ b) ^ a    b = a
             = b ^ (a ^ a)
             = b ^ 0
             = b

[xor-swap]: https://en.wikipedia.org/wiki/XOR_swap_algorithm

## Is the inline keyword important?

`inline` is one of the things which will go away.
There are certain things in C and C++ which are there because compiler technology was imperfect.
When I started in C++ in 1986
I had to write the keyword [`register`][register] everywhere because, believe it or
not, compilers wouldn't use registers[^registers] unless you specifically indicated that something goes into the register.
Of course if it went into
the register you could never use address operator `&` because obviously registers do not have addresses.
It was a very special thing you needed to worry about.
It was important in my measurements at the time.
Stripping `register` declarations from fundamental algorithms caused a slowdown by a factor of three.
Why? 
For every call to `++` the assembly code first did a load 
and then after it stored the result.
At that time computers used to do one thing at a time.
So by adding a load and store around everything, it basically tripled the time.

This is no longer true, meaning that computers no longer
execute one operation at a time, as we will discover.
For sure, you never need to worry about registers.
In modern computers this is utterly idiotic; you should never do it.
In the same way the compiler is perfectly theoretically capable
of figuring out what needs to be `inline`, much more than you.

But, we're living in
this transition time.
I think about five years from now you will never need
to write `inline`.
Compilers will do it.
Right now it still makes a difference.
You remove this `inline` and you
could get enormous performance difference.
It could be a factor of 10 for something like swap.
The problem is that the function call sequence,
is a bad sequence.


[register]: https://en.cppreference.com/w/c/language/storage_duration

[^registers]: 
    In CPU architecture, a register is a slot on the CPU
    that can store a single value (typically 32 or 64 bits).
    Most CPU operations are confined to operating on values in registers.
    For example, an "add" instruction might add the value in one register,
    to another register, and then store the value in a third register.
    Separate "load" and "store" instructions
    are used to move a value between a register and a location in memory.

    Typically a CPU has only a handful of registers (fewer than 50), so a large part of the program
    is spent moving values from memory into registers so they can be operated on,
    and then transferring the results back to main memory.


## Code

- [swap.h](code/swap.h)
