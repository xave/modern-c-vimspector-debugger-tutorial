#!/usr/bin/env bash

set -eou pipefail
#set -x # print debugging

CFLAGS="-Wall -Wextra -fexceptions -DNDEBUG "
#CFLAGS+="-arch x86_64 " #Linux
CFLAGS+="-arch arm64 " #macOS
CFLAGS+="-g "          #DEBUGGING

CC="clang"
SRCS="src/sizeof.c"
BUILDAPP="./build/app"

mkdir -p ./build/
$CC $CFLAGS -o $BUILDAPP $SRCS
