#!/usr/bin/env bash
cd "$(dirname "$0")" || exit 1
set -eu

source defs.sh || exit 1

if [[ $# -ne 0 && $# -ne 1 ]]; then
    echo "usage:"
    echo "  $0"
    echo "  $0 build"
    echo "  $0 dist"
fi

case "${1:-}" in
    "")    rm -rf "$RELEASE_DIR" ;;
    build) rm -rf "$BUILD_DIR"   ;;
    dist)  rm -rf "$DIST_DIR"    ;;
esac

