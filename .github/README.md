## FAQ

**I have a correction, additional reference, or other helpful idea.**

Please, make a pull request.
Corrections are welcome and needed.
However, be aware that some of the awkwardness of grammar
comes from the speaker and is intentionally preserved.

Please make changes to the markdown files (not the HTML).
If you do not want to build the HTML for a change,
I am happy to do so for you.

## Build Dependencies

Building requires 2 widely available dependencies:

- `discount` (markdown implementation)
- `envsubst`

**macOS Homebrew**

    brew install gettext discount

**Debian/Ubuntu**

    sudo apt install gettext-base discount

## Build script

    ./build.sh

## Build with Docker

If you don't want to install anything in your enviornment
we have included a convenience script which uses docker.

    ./docker_build.sh



