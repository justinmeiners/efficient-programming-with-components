#!/bin/sh

# This is a wrapper that executes build.sh inside a docker container which has
# all of the required third-party dependencies.

script_path="$( cd "$(dirname "$0")" || exit $?; pwd -P )"

docker build -t meiners/epwc -<<EOF
FROM ubuntu:focal
RUN apt-get update && apt-get install -y \
    gettext-base \
    discount \
 && rm -rf /var/lib/apt/lists/*
EOF

# permission fix?
# https://stackoverflow.com/questions/26500270/understanding-user-file-ownership-in-docker-how-to-avoid-changing-permissions-o/29584184#29584184
docker run \
  --rm \
  -v "${script_path}:/src" \
  -w /src \
  -u `stat -c "%u:%g" .` \
  meiners/epwc \
  "./build.sh"
