#!/bin/sh

rm -f *.html
rm -f *.bak

ALL_MD="$(echo *.md | sort)"

export TOC="index.html"


# I need to keep track of "next" and "previous"
# so I have this ugly loop

PREV_MD=""
MD=""

FOR_ITERATION="$ALL_MD noop"

for NEXT_MD in $FOR_ITERATION
do
    if [ ! -z "$MD" ]
    then 
      BASE="$(basename $MD .md)"
      TARGET="$BASE.html"

      export TITLE="$(head -n 1 $MD)"

      if [ "$NEXT_MD" = "noop" ]
      then
        export NEXT=""
      else
        export NEXT="$(basename $NEXT_MD .md).html"
      fi

      if [ -z "$PREV_MD" ]
      then 
        export PREVIOUS=""
      else
        export PREVIOUS="$(basename $PREV_MD .md).html"
      fi

      cat template/prefix.html | envsubst >> $TARGET
      cat template/page_prefix.html | envsubst >> $TARGET
      markdown -f toc -f footnote $MD >> $TARGET
      cat template/page_prefix.html | envsubst >> $TARGET
      cat template/suffix.html | envsubst >> $TARGET
    fi

    PREV_MD=$MD
    MD=$NEXT_MD
done



# index page

export TITLE="Efficient Programming with Components"
export NEXT="01_data_structures.html"
export PREVIOUS=""
TARGET="index.html"

cat template/prefix.html | envsubst >> $TARGET
cat template/page_prefix.html | envsubst >> $TARGET

cat template/cover.html >> $TARGET

# fill with table of contents
for MD in $ALL_MD
do
    BASE=$(basename $MD .md)
	markdown -n -T -f toc -f footnote $MD | sed "s/href=\"#/href=\"${BASE}.html#/g" >> $TARGET
done

cat template/page_prefix.html | envsubst >> $TARGET
cat template/suffix.html | envsubst >> $TARGET


