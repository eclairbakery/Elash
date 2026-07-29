prev="$(pwd)"
cd "$(dirname "${BASH_SOURCE[0]}")" || exit 1

# shellcheck disable=SC2155
export PROJECT_ROOT="$(realpath ../..)"
export RELEASE_DIR="${RELEASE_DIR:-"$PROJECT_ROOT/.local/release"}"
export DIST_DIR="$RELEASE_DIR/dist"
export BUILD_DIR="$RELEASE_DIR"

mkdir -p "$RELEASE_DIR" "$DIST_DIR" "$BUILD_DIR"

cd "$prev" || exit 1
