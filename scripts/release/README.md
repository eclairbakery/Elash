## Elash release scripts

This directory contains my personal scripts for building releases for a few different targets:
 - `x86_64-linux`
 - `aarch64-linux`
 - `x86_64-windows` (mingw)

I'm sharing these mostly in case they're useful as inspiration
or a reference for setting up your own build environment.

However it's very likely that they just won't work out of the box on your setup
because they are specific to my particular environment (hard-coded paths etc.).

I think it's also worth noting that the windows script is VERY tricky
and unfinished (as many libraries that most windows systems doesn't have
are dynamically linked). That's mostly because windows sucks.

> [!NOTE]
> Future plans include containerizing the build environment to make these scripts portable!

You can copy this directory to your .local/ directory in the repository root and make your changes.

> [!WARNING]
> Do not commit them!

The scripts are mostly designed for Arch Linux *(btw)* x86_64.
