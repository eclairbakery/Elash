#!/usr/bin/env bash
cd "$("$(dirname "$0")"/directory.sh "x86_64-linux")" || exit

TOOLCHAIN_GXX_DIR=$(dirname "$(/opt/x86_64-linux-musl-cross/bin/x86_64-linux-musl-g++ -print-file-name=libstdc++.a)")
TOOLCHAIN_GCC_DIR=$(dirname "$(/opt/x86_64-linux-musl-cross/bin/x86_64-linux-musl-gcc -print-file-name=libc.a)")

MUSL_DEPS="$HOME/build/musl-libs"

export EXTRA_LDFLAGS="-static -L${TOOLCHAIN_GXX_DIR} -L${TOOLCHAIN_GCC_DIR} -L${MUSL_DEPS}/lib"
export EXTRA_CFLAGS="-I${MUSL_DEPS}/include"

make -C "$PROJECT_ROOT" archive -j"$(nproc)" \
    BUILD=release \
    CC="x86_64-linux-musl-gcc" \
    AR="x86_64-linux-musl-ar" \
    LLVM_CONFIG="/opt/musl-llvm/bin/llvm-config" \
    EXTRA_CFLAGS="$EXTRA_CFLAGS" \
    EXTRA_LDFLAGS="$EXTRA_LDFLAGS" \
    OBJ_ROOT_DIR="$PWD/build/obj" \
    DEP_ROOT_DIR="$PWD/build/dep" \
    OUT_DIR="$PWD/out"
