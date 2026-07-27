FROM alpine:latest

RUN apk add --no-cache \
    build-base \
    llvm \
    llvm-dev \
    git \
    meson \
    ninja \
    cmake \
    libffi-dev

RUN git clone --recursive https://github.com/Snaipe/Criterion.git /tmp/criterion \
    && cd /tmp/criterion \
    && meson setup build \
    && meson compile -C build \
    && meson install -C build \
    && cd / \
    && rm -rf /tmp/criterion

RUN apk del meson ninja cmake libffi-dev

