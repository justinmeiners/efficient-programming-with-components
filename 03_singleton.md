3. Singleton: a pattern for regular types
============================================

## Learning C++ isn't as hard as it appears

I talked to someone and he said "I have been programming in C++ for about eight years, but I never learned C++".
 That is a very common thing.
He was just honestly admitting something which happens to be the case for most people.
Most of us never learn programming languages.
I never learned C++ I was just very
fortunate that I worked next door to the guy who designed C++ so I could 
ask questions about why it was doing things. 
We learn by osmosis. 
We ask each other questions. 
I will assume for a second that my friend was right and I will generalize from him to the rest of the class.
Let us figure out together some basic things about C++.

Of course C++ looks very scary if you look at the language description.
It's about 2,000 pages long and written in incomprehensible legalese.
You stand no chance to ever understand this, it's just way too long.
I claim that you could write wonderful C++ if you learn about five pages of basic code, not 2,000,
but five and anybody can.

My goal is to make everybody a C++ expert, not to prove that experts are experts.


## Singleton: a pattern for regular types

Today I wanted to start with one such page which would show us how to write any class.
This class is called `singleton` and possesses two properties:

1. It is the most simple class possible, meaning it will have no code whatsoever.
2. It is the most complete class possible, meaning it will have all the language details about class writing which you need to know.

After this class is written, you will trim it and put it on your wall,
because any other class will have all these things and more.
When you make any other class, you can start by taking this class and replace
the name `singleton` and then you just need to fix two or three things.

### Not the object oriented pattern

Some of you might be confused by the name singleton.
All the people
read the "Gang of Four" book[^about-gof] and are familiar with singleton pattern[^singleton] and 
might think that's what I'm talking about.
No, I wouldn't do that, would I? 
What I'm talking about is not a pattern, but a very simple class. 
It's similar to a class called [`std::pair`][cpp-pair].
If you go and check a dictionary it says: singleton, pair, triple, quadruple, etc. 
A pair has two things, well singleton has just one thing.

[^about-gof]: "Gang of four" is a nickname for an influential text in object oriented programming called ["Design Patterns"][gof]
    by Gamma, Helm, Johnson, and Vlissides. 
    As the name suggests, the book studies patterns of ways to organize code and objects to solve common problems
    while aiming to preserve modularity and flexibility.

[^singleton]: A [singleton](https://www.oodesign.com/singleton-pattern.html) in object oriented programming is a class which is intended to have one instance,
    or at least one canonical instance, which all code can reference.

[gof]: https://en.wikipedia.org/wiki/Design_Patterns
[cpp-pair]: https://en.cppreference.com/w/cpp/utility/pair

### Template and type functions

Let's start with:

    template <typename T>

Some people get scared when they hear `template`.
Why do I need template? Because we want to write something which takes one type and returns
another type.
Let's forget C++ for a second.
Mathematically speaking we want to write a type function: a thing which takes a type in
and  generates new types.
The `template` is the mechanism in C++ for doing just that.

There are of course other type functions, even type functions in C. 
For example, we want to take a type we want to take an `int` and convert it to some other type.
Can you think of one?
What about [`sizeof`][sizeof]?
No, this does not return a type.
But it is a wonderful example of a **type attribute**,
something which maps type into value.
What about macro?
Macro for sure has now no effects on type system[^macros].
What about pointer?
Yes. The pointer star is a type function.

    int* 

It takes type int, double, etc and returns another type.
You write it postfix but it is logically a type function.


[^macros]: Macro systems typically manipulate only the text of the code itself, and thus have no
    access to compiler data.
    Expanding all the macros to create new source code,
    and then compiling the resulting macro-less code
    would give equivalent output.

[sizeof]: https://en.cppreference.com/w/cpp/language/sizeof

### Guidelines for writing classes

Now let's start our class, with a field for value:

    template <typename T>
    struct singleton
    {
        T value;
    };

What is the difference between a class and a struct?
The only difference is how they treat public and private.
They're fundamentally the same.
If I say `struct` the default field accessibility is public. If I say `class` the default is private.

Some would say, "let us make value private because 
that's object-oriented way".
"We will provide people with two functions: set value, and get value."
That is good because it makes your productivity go up.
You could report that you wrote two more lines of code (joke).
On the other hand, it is idiotic because the only reason you want that is to read and write in a peculiar way,
not the way the language intends, which is just using the assignment operator.
It doesn't protect anything[^communist].

That does bring us to another question. 
What do we need to make private?
When you should not be able to set a value.
A canonical example is something like `std::vector`.
Internally, it has a pointer to a memory area where all the data is allocated.
You don't want to have this thing public because if somebody writes into there,
you're going to be leaking memory.
There is no reason to write to it before you first deallocate.
Another way to think about it is to help preserve an invariant.
There is some condition we need to assure that the values cannot change.

For `singleton`, we pick struct because it's less typing.
*We should find the most minimal way of saying what we want to say*.

[^communist]: Alex: Don't be so eager with making members of classes private.
    I'm a communist, I don't like private property.
    OK, I'm not really a communist.
    Someone accused me of being one for not liking object-oriented programming.
    They said STL was basically communist propaganda.
    Objects are important because they own things.

    This same person made a million dollars selling my code.
    I'm not making it up.
    I worked for [HP labs][hp-labs].
    But, HP couldn't use code from research,
    because researchers don't know anything.
    So, they bought it from this guy.
    The only thing he did is run it through a pretty printer
    which made it unreadable.
    I didn't get a kickback.
    My income from STL is about $750.
    I got $250 for writing an [article][byte] for BYTE magazine.
    I got $500 for an invited keynote at [oopsla][oopsla].
    I paid taxes from both.

[oopsla]: https://en.wikipedia.org/wiki/OOPSLA
[byte]: http://stepanovpapers.com/BYTE_com.htm
[hp-labs]: https://en.wikipedia.org/wiki/HP_Labs

## Semi-regular singleton 

When we implement this class, we want people to feel as good about it as they feel about `int`.
It has to be natural, so all the functions they would normally write for `int` would just work for this class.
Not of course plus, minus, divide, etc but the basic fundamental stuff should work the same.
Even people who don't understand logic behind STL still
 manage to use `std::vector` because
vector behaves in somewhat natural way.

For example, we want to be able to support conditional initialization:

    T a;
    if (something) a = b;
    else a = c;

To support this we need:

    T a;
    a = b;

to be identical (hopefully even in performance), to:

    T a(b);

Now, let's provide the operations to make it semi-regular.
We will start with constructors:

    // semiregular
    singleton(const singleton& x) : value(x.value) {}
    // could be implicitly declared
    singleton()  {}
    ~singleton() {}

    singleton& operator=(const singleton& x) {
      value = x.value;
      return *this;
    }

What are the semantics of the default constructor?
In this case you want whatever the default value of `T` is, to be constructed.
The compiler will do this for us.

What will the default value of `T` be when it's a pointer type?
Just some random address, which is fine.
The two properties of default constructed value of `T` on which you
can rely are:

1. You can assign to it.
2. You can destroy it.

A pointer to a random address satisfies these requirements.

The default constructor will always be synthesized by the compiler unless you have another constructor.
It turns out, you usually want multiple constructors.
Therefore it's very unsafe to depend on.
You put additional constructor, and bam whole bunch of stuff stops compiling, very unpleasant.

### Optimize the common case

In the assignment operator do we need to do anything special when assigning a value to itself (`x = x`)? 
It's a thing [Scott Meyers][meyers] says you should check for with `if (x == x)`[^scott-assignment-check],
but you don't need to check, just consider what will happen if they are equal.
It will just assign back to itself.
It might do extra work.
But, you don't want to optimize the case which is once in a blue moon while slowing every other case.
Believe it or not when we do timing experiments we will learn conditionals are very very expensive and getting to be more and more expensive.
Assignments are getting to be free.
The processor could schedule them in parallel.
There is just enough hardware to do that so don't break the pipeline.

Compiler writers don't read Scott Meyers, fortunately for us.
You have to learn to think because he doesn't. 
He just gives you this rule, number 173.
If you follow the rule the code will be good.
No.
You have to think.
Then the code will be good.


[^scott-assignment-check]: See item 17 in "Effective C++".


### Virtual functions, virtual destructors, and OOP

Add a destructor:

    ~singleton() {}

If you interview for a job and there is a manager who knows
nothing at all about programming or C++, what is the one question he asks you?
"Why should you always make a destructor virtual?"[^virtual-destructor]
This is frightening thing for me because I lived my life
until now, without ever making a single destructor virtual.
Believe it or not most of my destructors are part of the standard library.

All the great authorities on C++ such as [Scott Meyers][meyers] who
people think invented C++ are idealized in books like "Effective C++",
"Effective STL", "effective everything".
He tells you always declare destructor as virtual[^scott-virtual].
OK, he's wrong. Simple as that.

What we do we want to create? Take type `T`, put it in a struct. Will the size of the thing increase?
No. It has no overhead. 
Singleton is the same size as `T`.
It's the wonderful thing about singleton, and lets you pack them in arrays.
If you add `virtual` to the destructor, the size will dramatically grow.
Why would I put virtual? Because some really crazy person would want to
inherit something virtually from this? He needs counseling.
Not a single class in the standard library does it. 

You have all heard that C++ is a multi-paradigm language.
What it means is once upon a time it was an object-oriented language.
Then some people chased it away and said you could program differently.
I am showing you how you could program differently.
If you program in an object-oriented way then many good things might happen.
I don't know what they are but you are not going to be efficient.
Bjarne used to joke that object
oriented systems are those that slow graphics.

As time progresses, `++` is getting faster and faster and virtual function is getting slower and slower.
Their spread is growing  and we're not going to address
any parts of C++ in this course which slow the computations.

If you want to learn about virtual I'm sure there are lots
of people here who would clearly advocate object-oriented programming,
but I'm not one of them.
I destroyed my career by becoming this sworn enemy of object-oriented a long time ago.
I am yet to see a situation where it's helpful.
I'm not denying that it's theoretically possible to find a good piece of code written with virtual functions. I am just stating the fact I haven't seen one yet.

[meyers]: https://en.wikipedia.org/wiki/Scott_Meyers
[^virtual-destructor]: Making a destructor virtual ensures that if a class
    inherits virtually and is destructed, then the base class destructor is called too.
[^scott-virtual]: In "Effective C++", Scott Meyers does not say to use `virtual`
    on every destructor, he specifies "make sure base classes have virtual destructors"
    and clarifies "gratuitously declaring all destructors virtual is just as wrong
    as never declaring them virtual".
    He provides a heuristic, "declare a virtual destructor in a class
    if and only if that class contains at least one virtual function."

    Alex is wrong about Scott's recommendation.
    But for whatever reason, this incorrect recommendation that
    we should always use `virtual` destructors was repeated
    and enforced in code linters at many organizations for a long time.
    I have personally had the question come up in several C++ interviews.
    It's likely that defenders of this practice incorrectly invoked
    Scott's name.

## Regular singleton

For regular types we must define equality.

    // regular
    friend
    bool operator==(const singleton& x, const singleton& y) {
      return x.value == y.value;
    }

    friend
    bool operator!=(const singleton& x, const singleton& y) {
      return !(x == y);
    }

Recall that we decided not to define these as member functions, because they are symmetric.
`friend` functions inside the class declaration are not member functions,
but still have all the access to all the members (not that we care here).
More importantly this signature is nice.
If you put it outside you discover you have to write an ugly thing.

### Equality and the three laws of thought

We have to define what equality is 
and it has to satisfy some very basic rules.
There is one great law on which the entire edifice of civilization
is based. It is called the **law of identity**
and goes with two other fundamental laws of thought [^laws-of-thought]:

**The law of identity**: a == a. Popeye the Sailor used to say, "I am, what I am".

Computers do not obey such law. 
There is a fundamental case which breaks and has consequences.

**Exercise:** If you don't believe me, try to figure out a type which violates the law of identity (solved in chapter 6). 

**The law of non-contradiction**: You cannot have a predicate `P` be true and `!P` be true at the same time.

**The law of excluded middle**: Every predicate `P` must be either true, or false.

Everybody from Plato all the way up to [Ayn Rand][rand], the greatest American mind (joke).
Whatever her political views, even she, [supported][rand-identity] the law of identity.
There is a lot to figure out.
There is deep stuff about equality[^constructivism]. 

[^laws-of-thought]: The "laws of thought" referenced here by Alex
    are generally attributed to Aristotle.
    Here is an expression of the Law of non-contradiction
    in the [Metaphysics][metaphysics]:

    > We have now posited that it is impossible for anything at the same time to be and not to be, and by this means have shown that this is the most indisputable of all principles.
    > Some indeed demand that even this shall be demonstrated, but this they do through want of education, for not to know of what things one should demand demonstration, and of what one should not, argues want of education.
    > For it is impossible that there should be demonstration of absolutely everything (there would be an infinite regress, so that there would still be no demonstration);
    > but if there are things of which one should not demand demonstration, these persons could not say what principle they maintain to be more self-evident than the present one. (Book 4)

[^constructivism]: There is a branch of logic
    called [intuitionism][intuitionism],
    closely associated with [constructivism][constructivism] which 
    denies the Law of excluded middle.
    Specifically, it takes issue with the idea that `!(!P)`, is the same as `P`.
    Modern [philosophers][dialetheism] have even questioned the law of non-contradiction.
    But, I know of no serious mathematical or philosophical 
    movements denying the law of identity.

[rand]: https://en.wikipedia.org/wiki/Ayn_Rand
[metaphysics]: http://classics.mit.edu/Aristotle/metaphysics.4.iv.html
[constructivism]: https://en.wikipedia.org/wiki/Constructivism_(philosophy_of_mathematics)
[dialetheism]: https://plato.stanford.edu/entries/dialetheism/
[rand-identity]: https://plato.stanford.edu/entries/ayn-rand/supplement.html#ExisIdenCons
[intuitionism]: https://en.wikipedia.org/wiki/Intuitionistic_logic

### Why can't the compiler generate == and !=?

Equality should be implicitly declared by the compiler, but it isn't.
It's not in C++11 or C++14.
This goes back to problems C++ inherited from C.
C is a very great language in some respects but
it also was a homegrown thing without some particularly deep thinking.
Initially they didn't have structs.
Then they had structs, but they couldn't pass them.
They were not copyable and you couldn't pass them as arguments.
Then they fixed it. 
They said, "we'll just copy the bits" and things would work.


Equality is defined for all built-in types; `int`, `double`, `short`, `char`, pointers, etc.
So, if you have a struct, why can't you generate an equality that uses those?
Two things are equal if all their members are equal.
It's a sensible rule a compiler could do. 

I never managed to ask Dennis Ritchie about it.
But, I asked probably the second best person on the subject, [Steve Johnson][johnson], because
he actually implemented all these assignments for structures and things like that.
Steve told me,  "it was very hard because the bits in the padding might not be equal.
If you compare bit by bit things which have equal members, but not equal bits in the padding, it will not work"[^padding].

But, why should you compare bit by bit?
You should do recursive member by member as we discovered.
But C didn't do it.

But, here we come to something even more egregious.
I just defined equality.
It's utterly horrible that I have to now define inequality because
what's the meaning of inequality? 
Not equality. Could there be any other meaning?

In 1994 I proposed such a thing to the standard committee.
I even proposed bunch of templates which will automatically do it.
They threw them out because there were people who said, "but we want to have the freedom to make the
glyph `!=` do something else."
I literally have no words because that is not freedom.
It's like saying I want to have freedom to run on the street with no pants!
The semantics must be fixed, you have no right
to define inequality which will do a semantically different thing.

What if you determine something is unequal faster?
I don't know such examples but supposing there was, that is fine.
in 99% of the cases it should just be defined (and optionally manually defined).

Notice that we didn't define `!=` as:

    return x.value != y.value;

Why? Because by defining one in terms of the other,
it can always stay the way it is, even if we copy it to other classes.

[^padding]: Padding is a technical fact about how C prepares data to be stored in memory.
    The compiler is not only concerned with memory usage,
    but also how the CPU will address that memory.
    Suppose you write the following:

        typedef struct
        {
            uint16_t id;
            uint32_t number;
        } Record;

    A typical compiler will insert 16 hidden padding bits between `id` and `number`,
    so that all the fields in the structure begin at addresses
    which are multiples of 32 bits.
    Even if you assign all the fields:

        Record r;
        r.id = 1;
        r.number = 20;

    The invisible padding bits will not be modified,
    and so their values are undefined.
    You can read more [here][padding].

[padding]: https://en.cppreference.com/w/c/language/object 
[johnson]: https://en.wikipedia.org/wiki/Stephen_C._Johnson


## Totally ordered singleton

To make `singleton` `TotallyOrdered` we need to define `<` and
related operators.

    // totally ordered
    friend
    bool operator<(const singleton& x, const singleton& y) {
      return x.value < y.value;
    }

    friend
    bool operator>(const singleton& x, const singleton& y) {
      return (y < x);
    }

    friend
    bool operator<=(const singleton& x, const singleton& y) {
      return !(y < x);
    }

    friend
    bool operator>=(const singleton& x, const singleton& y) {
      return !(x < y);
    }

I chose `<` as the primary one out of four.
Considering comparison applied to sorting, you could sort in ascending order or in descending order.
It seems to be natural to pick ascending.
If I asked you to name a bunch of numbers, you would say "one, two, three, four, five".
Only strange people will say "five, four, three, two, one". 
Natural numbers go from one up.

Even though most of the STL algorithms only use `<` you should still define all the others.
I'm not going to use the others, but I will provide them for other people to use.
There is a rule used in the [internet protocol][ip-quote] that you should assume to do everything for your clients,
but expect nothing from them.

Understand how all the other operators are defined in terms of `<`.
Contemplate it.
It's really important.
It is mathematics, but that's mathematics every programmer should be able to do .

[ip-quote]: https://en.wikipedia.org/wiki/Robustness_principle

### Specifying concepts


Now let's talk about what kind of type `T` could be.
Because we implemented all these operators, 
`T` could be `SemiRegular`, `Regular`, or `TotallyOrdered`.
These are requirements on the type `T`, otherwise known
as **concepts**.

The C++ language doesn't have support for concepts at this time[^concepts-proposal].
So, we use comments to communicate them.

In `singleton` we add a comment to describe this:

    // T is semiregular, or regular, or totally-ordered.

It's a good example of a **disjunctive concept**.
`T` could be any of them.


You might wonder how `==` will work, if you plug-in only a type `T`
which is only `SemiRegular`.
In C++, things don't have to be defined unless they are used.
This is a wonderful property on which we are relying.
If you give me `T` which has no equality, it is fine.
It will give me singleton of `T`
which will have copy constructor and assignment but will not have equality.
If `T` has an equality, then singleton will have equality.
Same for total ordering, etc.

**Exercise:** Copy the file for singleton and modify it to write `pair`.

[^concepts-proposal]: [Concepts][cpp-concepts] as a language feature went through many
    iterations and delays before finally being included in the C++20 standard.
    When the course was given, a group at A9 (including Alex) was working to get them included in C++14.
    You can read their [proposal](papers/concepts_proposal.pdf).
    Bjarne actually visits A9 to give a guest lecture on concepts as part of the course,
    however this is not included in the notes as it is a departure from the rest of the material.

[cpp-concepts]: https://en.wikipedia.org/wiki/Concepts_(C%2B%2B)

## Implicit type conversion

Since we wrote a few constructors for `singleton` we should
talk a bit about implicit type conversions.

Once upon a time people invented strong typing.
When they did, they said everything has to have a type.
If you want to make it into a different type you have to convert it, by calling a function.
Makes sense, right?
But, our friends at [Bell Labs][bell] thought it was a bad idea.
They said, "I don't want to write the conversion functions".
Therefore they invented something called implicit conversions.

In C it converts integers to float.
Why did these Bell Labs guys introduce such a thing?
Were they mad? 
It was because they were lazy.
They couldn't do it elegantly because they didn't
have
[function overloading][overloading].

In C++ we have `sqrt(int)`, `sqrt(double)`, etc
You can pass `int`, or `double` and it does the right thing.
Without overloading they had two choices.

1. Introduce `sqrt_double`, `sqrt_float`, `sqrt_int`, etc
   (and implement them all separately).
2. Introduce a palliative (a solution which sort of works).
   We will just promote everything to the highest
   possible type and obviously it's `double`. 

When C++ came about they couldn't just reject the legacy of C.
One of the deals was that the C++ compiler had to compile the entire
UNIX codebase and there was only one place where it broke.
In UNIX there is a system call [`stat`][stat] which returns a struct `stat`.
But, in C++ structs and functions live in the
same namespace the function `stat` is a constructor for structure `stat` which it isn't.
So they had to find workarounds.

But, generally they had this wonderful goal of retaining full compatibility with C.
They said, "we will keep implicit conversions."
I wish they just did keep them the way they were in C but then
they said, "oh but we have to extend them. This is a wonderful thing which we need
to make work for any other type".
So, implicit conversions work for all types and the rules are [extremely complicated][implicit-rules] so this is the only time I'm going to mention them.


Here are the rules:
If you have a conversion from one user defined type to another,
the compiler will look for all the one-step user defined conversions.
Fortunately it is not going to look for
two, three, four, five steps because you know there is combinatorial explosion.

But, it doesn't stop there.
On top of user-defined conversion you could put one more layer of built-in conversions.
If you have a class which is convertible to `int` and a function
which takes `double`.
You could give me the element of a class and it will work.
It seems not to be a big deal,
but then people started writing code like

    std::cin << 42

You would think it couldn't possibly work.
They're trying to write into an input stream.
But, it did.

`std::cin` is convertible to a pointer.
If it fails, it returns `null` with type `void*`.
Since  it is convertible to a pointer, you can apply one
more conversion and convert this pointer to a Boolean.
Then you could convert it to an integer.
So `std::cin` becomes zero and you shifted by forty two positions.
Isn't that beautiful (joke)?

The problem is they fixed this problem by
inventing things called explicit conversion.
But then the `while` statement stopped working:

So they had to break the rule.
Explicit conversions are not going to be called implicitly, unless
they are in `while`, `if`, and other conditions which people used for input streams.
So, the entire type system is screwed up (technical term), to work around some
The end of the story is avoid implicit conversions.
Never rely on them it's impossible to avoid them.
Even if you declare everything explicit there is still a
context in C++ where implicit conversion will be done. 
You should never rely on one type
automatically becoming another.

There are some specialists on C++, even specialists
on STL, who write that STL totally depends on implicit conversions. This
is false. Of course it handles implicit conversions, what else could it do?


## Code

- [singleton.h](code/singleton.h)
- [test_singleton.cpp](code/test_singleton.cpp)


[bell]: https://en.wikipedia.org/wiki/Bell_Labs
[overloading]: https://en.cppreference.com/w/cpp/language/overload_resolution
[stat]: https://linux.die.net/man/2/stat
[implicit-rules]: https://en.cppreference.com/w/cpp/language/implicit_conversion
