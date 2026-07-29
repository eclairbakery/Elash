#!/usr/bin/env bash

fallback() {
    echo "./$1"
    exit
}

cd "$(dirname "$0")" || fallback "$@"
source ../defs.sh

cd "$BUILD_DIR" || fallback "$@"

mkdir -p "$1"
cd "$1" || fallback "$@"

pwd
exit
