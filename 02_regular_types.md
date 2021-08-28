2. Regular types and other concepts
===================================

## We don't know how to program yet

The field grew very fast.
We never had time to stop and think.
It used to be that people who were computer scientists were programmers.
When [Dijkstra][dijkstra][^about-dijkstra] gave his [Turing award speech][dijkstra-turing]
he called himself a "humble programmer".
When Ken Thompson, in his
lecture "Reflection on Trusting Trust", talks of himself he says, "I am a programmer".

Then programmers disappeared.
Everybody became a computer scientist or an architect.
People say, "no I'm not a programmer, I'm an architect".
I guess they build buildings like [Frank Lloyd Wright][frank] (joke).
What it means is they don't have any idea how to write code.
(Clearly, I just insulted several communities).

A couple of years ago I was at a party with
[Mike Burrows][burrows] who did the original [AltaVista][alta-vista] search engine and
is now at Google.
He told me, he cannot imagine how the guys at Google write anything at all. 
So, I'm not even going to mention Yahoo (joke).
I did interview at Facebook and let me tell you what I saw of that place... 
That was five years ago.
Maybe they improved but I don't think so.
Places tend to to decline.

You might say: "Who are you to tell us?"
I am as guilty as any of you. 
Given the chance, I write very bad code.
One thing which prevented me from being like
everybody else was that some of my code had to go through thousands of people looking at it and telling me what they think about it.
That is very helpful.

From early on I actually had this idea that
programming is a very great discipline.
I don't mean that I'm very great, or anything like that. 
It's that the discipline is a great discipline.
I always wanted to learn to write ultimate code and I still think it's possible.
I disagree with many of
the luminaries in the field who claim it's an art, meaning that there are some
gifted people like Mozart,
and others who are just like us.
I actually claim no, it's a discipline.

This is why I want to discuss everything I write with you.
If we together agree on it and we start practicing it maybe it will turn into a
discipline.
You say, "well, Alex what chance is there?
There is the world and there is you".
Yes, there are no chances but you still have to do
what you believe is right.

Well, plus they pay me (joke).

[^about-dijkstra]: Dijkstra is also a fantastic programmer to learn from.
    Instead of focusing on academic publishing the majority of his work 
    was typewritten or handwritten and passed around by mailing list.
    Consequently, it is very readable, and applicable, free from jargon
    or obscure technicalities.
    You can find an archive of these [here][ewd].

[alta-vista]: https://en.wikipedia.org/wiki/AltaVista
[dijkstra]: https://en.wikipedia.org/wiki/Edsger_W._Dijkstra
[dijkstra-turing]: https://www.cs.utexas.edu/users/EWD/ewd03xx/EWD340.PDF
[frank]: https://en.wikipedia.org/wiki/Frank_Lloyd_Wright
[burrows]: https://en.wikipedia.org/wiki/Michael_Burrows
[ewd]: https://www.cs.utexas.edu/users/EWD/

## The motivation for concepts

Remember what I said about these awful "maps and maps of sets" and things like that?
The fact that they actually work is kind of amazing[^complete].
It's a testament to how flexible they are.
You might think you can just put any type in a map,
but actually there are requirements on the type,
certain properties which are required for these containers to function.

Let, us think about what my task was when I started working on the C++ STL.
It was to define standard data structures which will work for any reasonable subset of types.
What is a reasonable subset of types?
This is a little bit tricky but it's of paramount importance.

My duty was to discover what will make
a type work with any standard container.
I knew that for sure, no matter what I do, there is one type which should work in every container: `int`.
So, a regular type has to be like `int`, but not completely like it because it should work for `double`. 
Pointers are perhaps the most important type,
even more important than `int`.
It has to work for pointer. 

To understand all this, we're going to become a little bit theoretical.
None of this stuff actually works  unless you understand at least a
little bit of theory.
We will call such reasonable types `Regular`.
What we will do is formally define a set of operators that all `Regular` types must have,
along with a set of requirements on those operators[^equational-reasoning].
The goal is that `Regular` types (those that obey the requirements)
will behave sufficiently like `int`, `double`, and the rest.
Such a definition is referred to as a **concept**, which we will talk about more later.

Once we understand `Regular` then we will understand what algorithms are allowed to do;
use the operations which are defined on `Regular` types. 
Whatever is a natural idiomatic expression in C should be a natural idiomatic expression for `Regular` types.

### Closure property for containers

Furthermore, I realized that I have to make my own containers be like built-in types.
I had to close the loop.
I had to provide a bunch of type constructors.
A `vector` is a type constructor.
It takes the type `T` and constructs a type out of
type `T`. 
Out of type `T` you get type `vector<T>`.
It's a different type.

We want these constructors to be closed[^closed].
If you start with a regular type and keep applying
these constructors you remain `Regular`.

[^closed]: Closed is a term from math that has various but similar meanings
    depending on context. In the context of abstract algebra, from which generic programming
    is inspired, closure means that applying an operation to an element in a domain `D` gives
    you something back in the same domain.
    For example the integers `Z` are closed under the operation of multiplication.
    Multiply two integers, you get an integer.
    However, it is not closed under division.
    `1 / 2` is not an integer.
    Applying this to Alex's quote, he means that containers should preserve properties
    of the base types they are constructed from.
    Containers of `Regular` types should be `Regular`.

[^equational-reasoning]: Alex: Some might think regular types are a form of equational reasoning.
    However, you can have a universe that is strictly functional where you have equational reasoning.
    Our universe has assignment and state.

[^complete]: Alex: They're not [Turing complete][turing-complete], but they're Stepanov complete (joke).

[turing-complete]: https://en.wikipedia.org/wiki/Turing_completeness

## Semiregular types

Leading up to `Regular` we will define `Semiregular` which is a bit weaker.
All `Semiregular` types must have the following operations:

### Copy constructor

We should be able to write:

    T a(b); 

Or equivalently

    T a = b;

They are not the same in general but they are the same if `b` is of type `T`.

How do we define the semantics of this operation?
Could we use an [equivalence relation][equivalence-relation]?
Consider the relation that always returns true.
It is an equivalence relation.
In other words, the relation R(a, b) = t, satisfies:

- symmetric: `R(a, b) <=> R(b, a)`
- reflexive: `R(a, a)`
- transitive `R(a, b) and R(b, c) => R(a, c)`

But, this is a wrong equivalence relation.
We actually want something way stronger.
We want *equality*.
Given a notion of equality, we can define
some axioms for our copy constructor.

**Axiom:** After `a` is copy constructed from `b`, we have `a == b`.
Whatever our meaning of equality.

Let's think about what a copy is.
It is something which is *equal to the original, but
not identical to it*. 

**Axiom:** After `a` is copy constructed from `b` they
have distinct identity markers.
In C++ the identity marker is usually address: `&a != &n`.

All copy constructors must behave this way.
If somebody clever comes and says, "oh we're going
to have a semantics where we're going to have this shared thing"[^smart-pointers].
Will it work? No.
Copy has to construct a different thing.

### Assignment operator

    T a; a = b;

**Axiom:** Constructing and assignment are equivalent: `T a(b) <=> T a; a = b`.

So, in order to for these operations to have correct semantics, they have to have equality defined.

### Destructor

    ~T();

You don't call destructors, so nothing specifically to say here.

[equivalence-relation]: https://mathworld.wolfram.com/EquivalenceRelation.html
[cpp-shared-ptr]: https://en.cppreference.com/w/cpp/memory/shared_ptr

## Regular types

The concept `Regular`  extends `Semiregular` with equality operators
which are `==` and `!=`.

### Equality operator

As we said before, we should define `==` so that after constructing
a copy, the original and the copy are equal.

`!=` should always behave like: `!(a == b)`.
My very strong point is that the semantics of
inequality (`!=`) is absolutely and totally bound to the semantics of equality (`==`).
You should not even be able to have a situation where they have different  semantics.
But, the standards committee disagrees with me on that.
They say that you could have equality be equality and
inequality be multiplication operator.
I think it's a very bad idea and a good idea depending on what you want (joking).
It will provide you with job security. 
Because nobody will ever figure out why your code works or doesn't work.

Many of these operations will be member functions. But what about equality?
No, it shouldn't.
Because fundamentally equal is a symmetric function.
It compares two things.
So even the paradigm of a member is the wrong paradigm.
They are symmetric.

[^smart-pointers]: This is likely a reference to how [`std::shared_ptr`][cpp-shared-ptr] behaves
    which provides a form of automatic memory management.,

## Total orderings

The concept `TotallyOrdered` extends `Regular` by adding a comparison operator `<`.
This is actually something that Bjarne[^about-bjarne] disagrees with me on.
I say that `<`, the total ordering of the type is
fundamental, and should be required.
He says it's too much to ask, and `Regular` types should be less strict.
However we will follow his and the standard C++ convention by 
using a weaker `Regular` and limiting the requirement of ordering
to `TotallyOrdered`.


### Less than operator

`<` must obey the following [mathematical properties][total-order]:

**Axiom 1:** Anti-reflexive: `!(a < a)`

**Axiom 2:** Transitive: If `a < b` and `b < c` then `a < c`.

**Axiom 3:** Anti-symmetric: If `a < b` then `!(b < a)`.

**Axiom 4:** If `a != b` then `a < b` or ` b > a`.

In the same way that the semantics of `==` is related to `!=`,
we can see the semantics of `<` must be totally bound to the semantics of equality.
But furthermore,  `<` is related to four other operations which must also be defined:

1. `<` less than
2. `>` greater than
3. `<=` less than or equal to
4. `>=` greater than or equal to

If you provide them then they have to have natural meaning.
For example, `!(a < b)`  should be `a >= b` otherwise the world
perishes.

Later we will talk more about orderings, and define several different
kinds.

In the next lesson we will become familiar with these concepts
by implementing a class which satisfies each of them. 

[total-order]: https://en.wikipedia.org/wiki/Total_order 
[bjarne]: https://www.stroustrup.com/
[^about-bjarne]: [Bjarne Stroustrup][bjarne] is the creator of C++, author of many C++ books,
    and always has been an active member of the community.
