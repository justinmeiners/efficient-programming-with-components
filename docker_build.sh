#!/bin/sh

# This is a wrapper that executes build.sh inside a docker container which has
# all of the required third-party dependencies.

script_path="$( cd "$(dirname "$0")" || exit $?; pwd -P )"

docker build -t epwc-build -<<EOF
FROM alpine:3
RUN  apk add --no-cache gettext discount
EOF

docker run \
  --rm \
  -v "${script_path}:/src" \
  -w /src \
  -u `id -u`:`id -g` \
  epwc-build \
  "./build.sh"
