#!/usr/bin/env bash
cd "$(dirname "$0")" || exit
set -eu

source defs.sh

chmod +x build/directory.sh

run() {
    echo "--------- building $1 -----------"

    local path="build/$1.sh"
    chmod +x "$path"
    "$path"
    code="$?"

    echo
    return $code
}

run x86_64-linux
run aarch64-linux
run x86_64-windows

cp "$BUILD_DIR/aarch64-linux/out/"*-release.tar.gz "$DIST_DIR/"
cp "$BUILD_DIR/x86_64-linux/out/"*-release.tar.gz "$DIST_DIR/"
cp "$BUILD_DIR/x86_64-windows/out/"*-release.tar.gz "$DIST_DIR/"
