Foreword
==========

Alexander Stepanov is well-known for being the creator of the C++ "Standard Template Library" (STL).
Anyone who has used `std::vector` has benefited from his work.
Much less well-known is that STL is not just a collection
of common routines;
it's an ecosystem for a *theory of programming*
championed by Alex called **generic programming**.
For those not familiar with the ideas of generic programming, the STL
can be a confusing and verbose mess.
But with just a short introduction to generic 
programming
principles, its thoughtful design and capability
for writing reusable and correct code become clear.
These lectures are intended to teach exactly that.

Programming, as distinct from computer science,
tends to be described as a craft or art,
rather than a field of science or engineering.
It's learned by experience and by studying a loose collection
of examples and rules passed down;
don't make functions "too long," hide "unnecessary" details
in abstractions, group "similar" functionality,
don't use global variables,
avoid duplicating code.
While these hint at some truly good ideas, they also 
suggest that perhaps the field's underlying principles
have yet to be understood.

Much more than anything in particular about C++,
Alex's work is about discovering these principles.
It's about the basic logic and math which govern programs.
The power of this foundational approach is being recognized in more areas.
(For example, see Apple's WWDC session on [Protocol-Oriented Programming][wwdc].)

Generic programming starts with algorithms.
Given an algorithm, how do we know it will work?
To answer this question, a programmer must discover its essential mechanism
and, consequently, the minimal requirements for it to work.
These requirements are described as mathematical
constraints on the algorithm's inputs.
From this understanding, we can *express* the algorithm
in an implementation useful for others, called a **component**.
Complex programs are written simply by combining simple
components that are well understood.

Components facilitate abstraction and code reuse through minimalism;
code is reduced to its essential function,
as opposed to traditional black-box layering
which merely hides information.
Programs written this way also tend to be fast,
which contributes to their reusability.
If a component doesn't perform, 
there will always be some performance-sensitive application
that can't use it.
Of course, not every algorithm is the best choice for all needs,
but those limitations should be imposed by the algorithm,
not the implementation.

Alex's work makes an additional contribution 
to the question, what does developing software look like in the future?
Will we scavenge for snippets to piece together from the GitHub junkyard?
Will programs be carefully specified and verified with formal proof systems?
Can you `npm install banking-app-starter-template` and 
start customizing?
Are all programs written by hobbyist C artisans? (the vision I'm most partial to)

Alex derives a different model from generic programming,
resembling engineering as done in other fields.
Mathematicians and computer scientists discover algorithms.
Software engineers work together as an industry to implement these as 
standard components with a high degree of professionalism.
These are distributed as standardized catalogs.
Each component is documented in detail, with references to academic literature.
They specify their requirements and complexity in mathematical terms.
It displays graphs of their performance on various data sets.
Programmers carefully select components that fit their needs
and assemble them into applications.
To understand these catalogs and make contributions of their own,
programmers must be educated in basic mathematics
and computer science
in the same way that engineers require physics and calculus.

-- Justin Meiners


[wwdc]: https://developer.apple.com/videos/play/wwdc2015/408/

# Acknowledgments

Original course by Alexander. A. Stepanov.

![alex](img/alex.jpg)

Course notes were assembled from videos, course materials, interviews,
and books by [Justin Meiners](https://github.com/justinmeiners) in 2021.
The lectures were given in 2013 at [A9](https://en.wikipedia.org/wiki/A9.com).
These notes are intended to share scientific information for educational and historical purposes. This is a non-commercial project.
Most of the code comes from [Ryan Ernst's repo](https://github.com/rjernst), who attended the lectures.

Special thanks to [Alastair Harrison][aharrison] for his significant efforts editing and providing feedback.

The following people also helped provide corrections:
[Petter Holmberg][petter], [Ryan Pendleton][ryanp], Frank Ettwein, [Yuri Valentini][yuroller].

[aharrison]: https://github.com/aharrison24
[ryanp]: https://github.com/rpendleton
[petter]: https://github.com/petter-holmberg
[yuroller]: https://github.com/yuroller

# FAQ

**Why do we need these notes if we can watch the [videos][videos]?**

This has all the information in an organized form, with references
and working code.

The videos are often hard to watch due to the slow pace and interaction with the audience.
Sometimes a mistake is made, and they go back and fix it.
Alex may introduce a story and then finish it days later.
Consequently, some videos have fewer than 800 views, and at least 10 of those views are mine.

[videos]: https://www.youtube.com/watch?v=aIHAEYyoTUc&list=PLHxtyCq_WDLXryyw91lahwdtpZsmo4BGD

**Is all this information available in his books?**

A majority of the information, but not the code, is available in other forms.
In these lectures, you get history, opinions, motivation, practical tips, applications, and responses to criticism, all at once.
This is not present in the books, especially "Elements of Programming" which is very formal.
Having this rich context makes the books much more approachable and meaningful.

**This is ten years old. Is it relevant for a C++ programmer today?**

Absolutely. He will almost certainly teach you applicable things about
current-day C++ and STL you did not know.

**Is this relevant for a non-C++ programmer?**

These ideas are not limited to C++ and are being adopted
in more and more libraries and languages.
I hardly ever write C++ anymore.
But, if you have no familiarity with C++, then
this presentation will probably be difficult
to follow.
Try the first lesson.

**How similar is Alex's vision to the "modern C++" style?**

The emphasis on value types and templates over dynamically allocated
objects with virtual members is similar,
but I think that's where the similarities end.

It's important to emphasize that this is not a style guide,
but a theory of programming.
The C++ style is then one way of representing this theory in C++.

In my opinion, (somewhat supported by various comments made by Alex),
most developments since C++11 seemed to have missed the point of STL.
A lot of it has been partially understood and remixed in strange and inconsistent ways.
Things which were understood, are no longer (see [std::find is broken][find-broken]).

If you study these notes, you will find that Alex outlined much cleaner solutions to many of the problems which later C++ versions attempt to address. 
The most baffling of these to me are three-way comparison and ranges.
I am unsure of his opinion on move semantics.
I am unable to comment on whether the Concepts which made it into C++20
are anything like his original vision.

[find-broken]: https://sean-parent.stlab.cc/papers-and-presentations/#warning-stdfind-is-broken

**I have a correction, additional reference, or other helpful idea.**

Please, make a [pull request](https://github.com/justinmeiners/efficient-programming-with-components).
Corrections are welcome and needed.
However, be aware that some of the awkwardness of grammar
comes from the speaker and is intentionally preserved.

**How accurate is this text?**

The goal of this project is to provide course notes, not a transcript.
I try to discern and communicate his intended message,
not necessarily make a historical record of the conversation.

If Alex is sharing a strong opinion or giving a speech,
I usually quote him exactly (with corrections to grammar) to avoid misinterpreting him.
The technical exposition is where significant rearrangements and have been made for educational purposes.

