#!/bin/sh

# check depencencies
MD_BUILD=markdown

$MD_BUILD --version | grep discount > /dev/null

if [ $? -ne 0 ]
then
    echo "markdown --version did not match discount."
    exit 1
fi

envsubst --version > /dev/null

if [ $? -ne 0 ]
then
    echo "cannot find envsubst"
    exit 1
fi


# cleanup generated content
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
      $MD_BUILD -f toc -f footnote $MD >> $TARGET
      cat template/page_prefix.html | envsubst >> $TARGET
      cat template/suffix.html | envsubst >> $TARGET
    fi

    PREV_MD=$MD
    MD=$NEXT_MD
done



# index page

export TITLE="Efficient Programming with Components"
export NEXT="00_foreword.html"
export PREVIOUS=""
TARGET="index.html"

cat template/prefix.html | envsubst >> $TARGET
cat template/page_prefix.html | envsubst >> $TARGET

cat template/cover.html >> $TARGET

# fill with table of contents
for MD in $ALL_MD
do
    BASE=$(basename $MD .md)
	$MD_BUILD -n -T -f toc -f footnote $MD | sed "s/href=\"#/href=\"${BASE}.html#/g" >> $TARGET
done

cat template/page_prefix.html | envsubst >> $TARGET
cat template/suffix.html | envsubst >> $TARGET


