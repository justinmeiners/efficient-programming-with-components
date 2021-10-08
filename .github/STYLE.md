Style guide
===========

## File format

The text should be standard markdown with the addition of the footnotes extension.
It should be readable in markdown form.
Avoid using HTML. 
Simple HTML is allowed when there are no good alternatives.

## Accuracy of text

These are course notes, not a transcript.
I try to discern and communicate his intended message,
not necessarily make a historical record of the conversation.

If Alex is sharing a strong opinion or giving a speech,
it is likely an exact quote (minus grammar and filler words).
The technical exposition is where 
I have rearranged and filled in missing parts
for educational purposes.

## Voice

Try to match Alex's voice.

## Exercises

### When to include?

Exercises may originate from Alex, or our own suggestion.
They should always reinforce material learned,
or offer challenges to prepare for upcoming material.

Every chapter should have at least one.

### How difficult shoult they be?

They should not be obvious how to get started (avoid tricky puzzles)
but may take some programming time. 

### Provding solutions

When exercises have a solution later in the text,
this should be indicated by a parenthetical statement such as.

> Find the sum of integers from 1 to n (solved in chapter 3).

Exercises may be solved in footnotes.

## Links

### When should I use inline links? 

Inline links should correspond exactly with their subject.
They should be obvious and only come from general sources.
For example, we might provide an inline link to a persons's Wikipedia page
or personal website when using their name.
Another example is linking to a documentation page when mentioning 
an STL function like `std::sort`.

However, when linking off to more detailed explanations,
or information from a specialized source, a footnote
with a brief explanation should be provided.

### Why use inline links at all?

Many of the links may seem unnecessary as they are a quick Google search away.
I added those for convenience and historical purposes.
References which are obvious now may be difficult to track down in the future.

### Where should links be place in the markdown file? 

Links should be placed at the bottom of the section of the content they are related to.
This makes it easier to move content around and.

## Story

Each chapter should begin with an engaging story or opinion.

## Code

Each chapter should have links to completed code files at the bottom.
The code follows Alex's style including:

- two space indent
- snake case (STL naming)
- { on same line as statement

## References to literature

Titles of books and articles are referred to with quotes: "The Art of Computer Programming".

## Headings

Only the first word of a heading should be capitalized.

## Alex footnotes

Occassionally Alex will make comments which are interesting but not related
to the lesson or directly applicable to the audience.
These can be moved to footnotes, but should be indicated 
in the following manner:

    > Alex: here is a quote.

No quotation marks are used.

## Quotes 

Alex will sometimes use a voice as another person or group of people talking.
For example:

    You might say: "Who are you to tell us?"

Quotation marks should be used in these cases.

## Definitions

When defining or introducing a new word,
it should be in **bold**.

## Footnotes

### When are foonotes required?

When Alex references a piece of literature, individual, or event, we should find sources to support it.

### When are optional footnotes appropriate?

Footnotes can be added for any of the following reasons:

- explain more about the material
- connect it to another subject or topic
- provide additional examples
- Provide supporting references.

### What sources should footnotes refer to?

When providing additional explanation or reference material,
we should prefer to use books which Alex recommends, over other materials.
These include:

- "The Art of Computer Programming"
- "Structure and Interpretation of Computer Programs"
- cppreference vs cplusplus
- "Computer Architecture" (Brooks)


