10. Balanced binary reduction
==============================

## Alice in wonderland 

Let us introduce the problem of finding not just the smallest
of `n` elements, but the smallest and second smallest.
The problem has a very distinguished pedigree, it was first addressed by a well-known British mathematician
[Charles Dodgson][carroll] (Lewis Carroll).
If you haven't heard of him, you should.
There is a very important book which he wrote, not the [mathematical book][carroll-logic],
but the book called ["Alice's Adventures in Wonderland"][alice-in-wonderland].
If you haven't read it, do[^alice-free-ebook].
No person should be hired ever unless they read "Alice in Wonderland".
In any case, he was also a mathematician.
He also dabbled in all kind of games.
Apparently he invented Scrabble and a bunch of other games[^inventor-of-scrabble].

At some point he decided that there is a clear problem with lawn tennis tournaments.
He observed that with a very high probability,
if you have say a tournament with 64
players, the guy who gets the second prize is actually not the second strongest.
He observed that the strongest and
second strongest could be paired in the first round.
Therefore the second strongest guy gets eliminated and doesn't get the second prize,
in spite of his prowess.
This is why they now use a technique known as [seeding][seed] to assure
that people of similar ability are spread out to different parts of the tree.
But he wanted to come up with an algorithm
which assures that the second-placed guy is truly the second best player.
He published it in 1883[^lawn-tennis-article].
The algorithm wasn't quite an algorithm[^not-an-algorithm] and it was clearly not optimal.
It took 50 more years before the problem was stated correctly.
People realized that you could talk about the minimum
number of comparisons but it took another thirty years, until 1964 when a
Russian mathematician [Sergei S. Kislitsyn][sergei] published a paper which 
proved there is an optimal algorithm and described it.

By the way, all of this information is available in a
book called ["The Art of Computer Programming"][aoc] by [Donald Knuth][knuth][^aoc-second-smallest-ref].
You really should buy it.
You make a certain commitment.
You spend $150 saying that you really care about programming.
This is not a book which is "useful", meaning it's not "programming in Python for idiots",
or "information retrieval for 21st century" or something like that.
This is one of the fundamental books which you buy and then spend your
lifetime getting the information out of it.
You get beautiful things which you then could use for programming in Python or information retrieval or other things.
I'm going to be mentioning Knuth throughout the course.
It's not a perfect book, it is just the greatest book we've got.
Some people think it's a good reference book.
No, it's not, because you have to
basically do linear search to find what you're interested in.
Another important thing; do not spend too much time solving problems.
Read the solutions.
They are right at the end.
Lots of very important algorithms are described in the solutions to his problems.
Reading Knuth has to become a lifelong activity.

[^aoc-second-smallest-ref]: See chapter 5.3.3 in Volume 3 of "The Art of Computer Programming".

[^not-an-algorithm]: Knuth: it is not formulated precisely enough to qualify as an algorithm.

[^lawn-tennis-article]: "Lawn tennis tournaments; the true method of assigning prizes with a proof of
    the fallacy of the present method". London, Macmillan and co., 1883.

[^inventor-of-scrabble]: The invention of Scrabble is attributed to Lewis Carroll's brief journal entry:
    "A game might be made of letters, to be moved about on a chess board till they form words" (Dec 19th,
    ["The Life and Letters of Lewis Carroll"](http://www.fullbooks.com/The-Life-and-Letters-of-Lewis-Carroll3.html) ).
    See also ["History of Scrabble"](https://scrabbledaily.blogspot.com/2008/05/history-of-scrabble.html)
    and ["The games of Lewis Carroll"](http://www.bananagrammer.com/2009/10/games-of-lewis-carroll.html).

    His book ["The Game of Logic"](https://en.wikipedia.org/wiki/The_Game_of_Logic) teaches formal logic
    using a board game. It is also available from [Project Gutenberg](https://www.gutenberg.org/ebooks/4763).

[^alice-free-ebook]: The book is freely available from [Project Gutenberg](https://www.gutenberg.org/ebooks/11).

[alice-in-wonderland]: https://en.wikipedia.org/wiki/Alice%27s_Adventures_in_Wonderland
[carroll]: https://en.wikipedia.org/wiki/Lewis_Carroll
[scrabble]: http://www.bananagrammer.com/2009/10/games-of-lewis-carroll.html
[seed]: https://en.wikipedia.org/wiki/Seed_(sports)
[knuth]: https://en.wikipedia.org/wiki/Donald_Knuth
[aoc]: https://en.wikipedia.org/wiki/The_Art_of_Computer_Programming
[sergei]: http://www.mathnet.ru/eng/person27000
[carroll-logic]: https://www.gutenberg.org/ebooks/28696
[lawn-tennis]: https://en.wikipedia.org/wiki/History_of_tennis


## Smallest and second smallest element

The problem of finding the smallest and second smallest element
is described fully in "The Art of Computer Programming", but somehow Knuth does not implement it.
As we shall see it's actually a little tricky.
It will take two chapters to complete.

How many comparisons do you need to solve this problem?
Same as `minmax_element` from last time?
No. 
You can do it in fewer.
Let us try to use some logic.
How many comparisons do we need to find the winner of the tournament? `n - 1`.
It is necessary to find the winner in order to find the second place guy.
We could sketch a proof of this.
Let us assume there are two potential guys greater than the second place guy.
If there are none, he isn't second place.
If there are two, he isn't second place either.

What do we know about second place guy (specifically the games he lost)?
*He only lost one game, and it was to the winner*.
If the winner remembers all the games he won, and who he played,
how could we determine second place? (We know the second place guy is one of them.)

How many people does the winner beat to win?
For example, [Wimbledon][wimbledon] has 64 players who are admitted.
The winner doesn't play 63 games.
The tournament is structured as a [binary tree][binary-tree].
This tree is how deep? 
If you have `n` elements? It's `ceil(log_2(n))`.
We could somehow arrange our tournament so that the 
winner will defeat this many people.

     winner: d
     second place: a or c
     
         d 
        /  \
       a    d
      /\    /\
     /  \  /  \
    a   b c    d    ceil(log_2(4)) = 2

Now that we have a list of the `ceil(log_2(n))` people who played the winner,
we just find the best out of them which is `ceil(log_2(n)) - 1` comparisons.

To review, we have `n - 1` comparisons to get the winner.
`ceil(log_2(n)) - 1` to get the second best, from the list he played.
So an upper bound on the comparisons for the algorithm is:

    n + ceil(log_2(n)) - 2

Actually implementing this algorithm effectively
will require us to create several components.

[wimbledon]: https://en.wikipedia.org/wiki/The_Championships,_Wimbledon
[binary-tree]: https://en.wikipedia.org/wiki/Binary_tree

### What about divider and conquer?

It might appear you could use divide and conquer.
First split the list of elements in two, 
find the min and second min of the first half,
and the second half, and then merge them together doing two comparisons.
It sounds very elegant because it's all recursive.
But, let us think about how many comparisons it's going to do
using simple mathematics.

1. We start with `n`. We need to pair and compare them,
   so the first round is `n/2` comparisons.

2. In the second round we pair up the results,
   so we need `n/4` "games".
   But, each game requires two comparisons (to find min and max).
   So we have another `n/2` comparisons.

3. And so on...

So the total number of comparisons would be:
    
      n/2 + (n/2 + n/4 + n/8 + ...)
    = n/2 + n-1
    = 3n/2 - 1

That's not what we're trying to accomplish, so divide and conquer doesn't always do what we think.

### Tournament tree shapes

To get the number of comparisons that we want, we need to rearrange the tournament we play.
Right now `min_element` plays a tree structure that looks like this:

    unbalanced tree

    /\
      /\
        /\
          ... 
           /\

It has `n - 1` internal nodes.
But we don't want the winning element to do `n - 1` comparisons.
We need to transform that into the way they play tennis tournaments.
We need to balance the tree.

    balanced tree

        / \
       /\ /\
        ...
      /\ /\ /\

How do we do it?
One way is to just pair up elements and build up.
But then we need lots of memory to save the intermediate results[^early-ref-to-inplace].
Note that once a bottom-level round has been played, they are ready to move up.
Our goal is basically to become eager.
Whenever elements are ready to be paired together, we want to pair and compare them.

So if we store only the winner at each level, we never need to store `log(n)` things.
We can define the **power** of each element
to be the number of games they have played.

Realize that suddenly we see something which has nothing to do with our problem.
*The foundation of our algorithm is the ability to take a tree, like
the linear (unbalanced) tree, and transform it into a balanced tree*.
What mathematical property allows us to do such a transformation?
Why can we convert one kind of computation to the other?
**Associativity**[^associativity].
As long as our operation is associative, 
what property don't we need? **Commutativity**[^commutativity].
We keep the elements in the same order,
we're just rebalancing parenthesis[^min-not-commutative].

[^early-ref-to-inplace]: Alex: What do we mean when we say lots of memory?
    `O(n)` is bad, `O(sqrt(n))` is pretty bad.
    See the definition of "in-place memory usage" at the end of the chapter. 

[^min-not-commutative]: Alex: If you think about it,
    our `min` is not quite commutative.
    In mathematics `min` is commutative.
    But because we want to preserve stability, it is not.
    We distinguish between the left and right argument.

[^associativity]: A binary function `f` is [associative](https://en.wikipedia.org/wiki/Associative_property)
    if the the following holds for all `a, b, c` in its domain:

        f(f(a, b), c)) = f(a, f(b, c))

    Informally, `f` can be applied in any order.
    For example, addition of integers is associative:

        (a + b) + c = a + (b + c)

    Subtraction of integers is not associative:

        (3 - 2) - 1 != 3 - (2 - 1)

    You can see why associativity is often referred to as being
    able to "re-parenthesize" expressions.

[^commutativity]: A binary function `f` is [commutative](https://en.wikipedia.org/wiki/Commutative_property)
    if for all `a, b` in its domain:

        f(a, b) = f(b, a)

    Informally, `f` gets the same result, regardless of the order of the inputs.
    For example, multiplication of integers is commutative:

        a * b = b * a

    In Chapter 9.1 of "From Mathematics to Generic Programming", Alex gives
    a neat visual proof of the commutativity of positive integers:

                      * * *
        * * * * *     * * *
        * * * * *  =  * * *
        * * * * *     * * *
                      * * *

    Or as [Dirchlet][dirichlet] put it: "Whether you arrange soldiers in rows or columns, you still have the same number of soldiers".

    An example of an operation which is not commutative is string concatenation. 

        "Hello, " + "World!" != "World!" + "Hello, "

[dirichlet]: https://en.wikipedia.org/wiki/Peter_Gustav_Lejeune_Dirichlet)

## Binary counting and reduction

Here we come to the amazing idea of how to do this transformation.
This is one of the most beautiful ideas which they kept secret from you.
They should have taught it in high school.
But, they want to publish papers themselves and not tell you the general mechanism.

Let us assume we have elements of type `T` that need to be paired or combined in some way,
whether with `min`, `+`, `merge`, or any other assocative operation on `T`.
What we can do is create an array called a "counter".

       index: 0  1  ...   31
    contents: x1 x2  ...  x32

The `nth` slot of the counter will store the element that has had `n` "victories" so far.
So if there is a guy in slot 0 he hasn't played any games yet.
If there is a guy in slot 2 he has won 2 games, and so on.
This structure will help us to only pair up elements that have the same power.

The following example using `min` as the operation should make this clear.
Initially the counter has zero in every entry:

    initial counter

       index: 0 1  ...   31
    contents: 0 0  ...   0

Take a new guy `x` who has never played any games,
and look at the guy in the first slot of the counter.
The existing guy is either zero or not.
If it's zero, put the new guy `x` in the counter at index `0` (he has not played any games).

    0 1  ...   31           0 1  ...   31
    0 0  ...   0     -->    x 0  ...   0

Now take another guy `y`. Since `x` is in first slot of the counter, we must pair them up.
The winner moves on up to the next slot in the counter,
as they have now one a game.
So if `y` wins:

    0 1  ...   31           0 1  ...   31
    x 0  ...   0     -->    0 y  ...   0

Otherwise `x` wins:

    0 1  ...   31           0 1  ...   31
    x 0  ...   0     -->    0 x  ...   0

What if the index `1` slot was non-zero, after comparing `x` and `y`?
Then the guy there already won one game.
So, we must **carry propogate**[^adder-circuit].
Repeat the same process all the way up the counter, until we find a slot which is zero.

What if the counter is full, and has no zero slots?
That's called an an **overflow**.
There is a close analogy between our counter and binary integer counting:

    0 0 0
    1 0 0
    0 1 0
    1 1 0
    0 0 1
    1 0 1
    0 1 1
    1 1 1

But instead of 0 and 1 in each slot or "bit" we have arbitary elements that are combined with an associative operation.

### Handling overflow

What do we do if the counter overflows?
Whenever we don't know how to proceed,
do something sensible and let whomever uses it figure out what is a sensible thing to do.
Return the carry.
If the return is non-zero the programmer who called the counter will know
it overflowed and can decide what to do.
Maybe they will extend the counter or throw an error.
It's his business not ours.

Let us be lazy. 
The great success in programming comes because there are lazy people who say,
"I don't want to know now,
I'll find out later."
Right now we are solving this problem.
We have an associative binary operation of some kind
on type `T` and what we discovered that if we have associativity,
we can make this counter and it will work for us.

[^adder-circuit]: The terms **carry** and **overflow**
    are closely associated the implementation of binary counting
    or addition as an electrical circuit, called an [adder][adder].

    A single bit adder has two inputs `a` and `b` for
    the two digits to add together.
    It outputs a digit `s` which is the digit
    to display for this place value.
    There is a supplementary output called the carry,
    which is the amount that needs to move to the 
    higher place value.
    
    The circuit behavior is determined by the following input/output table:

        a  b   c  s
        ----------
        1  0 | 0  1
        0  1 | 0  1
        1  1 | 1  0

    This adds a single digit, but we want to add entire numbers.
    To, do so we add an additional input `l` for carried digits
    from lower place values.

        a  b  l    c  s
        ----------------
        1  0  0  | 0  1
        0  1  0  | 0  1
        1  1  0  | 1  0
        1  0  1  | 1  0
        0  1  1  | 1  0
        1  1  1  | 1  1

    Now we can chain `n` of them together to be able to add `n` digits

            s_1 c_1-----|      s_2 c_2----  ...
            |   |       |      |   |
         -----------    |   ------------
         |    |    |    |   |    |    |
        l_1  a_1  b_1   |_ l_2   a_2  b_2

    An **overflow** is when the last adder has a non-zero carry.

[^errors]: Floating point arithemetic can involve many tricky details,
    but the basic issue Alex is referring to is straightforward.
    We often use scientific notation to write larger numbers as a decimal to a power,
    when they would otherwise be very long to write out.
    For example `1,450,000,000 == 1.45 * 10^9`.
    However, adding two numbers in this form can only be done accurately if they are roughly of the same power,
    otherwise the larger of the two is still the best representation:
    `1.45 * 10^9 + 1 ~= 1.45 * 10^9`. 

    `float` and `double` have very similar limitations, and give the best results
    when operations are applied to values of similar magnitudes.
    To try for yourself, compare these two expressions in a program:

        double x = (152500.0 * 5000.0);
        double y = (152500.0 * 5000.0) + 0.01;

[adder]: https://en.wikipedia.org/wiki/Adder_(electronics)
[numerics]: https://en.wikipedia.org/wiki/Numerical_analysis
[merge-sort]: https://en.wikipedia.org/wiki/Merge_sort
[binomial]: https://en.wikipedia.org/wiki/Binomial_heap


### Implementation

Now we have to write the code.
The first function will add an element to the counter
using the process we just described.

    template <typename T, typename I, typename Op>
    // requires Op is BinaryOperation(T)
    // and Op is associative 
    // and I is ForwardIterator and ValueType(I) == T
    T add_to_counter(I first, I last, Op op, const T& zero, T carry) {
        // precondition: carry != zero
        while (first != last) {
            if (*first == zero) {
                *first = carry;
                return zero;
            }
            carry = op(*first, carry);
            *first = zero;
            ++first;
        }
        return carry;
    }

`op` is not commutative, so we need to be careful about the order
it is evaluated in.
Elements in the counter got there before, so they were to the left of the
element we are inserting.

Notice that zero is `const T&` reference because we don't plan to modify it,
but we do modify carry,
so it should be passed by value.

After we finish adding all our elements to the counter, they might not all be reduced to one element.
There may be several elements left sitting at various levels of the counter.
We need to do one more pass of the operation to combine them into the final result.

This second function does that. It applies the operation,
in the same manner to the elements left sitting in the counter.

    template <typename T, typename I, typename Op>
    // requires Op is BinaryOperation(T)
    // and Op is associative 
    // and I is ForwardIterator and ValueType(I) == T
    T reduce_counter(I first, I last, Op op, const T& zero) {
        while (first != last && *first == zero) {
            ++first;
        }
        if (first == last) return zero;

        T result = *first;
        while (++first != last) {
            if (*first != zero) {
                result = op(*first, result);
            }
        }
        return result;
    }

We have to be careful with zero.
Sometimes it will work with the operation so apply `op(x, zero)`
gives you `x`, but sometimes that won't happen.
So we can't really initialize to zero.

## Binary counter class

### Start with algorithms

We want to take
these two algorithms and combine them into an object.
We have two beautiful algorithms but they're stateless.
Everything is outside, and this is what we should always do,
no matter what we have been taught in software engineering classes by very wise
object-oriented professors.
You start with algorithms.
You don't start with objects.
Figure out what you're going to do first.
But you don't have to stop there. 
Because you can then put things together into an object.

It's very easy when you write an algorithm to
have a minimal iterator interface.
They externalize the counter.
We say, "we don't want to know about him,
we're just algorithms people".
We assume the principle that we will have no state for about
five minutes, and stay very functional, but then
turn around and deal with state.
We look at the whole thing.

### Counter storage

So what do we think is the state of this counter?
How should we store the counter?
Don't we have millions of elements to reduce?
The counter is size `log(n)` which will
never be greater than `64`.
So, it's actually a small fixed size.
We will store it in a `std::vector`.

    template <typename Op, typename T = typename Op::argument_type>
    class binary_counter
    {
    private:
      std::vector<T> counter;
      Op op;
      T zero;

    public:
      binary_counter(const Op& op, const T& zero) :
        op(op), zero(zero) {}
      
      void reserve(size_t n) { counter.reserve(n); }

      void add(T x) {
        x = add_to_counter(counter.begin(), counter.end(), op, zero, x);
        if (x != zero) counter.push_back(x);
      }

      // returns: value of the counter
      T reduce() {
        return reduce_counter(counter.begin(), counter.end(), op, zero);
      }
    };

Counter is private, we don't
want people to muck up our counter.
Same with our operation.

Using initializer lists instead of assignment in constructor is important.
If you initialize in the body, it will first call default constructors
for members, then you overwrite all the work with an assignment.

I think it is very beautiful.
We could compete with Steve Jobs for elegance of our design[^alex-apple-joke].

**Exercise:** If you are familiar with [numerical analysis][numerics],
    whenever you sum up large number you don't really want to
    add small quantities to big quantities.
    Bad things happen to the errors[^errors].
    Use this code to write a sum function for arrays of `double`. 

**Exercise:** Rewrite `min_element` using this code (just `min_element`, don't worry about second best).

**Exercise:**  If you want to implement [merge sort][merge-sort] you can use exactly the same device, since merge is associative. The idea with merge sort, is you only want to merge lists if they are roughly the same length and this helps you do it (see Chapter 12).
    Write the associtative binary operation `merge` which can combine two sorted arrays into a sorted array.

**Exercise:** When we become grownups we learn about advanced data structures, such as [binomial forest][binomial]. They use the same idea. Learn about this data structure and try to figure out where
    the counter could be used. 

[^alex-apple-joke]: Alex: Maybe we should make it in China.
    That's a necessary prerequisite for beautiful design.
    [Designed in Cupertino][designed-by-apple], assembled in China.
    So let's try to assemble our machine in Palo Alto.

[designed-by-apple]: https://signalvnoise.com/posts/2710-designed-by-apple-in-california

### What is in-place memory usage?

How significant is the storage of our counter?
We use the term [**in-place**][in-place] to indicate the memory
usage of an algorithm is not significant.
A long time ago people thought 
algorithms were in-place if they didn't require any extra memory.
Then they decided constant memory was enough.
But, then they said it doesn't quite work because our favorite algorithm
doesn't work. Of course the most important algorithm is [quicksort][quicksort].
It's not in-place because it's recursive.
Recursive algorithms tend to require `O(log(n))`.
Quicksort splits roughly in the middle let's assume on average.

So people say `log(n)` is good so that will count as in-place.
Then they said, "what if we have nested things?"
So is `log(n)^2` ok?
In our universe `log(n) <= 64` so this is 4096.
Then theoreticians said it's really alright if we have "poly-logarithmic" storage
Basically as long as the memory requirement is `O(p(log(n)))`
where `p` is a polynomial, it's alright.
Polynomials get a lot bigger than square,
but let's go with "poly-logarithmic" being "in-place".

[quicksort]: https://en.wikipedia.org/wiki/Quicksort
[in-place]: https://en.wikipedia.org/wiki/In-place_algorithm


## Code

- [binary_counter.h](code/binary_counter.h)



