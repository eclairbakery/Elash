#!/usr/bin/env bash
cd "$("$(dirname "$0")"/directory.sh "x86_64-linux")" || exit

make -C "$PROJECT_ROOT" archive -j"$(nproc)" \
    BUILD=release \
    OBJ_ROOT_DIR="$PWD/build/obj" \
    DEP_ROOT_DIR="$PWD/build/dep" \
    OUT_DIR="$PWD/out"

