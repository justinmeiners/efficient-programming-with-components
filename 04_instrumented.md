4. Instrumented: a performance measuring tool
================================================

## Great language designers

Here is a little speech about great language designers.
Once upon a time
there was a very great language designer called [Niklaus Wirth][wirth].
He was very brilliant, no question about it, and a very wonderful, kind, charming and witty person.
So he designed his first programming language called 
[Euler][euler] which he did at [Berkeley][berkeley].
Then it was [ALGOL W][algolw].
Then it was [Pascal][pascal].
After that it was [Modula][modula] and then [Modula-2][modula2].
Then [Oberon][oberon].
It was such a beautiful language, this is why all the world
is programming in it (joke).

He's indeed a very brilliant language designer and a great computer scientist
but, he would design a beautiful language, observe that it's limited, throw it away,
and design a better language.
What about the customer base? Well, who cares.
This is why we still have so many people programming in
Oberon. Because, by the time he got to Oberon, people got tired.

This is why I claim, that to be honest
Bjarne is literally the greatest language designer, at least after [John Backus][backus].
John Backus invented the first useful programming language [Fortran][fortran].
It is still a very useful programming language.
Then came C and C++.
Dennis was brilliant.
He did C and then washed his hands and walked away,
which is very wise.
I sympathize. 
But Bjarne started working on C++
in roughly 1978 (and then released in 1981), 35 years ago.
Then he never abandoned us.
It was never perfect, but he would work on it, and work on it, and work on it,
 and go to horrible meetings of the standard committee,
and listen to people who know nothing.
His hair fell out.
It was a terrible, terrible, life.
But, he had this sense of duty to develop the language
further, and further, and further, with the most advanced language
mechanisms known to humankind.
What you can do right now in C++, you cannot
really do in any other language.
But it requires patience, determination
and genius.
Whatever decisions he made in 1979 didn't lead to a stalemate later on.
There is some ugly stuff, but you could avoid it.
Being able to evolve the language for that long is incredible.
I have no other example, not just in language design, but in
computer science.
Ken Thompson did UNIX but do you think he stayed
with UNIX?
No, in his Turing award speech he said he stopped working 
on UNIX a long time ago.
It's very difficult.

I'm a clear example
of a lazy bum. 
STL was voted in August 1994, 20 years ago.
How many times did I attend standard committee meetings after that?
None.
How many times did I look at proposals related to STL? Did I do anything related to this?
Nothing.
This is why I have sanity but also this is
why, compared with Bjarne, I am a failure.
I let people do things with STL that should have been prevented.
I did not evolve it.
I did not grow it.
I walked away.
I know it's a free country, you can.
My advice to most of you, if you want good life follow my
example.
Because it's very hard to do what Bjarne does.
I cannot point to a single other example literally of a person who keeps
working.
[McCarthy][mccarthy] invents Lisp. 
After [1.5][lisp15] he's gone.
He didn't follow.
He didn't contribute, and so on.
Everybody does that, because we are weaklings.

So C++ is a great accomplishment, 
but of course it has flaws.
C and C++ are extremely instructive languages.
People think that studying [Haskell][haskell] is instructive.
I don't know about that, but studying C is very instructive
because Dennis was trying to solve real problems.
So, even his mistakes are very instructive.
C and C++ are great precisely because they are works in progress.

[haskell]: https://en.wikipedia.org/wiki/Haskell_%28programming_language%29
[wirth]: https://en.wikipedia.org/wiki/Niklaus_Wirth
[euler]: https://en.wikipedia.org/wiki/Euler_(programming_language)
[berkeley]: https://en.wikipedia.org/wiki/University_of_California,_Berkeley
[algolw]: https://en.wikipedia.org/wiki/ALGOL_W
[modula]: https://en.wikipedia.org/wiki/Modula
[modula2]: https://en.wikipedia.org/wiki/Modula-2
[oberon]: https://en.wikipedia.org/wiki/Oberon_(programming_language)
[backus]: https://en.wikipedia.org/wiki/John_Backus
[fortran]: https://en.wikipedia.org/wiki/Fortran
[mccarthy]: https://en.wikipedia.org/wiki/John_McCarthy_(computer_scientist)
[lisp15]: https://archive.org/details/lisp15programmer00john

## Instrumented class


Since all STL algorithms are built on fundamental
operations, what we can do is write a wrapper or adapter class `instrumented<T>`
which will take a type `T` and behave exactly like `T`.
You will be able to put `instrumented<T>` into any algorithm, it will behave normally, except
in addition it will count all the operations that are applied to it.

Which operations do we count?
The ones specified in the concepts we discussed.
`T` will be `SemiRegular`, `Regular`, or `TotallyOrdered`.
Then our `instrumented` class will redefine all the operations: copy constructor,
assignment, operator, etc,
adding code to count them.
Then we could take any STL algorithm: sort, unique, stable sort,
whatever you like, run it, and get a performance measurement.
For example:

    std::vector<double> vec;
    // ...
    my_func(vec.begin(), vec.end());

Could be replaced by:

    std::vector<instrumented<double>> vec;
    // ...
    my_func(vec.begin(), vec.end());

And it will count all operations.
Writing this particular class will teach you once and for all to write
`Regular` classes right.

**Exercise:** Before continuing on, try to write the `instrumented` class for yourself and experiment.

### Redefining regular operations with counting

We're going to write `instrumented` using the same technique
we use to write all classes:

1. Copy and paste the [`singleton.h`](code/singleton.h) file we made last time.
2. Replace the string `singleton` with `instrumented`.

Now we will do some work to count operations.
In the copy constructor, we will initialize value,
and add a line that 
bumps up the copy count, like this:

    instrumented(const instrumented& x) : value(x.value) {
        ++counts[copy];
    }

Similarly for the constructor:

    instrumented() { ++counts[default_constructor]; }

One line or three?
This is a very good question.
When I write code I want to do two things:

1. I want to make it as short as possible
2. but I also want it to print nicely

This line is short, so I like one.
I have been changing my programming style depending on the people with
whom I work.
[Paul McJones][paul] affected my programming style greatly when I started here.
For example I never used to use `x`.
I avoided short variable names.
In my old code everything is called `special_variable_x`.
Paul convinced me that `x` is just as good. 

Continue making similar replacements for the rest of the operations
on singleton:

    ~instrumented() { ++counts[destructor]; }
    instrumented& operator=(const instrumented& x) {
      ++counts[assignment];
      value = x.value;
      return *this;
    }
    // Regular
    friend
    bool operator==(const instrumented& x, const instrumented& y) {
      ++counts[equality];
      return x.value == y.value;
    }
    // TotallyOrdered
    friend
    bool operator<(const instrumented& x, const instrumented& y) {
      ++counts[comparison];
      return x.value < y.value;
    }

    // ... other operators should remain implemented as they were


[paul]: https://www.mcjones.org/paul/

### Storing counts

What to do with all the counts?
Where do they get stored?
Every time this `instrumented` thing happens we want some global count to be incremented.
We were told that using global variables is bad.
If I were doing it just for me, I would have used globals.
Old guys don't mind using global variables.
They're actually good.
Since you are modern people, we will show you how to do it with inheritance.
We will define a base class to hold this data:

    struct instrumented_base
    {
      enum operations
      {
        n, copy, assignment, destructor, default_constructor, equality, comparison, construction
      };

      static const size_t number_ops = 8;
      static const char* counter_names[number_ops];
      static double counts[number_ops];

      static void initialize(size_t);
    };

This is a remarkable example of a class containing nothing.
It is a very useful thing, we will use very many such classes.
It's very cheap to pass things which contain nothing.

A static member is a member which is one per class, not one per instance, and they're useful because we don't want to keep count per instance.
We want to keep count per class.
The static members need to be initialized in a `.cpp` file.
At the same time, we provide a table of strings so we can label counts.

    #include "instrumented.h"
    #include <algorithm>

    double instrumented_base::counts[];
    const char* instrumented_base::counter_names[number_ops] = {"n", "copy", "assign", "destruct", "default", "equal", "less", "construct"};
    void instrumented_base::initialize(size_t m) {
      std::fill(counts, counts + number_ops, 0.0);
      counts[n] = double(m);
    }

Why store count in a `double` instead of `int`? Sometimes I want to normalize by `n` to compute a ratio.

I'll tell you a great secret.
People all over the world spread this rumor that I'm absolutely opposed to using inheritance.
This is false.
Inheritance is very useful when you inherit from a class containing nothing because it couldn't do any harm.
That's what we're going to do here.

    template <typename T>
    // T is Semiregular or Regular or TotallyOrdered
    struct instrumented : instrumented_base

There is a notorious problem in C++ with static
members of templates, it's just not good.
We don't need to inherit from a template. All these different
`instrumented<T>`'s inherit from the same base which will contain nothing at
all and we will use this as a counting device.

What is good about this is we managed not to
muck up this nice class.
It's basically the same as singleton.
It's fundamentally of the same structure and we pushed all of the
statistic collection stuff out into a helper class.

### How should we use enum?

`enum` is a mechanism which introduces a bunch of constants.
It's a very evil mechanism.
I was wondering who invented enum,
because it wasn't in first edition of K&R[^kandr].
So I asked Bjarne.
He sent me an email: "Dennis. Under duress."
Dennis didn't quite know how to do it.
He wanted to give us the opportunity to name things. He did not invent it.
It appeared first in [Pascal][pascal] and
the person who invented it was weird.
Whether it worked there correctly or not remains to be seen.

Dennis decided to bring it in,
but the issue is that it's not really a type.
C++ attempts to make it a type but it doesn't quite work.
You could have a variable
typed with the enum which has three different values and then you take totally
different value assigned to it, nothing happens.
The compiler does not prevent you from doing that.
My recommendation is still to use them. Enums are very good when used
in a limited way.
But, do not depend on any operations.
Never depend on a value of a given enum.


[^kandr]: K&R (Kernighan and Ritchie) is a nickname for the book ["The C Programming Language"][c-lang].
    K&R usually specifically refers to the original release.
    A later edition was made when the C language became ANSI standardized,
    and the cover of that edition is labeled as such.

[c-lang]: https://en.wikipedia.org/wiki/The_C_Programming_Language
[pascal]: https://en.wikipedia.org/wiki/Pascal_%28programming_language%29

### Use all the language features

I use inheritance, when appropriate.
In general, I *use any language feature when appropriate*.
Paul and I even use `goto` and we're not ashamed.
There is a famous statement attributed to Ken Thompson that the fastest way of going from one place in the program to another is by using the `goto` statement, and it is so.
If you implement things like state machines it's a
wonderful technique, because you have transitions.
You go from this state to that state.
You could write a loop with some conditional.
Or, you could just `goto` and write very
beautiful code, at least we believe so.
Everything has its place, Dijkstra's strictures not withstanding[^goto].


[^goto]: Goto used to be the primary way to do control flow in programs,
    because it closely resembles  how machines and their languages work.

    For example, to implement a `while` loop, you might write:

        START:
          DO STUFF
          IF CONDITION
            GOTO START
          OTHER STUFF

    It doesn't look bad there, but if you do a lot of control flow
    (especially using adhoc patterns, besides `while`)
    then it becomes "spaghetti code"
    that is difficult to read and follow.
    In a complex program, one must essentially read
    every statement as if you were the computer and jump
    around at each `goto` statement.

    Dijkstra heavily criticized this approach in his famous paper:
    ["Go to statement considered harmful"][goto-paper].

    Alex is observing that it is a good solution to many problems,
    especially when used in a restricted context, and not as the primary
    way to organize programs.
    Later on he will give examples.

    Common Lisp has this idea present in the [`PROG`][cl-prog] feature
    which allows one to use labels and goto restricted to a specific block.
    Fast, assembly-like, messy code can be wrapped in nice functional interfaces.


[goto-paper]: https://homepages.cwi.nl/~storm/teaching/reader/Dijkstra68.pdf
[cl-prog]: http://www.lispworks.com/documentation/lw50/CLHS/Body/m_prog_.htm

## Using instrumented to analyze sort

To learn how to use `instrumented`, let's analyze the performance
of sorting routines in STL.
There are a few of them
and they all use a distinct algorithm:

- [`std::sort`](https://en.cppreference.com/w/cpp/algorithm/sort)

    It uses [quicksort](https://en.wikipedia.org/wiki/Quicksort).
    Why? Because it's "quick".
    It's a wonderful algorithm.

- [`std::stable_sort`](https://en.cppreference.com/w/cpp/algorithm/stable_sort)

    Stable sort does not change the order of things which compare the same.
    For example if you sort people alphabetically, and then stable sort them by
    department, the people will remain alphabetically sorted within every department.
    If you just sort with arbitrary sort, it will be all over the place.

    To be stable you use [merge sort](https://en.wikipedia.org/wiki/Merge_sort).

- [`std::partial_sort`](https://en.cppreference.com/w/cpp/algorithm/partial_sort)

    What's the interface?
    It takes three iterators `first`, `middle`, and `last`.
    What does it do?
    It orders the elements so that `first` to `middle`
    contains the smallest elements from the entire range, in sorted order.

    For example, suppose you give it 100 elements and you want to sort from 1st, to the 10th, to the last.
    The smallest 10 will be moved to the front in sorted order.
    But the last 90 elements will be left in *some unspecified order*.
    Those of you who work on search, know you don't really need to sort everything,
    you just need to sort a little bit[^partial-complete-sort].
    
    What algorithm do you use for partial sort?
    I'll tell you that it's wrong.
    The solution which STL uses was good in 1993, but a bad solution in 2013.
    It uses [heap sort](https://en.wikipedia.org/wiki/Heapsort).
    That's what algorithm books tell you and what I believed was the correct solution[^heap-sort-correction].


We want to compare how these various sort operations perform,
relative to each other.

[^heap-sort-correction]: Alex: There is a perfectly wonderful three
    line solution which could change partial sort and make it much more acceptable to modern computers but it's not the standard.
    It will take the implementers of the standard library 
    another 15 years to catch up.

[^old-heap-algorithm]: Alex: Computers changed.
    The algorithms which worked perfectly wonderfully in 1993, still work in the books.
    But they don't work in the computer.

[^partial-complete-sort]: Alex: If you can sort a little bit, you can sort everything.
    Set the second argument to be the same as the third:
    `std::partial_sort(first, last, last)`.

**Exercise**: With `instrumented`, compare the number of operations
between these three kinds of sort[^implementations].
Refer to the code provided at the end of the chapter.
A complete test harness is provided which will randomly
shuffle a large list of numbers to test with and print the results
in a formatted table.
Here is a sample of the output for heap sort[^pc-info]:

           n        copy      assign    destruct     default       equal        less    construct
          16          69          91          69           0           0          65            0
          32         141         211         141           0           0         168            0
          64         285         479         285           0           0         402            0
         128         573        1102         573           0           0         948            0
         256        1149        2430        1149           0           0        2135            0
         512        2301        5372        2301           0           0        4779            0
        1024        4605       11765        4605           0           0       10617            0
        2048        9213       25551        9213           0           0       23252            0
        4096       18429       55141       18429           0           0       50602            0
        8192       36861      118459       36861           0           0      109348            0
       16384       73725      253428       73725           0           0      235274            0
       32768      147453      539440      147453           0           0      503173            0
       65536      294909     1144418      294909           0           0     1071862            0
      131072      589821     2419727      589821           0           0     2274562            0
      262144     1179645     5101802     1179645           0           0     4811628            0
      524288     2359293    10727118     2359293           0           0    10146937            0
     1048576     4718589    22502934     4718589           0           0    21342599            0
     2097152     9437181    47102018     9437181           0           0    44781006            0
     4194304    18874365    98400338    18874365           0           0    93758166            0
     8388608    37748733   205193953    37748733           0           0   195909864            0


[^implementations]: Many programmers imagine the C++ standard library is a package like
    `sqlite` or `LaTeX` that is centrally developed
    and deployed to many platforms.
    This is not the case.
    Vendors who want to create a C++ compiler and support it on their platform typically develop
    their own library implementation in agreement with the standard.
    There is little or no collaboration on library code between platforms.

    Alex: If you try this on various computers and operating systems
    you will find the counts are different.
    This is because the algorithms are different.
    I always assume, being the guy who did all of that,
    that is the same algorithm wherever I go.
    Obviously somebody modified them a little bit 
    over these 20 years.
    For example, people at Apple did something slightly different from people at GNU.

[^pc-info]: AMD Ryzen 5 2400G (8 core, 3.6 GHz). GCC 9.3.0

### Normalizing data

Another useful way to study operation counts is by *normalizing the data*.
We know the asymptotic complexity of sort algorithms
should be `O(n log(n))`.
So, what we can do is normalize the data
to tell us for `n` elements, how many operations were done, per `n log(n)`.

Here is an example of such a normalizing function:

    double normalized_by_nlogn(double x, double n) { 
      return x / (n * (log(n) / log(2))); 
    }

After normalizing the data, a multiple of `2.86` would mean
it took `2.86` times as many operations as
`n log(n)` predicted.
If `n = 16`, that means `2.86 * 16 log(16) = 183` operations
were counted.

Here is a sample of data for heap sort with measurements normalized:

           n        copy      assign    destruct     default       equal        less    construct
          16        1.08        1.42        1.08        0.00        0.00        1.02         0.00
          32        0.88        1.32        0.88        0.00        0.00        1.05         0.00
          64        0.74        1.25        0.74        0.00        0.00        1.05         0.00
         128        0.64        1.23        0.64        0.00        0.00        1.06         0.00
         256        0.56        1.19        0.56        0.00        0.00        1.04         0.00
         512        0.50        1.17        0.50        0.00        0.00        1.04         0.00
        1024        0.45        1.15        0.45        0.00        0.00        1.04         0.00
        2048        0.41        1.13        0.41        0.00        0.00        1.03         0.00
        4096        0.37        1.12        0.37        0.00        0.00        1.03         0.00
        8192        0.35        1.11        0.35        0.00        0.00        1.03         0.00
       16384        0.32        1.10        0.32        0.00        0.00        1.03         0.00
       32768        0.30        1.10        0.30        0.00        0.00        1.02         0.00
       65536        0.28        1.09        0.28        0.00        0.00        1.02         0.00
      131072        0.26        1.09        0.26        0.00        0.00        1.02         0.00
      262144        0.25        1.08        0.25        0.00        0.00        1.02         0.00
      524288        0.24        1.08        0.24        0.00        0.00        1.02         0.00
     1048576        0.22        1.07        0.22        0.00        0.00        1.02         0.00
     2097152        0.21        1.07        0.21        0.00        0.00        1.02         0.00
     4194304        0.20        1.07        0.20        0.00        0.00        1.02         0.00
     8388608        0.20        1.06        0.20        0.00        0.00        1.02         0.00
    16777216        0.19        1.06        0.19        0.00        0.00        1.01         0.00

You remember Knuth (Author of "The Art of Computer Programming")?
In the beginning of the first volume when he introduces complexity
he tells you how to measure complexity.
He says we measure it as a function where we have
different coefficients for different operations.
This thing should allow us to predict timing
(as we will learn later nothing of the sort is true) so whatever Knuth believed when
he wrote first volume (which he did at least three times) is no longer true.
We will discover that computers got very strange.
They actually do many operations in the same cycle.
So, often
we could do more operations without actually incurring more time.


### What data should we test on?

Let us talk about possible input shapes.
What is a good set of data to test these algorithms on?
The most basic one is just to generate uniformly random data.
Another shape to try is a list which is already sorted.
As we'll discover later on, some sorting algorithms are particularly
bad for this particular configuration.
Both ascending and descending will give different results.
Another interesting shape is a constant list.

In general, we tend to assume all elements we test on are not equal.
It's neither good, nor bad. 
But, eventually we want to define some measure of the ratio of equal
to unequal elements.

Random shuffle of uniform shuffle of random data is very good,
but it's not a very realistic distribution.
One which is very common in real life is called [Zipf distribution][zipf].
Let me describe it incorrectly, first.
Assume that the most probable guy comes with probability `1`.
The second most probable guy comes with probability `1/2`.
The third guy `1/3`, and so on, so it's the [harmonic series][harmonic].
Of course that wouldn't work because they all need to add up to 1 so you
normalize by the sum of the harmonic series up to `n`:

    P(k) = (1/k) / (sum from j = 1 to k of 1/j)

The denominator is actually `ln(n) + gamma`
where gamma is a small number[^stirling].

**Exercise:** Introduce variation into the shape of data and compare
the sorting algorithms again. 


[^stirling]: Alex states that the little bit added on to `log(n)`
    is called the [Stirling number][stirling]. 
    This does not appear to be correct, and he probably meant
    to refer to the [Euler-Masheroni constant][euler-constant].

[zipf]: https://en.wikipedia.org/wiki/Zipf%27s_law
[harmonic]: https://en.wikipedia.org/wiki/Harmonic_series_(mathematics)
[euler-constant]: https://en.wikipedia.org/wiki/Euler%E2%80%93Mascheroni_constant
[stirling]: https://en.wikipedia.org/wiki/Stirling_number

## Measuring solution to unique elements

Counting operations is only one measure of performance.
If we apply `instrumented` to our problem of finding unique elements
in the first chapter, we will find that using
`std::set` actually uses fewer of almost
every operation than first sorting with `std::sort`
and then calling `std::unique`.
However, it is also many multiples slower.

    unique and sort
               n        copy      assign    destruct     default       equal        less    construct
              16          15          89          15           0          15          81            0
              32          43         111          43           0          31         182            0
              64         107         357         107           0          63         460            0
             128         254         649         254           0         127        1038            0
             256         550        1495         550           0         255        2339            0
             512        1214        3253        1214           0         511        5431            0
            1024        2693        7189        2693           0        1023       12168            0
            2048        5903       15203        5903           0        2047       26441            0
            4096       12613       32277       12613           0        4095       59443            0
            8192       27282       68458       27282           0        8191      125661            0
           16384       58938      144763       58938           0       16383      271892            0
           32768      124888      304651      124888           0       32767      586855            0
           65536      264829      640558      264829           0       65535     1269838            0
          131072      560125     1342465      560125           0      131071     2694253            0
          262144     1185967     2817120     1185967           0      262143     5601029            0
          524288     2496461     5881116     2496461           0      524287    11891710            0
         1048576     5223092    12214973     5223092           0     1048575    25679548            0
         2097152    10957202    25458959    10957202           0     2097151    52964702            0
         4194304    22876934    52830701    22876934           0     4194303   111536616            0
         8388608    47627169   109413625    47627169           0     8388607   238823427            0
        16777216    99722291   227797832    99722291           0    16777215   487063364            0
    set sort
               n        copy      assign    destruct     default       equal        less    construct
              16          16           0          16           0           0          81            0
              32          32           0          32           0           0         204            0
              64          64           0          64           0           0         479            0
             128         128           0         128           0           0        1091            0
             256         256           0         256           0           0        2466            0
             512         512           0         512           0           0        5498            0
            1024        1024           0        1024           0           0       12051            0
            2048        2048           0        2048           0           0       26117            0
            4096        4096           0        4096           0           0       56529            0
            8192        8192           0        8192           0           0      121727            0
           16384       16384           0       16384           0           0      259995            0
           32768       32768           0       32768           0           0      553227            0
           65536       65536           0       65536           0           0     1170563            0
          131072      131072           0      131072           0           0     2483786            0
          262144      262144           0      262144           0           0     5230032            0
          524288      524288           0      524288           0           0    10979385            0
         1048576     1048576           0     1048576           0           0    23058567            0
         2097152     2097152           0     2097152           0           0    48192395            0
         4194304     4194304           0     4194304           0           0   100748922            0
         8388608     8388608           0     8388608           0           0   210077288            0
        16777216    16777216           0    16777216           0           0   436448413            0



## Code

- [instrumented.h](code/instrumented.h)
- [instrumented.cpp](code/instrumented.cpp)
- [count_operations.h](code/count_operations.h)
- [test_instrumented.cpp](code/test_instrumented.cpp)
- [test_instrumented_unique.cpp](code/test_instrumented_unique.cpp)



