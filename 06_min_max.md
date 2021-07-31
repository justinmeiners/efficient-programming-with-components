6. Ordering, min, and max.
=============================

## Learning to design code

Sometime it feels like I should just tell you what's in STL.
But, I don't want to do that.
I want to teach
you to think so that you could design something equal or better.
So most of the algorithms we are looking at are in STL,
but they are not exposed.
They're beyond what the standard Committee would ever consider.
You might say, "Alex this is a simple problem. Couldn't you show us how
to build a search engine?"
Not in a class.


## Reviewing Total Orderings

Programmers need to know mathematics.
They have to know certain fundamental things.
When we talked about `Regular` and `TotallyOrdered` types we defined the
`<` operator 
and some axioms it must satisfy.

**Axiom 1:** Anti-reflexive (also called **strict**): `!(a < a)` 

This property is what distinguishes `<` from `<=`.

Recall that from `<` and `==` we defined all other comparison operators
`>`, `<=`, `>=`, `!=`.
Why did I pick `<` (the **strict** operator) to be primary in the STL,
instead of `<=`?
It's important to note that this was a choice I made.
They give you roughly the same universe of things, so you could design
it around either.
But, somewhow I felt `<` is a more fundamental relation.
`<` requires a little less typing.
There are other reasons, too.

**Axiom 2:** Transitive: If `a < b` and `b < c` then `a < c`.

Why is it important?
Think about sorting.
If I checked that `b` wins over `a`.
Then I check that `c` wins over `b`, I don't have to do the third check.
It's guaranteed.

**Axiom 3:** Anti-symmetric: If `a < b` then `!(b < a)`.

**Axiom 4:** If `a != b` then `a < b` or ` b > a`.

This is also called the trichomoty law,
because for all elements, exactly one of three things must be true[^eop-ordering]:

    (a < b) or (b < a) or (a = b)

There is a fundamental connection between `<` and `==`.
If `!(b < a)` then it must be the case that `b >= a`.


## Weak orderings

Our goal is to write the `min` component.
We could write it for `TotallyOrdered` types,
but there are other types and orderings it will work on as well.
Specifically, `TotallyOrdered` is a bit too strong, and not required.
"Partial ordering" may come to mind, but that's tricky 
to work with.
A partial ordering is one that cannot always be used to compare elements.
It is not defined for all elements in the domain[^set-compare].
We won't talk too much about them.

Consider a `struct` with several fields, and we want to order based on them.
For example, ordering employee records by last name.
If you're in China there will be many people in the same buckets
because the set of last names is small
and the number of people is very large.
There will be people which are
not less, nor equal, nor greater but they're not equal.
They are *equivalent but not equal*.

The concept we need is called `StrictWeakOrdering`.
A `StrictWeakOrdering` is exactly the same as total ordering but in a total ordering,
trichotomy says it's greater or less or equal.
In case of weak ordering it says it's greater, or less, or *equivalent*.
There is some equivalence relation, such as last name.


Some people would say, well equal is a kind of equivalence,
so  let's define `<` to just be weak ordering.
I say it's evil.
Why?
Because for `TotallyOrdered` we need to be able to know

    !(a < b)  <=> (a >= b)

The `==` is equality, not another equivalence relation.
We can't conclude that with a weak ordering.
We must overloaded symbols for what they commonly mean.

[^eop-ordering]: Alex recommends chapter 4 of "Elements of Programming"
    to learn more about this subject.

[^set-compare]: The classic example of a partial ordering
    is the relation subset on the domain of sets.
    The set `{ 1 }` is a subset of  `{ 1, 2, 3 }`
    But, `{ 1, 4 }` is not a subset of `{ 1, 2, 3 }`.
    Neither is `{ 1, 2, 3 }` a subset of `{ 1, 4 }`.
    The two elements are incomparable.

## Min

What we will do is write an incorrect version
and then fix it.
We will start with `TotallyOrdered` because
we are more familiar:

    template <typename T>
    // T is TotallyOrdered
    inline
    const T& min(const T& a, const T& b) {
      if (a < b) {
        return a;
      } else {
        return b; 
      }
    }

Notice that we need to pass things by `const` reference.
We want to be able to write `min(5, 3)`,
and 5 and 3 are literals which are constant.

### When elements are equal

What should we return when `a == b`?
It seems it doesn't matter.
But, that's the problem.
Everywhere in programming you do something and it seems to be correct.
But, you have to think deeply and then you discover a problem.
There is nothing little in programming.

Let me construct a proof for you.
When we sort a range which is already sorted,
we should not do any operations.
Nothing should be swapped.
It's a good requirement.

Another requirement is if I sort two things,
The first guy should be the `min` afterwards
and the second guy should be the `max`.
We agreed that default ordering for sorting should be ascending.
Zero, one, two, three is natural. 
Three, two, one, zero is when you launch missiles.
So what we can conclude is that `min` on two equal arguments
should return the first. We will see more of this later.

So let's correct it, so we don't swap unless necessary.

    if (b < a) {
      return b;
    } else {
      return a; 
    }

When you design a function you often think about just this
function, and ignore how it interacts with other parts of your API.
Then you discover inconsistencies which can be very subtle but painful.
There are several profound mistakes in STL
They are still in the standard, despite all my attempts to change them.
It's very easy to make a mistake and it's really hard to fix it.

### Correct implementation

Now we have a correct version for `TotallyOrdered`, let's generalize it
 for `StrictWeakOrdering`.
We no longer can rely on the `<` operator on the type, as there may be many
orderings and equivalence relations on a type.

    template <typename T, typename Compare>
    // Compare is StrictWeakOrdering on type T
    inline
    const T& min(const T& a, const T& b, Compare cmp) {
      if (cmp(b, a)) {
        return a;
      } else {
        return b; 
      }
    }

Why do we pass compare as a generic type argument instead of a function pointer?
There are two reasons:

1. **Functionality:** Making it a type would allow it to have state. Pointers to functions have no state.

2. **Performance:** If we were passing a pointer it would have to do a function call through the pointer.
   The function call has to save and restore registers.
   It's slow especially if it sits inside a loop and is called a gazallion times.

## Less than function object

It's somewhat inconvenient to pass `cmp` when you actually want to use a `TotallyOrdered` type.
Therefore there should be a version of `min` which doesn't take that parameter.
The wrong way is to use a default template argument.
Sometimes you want to get a pointer to function the function `min`
with a comparison function inserted.
For example:

    min<int, std::less<int>>

We also want to be able to say:
    
    min<int>

So we write a second interface:

    const T& min(const T& a, const T& b) {
        return min(a, b, std::less<T>());
    }


Let's implement a standard class called [`std::less`][cpp-less].
It overrides the evaluation operator so it can be called just like a function[^function-objects].

    template<typename T>
    // T is TotallyOrdered
    struct less {
        bool operator()(const T& a, const T& b) const {
            return a < b;
        }
    };

Function objects are a very important technique to allow you to pass things to template algorithms.
It will literally be inlined at compile time.
It is very good to be inlined like this.
Instead of a function call it will be one instruction,
which is most likely going to be done in parallel with some other instructions.
It is free.

Here is an example of how to use it:

    int a[] = { 4, 9, 3, 1, 4 };
    std::sort(a, a + 5, std::less<int>());

Notice that we aren't passing a type, we are actually creating an object[^object-size].
You can write your own function objects.
If you have a record and you want to do sorting, or min, or max on the second field you could
write one which compares the second field of the record.
It will be inlined.
Remember the faster computers get, the slower function calls are.


[^object-size]: Alex:
    How big is this object?
    You might say it's empty.
    But, sadly enough, there is a great secret. 
    There are no empty things.
    They are still one byte because C doesn't want us to allocate two objects at the same
    address.
    If you have zero size you will be able to just pile objects on top
    of each other so you do use one byte.
    Does it matter? 
    Not at all.

[^function-objects]: Function objects like this in C++ fulfill
    the same role as [closures or lambdas][sicp-env].
    They capture variables or state, and then use them to evaluate the function.
    The main difference is that their saved context is explicit rather
    than implicit.

[cpp-less]: https://en.cppreference.com/w/cpp/utility/functional/less

[sicp-env]: https://mitpress.mit.edu/sites/default/files/sicp/full-text/book/book-Z-H-21.html#%_sec_3.2

## Max

Let us try to do `max`.
It is a hard one.
I got it wrong in STL and only realized after it was accepted as an international standard.
I started this campaign on changing it to the correct one.
The campaign has been going on close to twenty years without any success.
So, the one in the standard is still broken.
Let us see why.

It seems that `max` is just `min` with `>`.
So wh do we need it?
We still want to provide what is convenient for the customer.
When they think `max` and go looking for it, it should somehow work.
But, its a little bit more.

### Sort2

We previously said that `min`, `max`, and `sort` should work together naturally.  
To see how they should all work, let's write `sort2`, which sorts two things.

    template<typename T, typename Compare>
    // requires Compare is a StrictWeakOrdering on T
    inline
    void sort2(T& a, T& b, Compare cmp) {
      if (cmp(b, a)) {
        swap(a, b);
      }
    }

It's always preferrable to sort in-place because we can obtain a composable
one by first copying, and then applying the in-place algorithm.

Note once again the order of comparison.
We have to be careful that aren't going to swap when they are equal.
I want the following invariant, after `sort2`
`a` contains min and `b` contains max.
It's very natural.
But, we have to ensure it works, even when the two parameters
are equivalent.

### Implementation

So, if we define `max` with `min` and `<`
it's not going to work.
Suppose we have `a == b` (they are both equivalent), then

    min(a, b) = a
    max(a, b) = a

You actually want `min` and `max`
to do different things because they're both useful.
When there are two equivalent things, `min` will return the first,
and `max` will do the opposite and return the last.

    template<typename T, typename Compare>
    // requires Compare is a StrictWeakOrdering on T
    inline
    const T& max(const T& a, const T& b, Compare cmp) {
      if (cmp(b, a)) {
        return a;
      } else {
        return b;
      }
    }

The naive user will never even know about what happens to equal.
This is for sophisticated guys like you who write complicated and relevant algorithms.
You need to understand the finer things of programming.

Every time I talk to one member of the standard
committee he always says, "Alex you are too theoretical".
I guess I am because I pay attention to
these little details.
But I claim to be able to to show you that
things like that will matter. 

Later we will generalize it on a bunch of things.
Sometimes I want to find the first min, sometimes I want
to find the last min, and the same for max.

**Exercise**: Define a version of `max` that uses `TotalOrdering` instead of `Compare`, like we did for `min`.

## Fundamental logical laws are not always obeyed

We have to address an issue which will cause lots of trouble.
The people at Google encountered it and they couldn't figure out.
Certain thing should always work, such as the following:

    a = a
    
    a < b or b >= a

We assume these statements are true, 
but it's not true.
There are exceptions whic cause enourmous amounts of harm
and break all the laws of equality and ordering.

    double x(0.0/0.0);
    std::cout << (x == x ? "yes" : "no") << std::endl;

This should always print "yes", but it prints, "no".

You could say, "Only total idiots will write code like that".
The problem is much harsher problem.
You could have a floating-point computation which leads to this result 
and is utterly invisible to you.
The problem actually appears when people do complicated things.
Perhaps they do millions of computations and then sort them.
Sort assumes that equality and inequality work like 
they should  and bad things happen.

The [IEEE floating point standard][float] is one of the great accomplishments of
computer science. One of the top five.
From my point of view, we haven't got that many accomplished.
We might have six altogether.
It was terrible before it.
But somehow they decided to
indicate undetermined floating-point value by breaking fundamental logical axioms.

They are also not `!=` which is weird too:

    double x(0.0/0.0);
    std::cout << (x < x || x >= x ? "yes" : "no") << std::endl;

Prints "no".
Law of excluded middle doesn't work anymore,
and this is sad.
You can't make reasonable assumptions.

What's the solution?
There are two solutions

1. abandoning the laws.
2. putting a wall around bad values.

I'm advocating the second one.
We keep the laws and define singularities.
If there are singular values, the universe collapses,
you know  nothing applies.
you have to assure that singular values do not appear in your computation.

[float]: https://en.wikipedia.org/wiki/IEEE_754

## Final code

- [minmax.h](code/minmax.h)



