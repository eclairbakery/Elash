#!/usr/bin/env python3
import os
import sys
import json

CC          = os.environ.get('CC', 'CC')
CFLAGS      = os.environ.get('CFLAGS', '')
LLVM_CFLAGS = os.environ.get('LLVM_CFLAGS', '')
BUILD       = os.environ.get('BUILD', 'release')

LIBELASH_SRCS   = os.environ.get('LIBELASH_C_SRCS', '').split()
LIBELC_SRCS     = os.environ.get('LIBELC_C_SRCS', '').split()
ELASH_TEST_SRCS = os.environ.get('ELASH_TESTS_SRCS', '').split()
ELC_TEST_SRCS   = os.environ.get('ELC_TESTS_SRCS', '').split()
FUZZ_SRC        = os.environ.get('FUZZ_SRC', '')
MAIN_SRC        = os.environ.get('MAIN_C_SRC', '')

def main():

    base_args = [CC] + CFLAGS.split()
    llvm_args = LLVM_CFLAGS.split()

    entries = []
    cwd = os.getcwd()

    def add_entry(file_path, extra_args):
        obj_root = f"build/{BUILD}/obj"
        obj_path = os.path.join(obj_root, os.path.splitext(file_path)[0] + '.o')

        file_norm = file_path.replace('\\', '/')
        obj_norm = obj_path.replace('\\', '/')

        args = base_args + extra_args + ['-c', file_norm, '-o', obj_norm]
        entries.append({
            "directory": cwd.replace('\\', '/'),
            "arguments": args,
            "file": file_norm
        })


    seen: set[str] = set()

    for f in (*LIBELASH_SRCS, *ELASH_TEST_SRCS, FUZZ_SRC):
        if f not in seen:
            seen.add(f)
            add_entry(f, [])

    for f in (*LIBELC_SRCS, *ELC_TEST_SRCS, MAIN_SRC):
        if f not in seen:
            seen.add(f)
            add_entry(f, llvm_args)

    with open('compile_commands.json', 'w') as out:
        json.dump(entries, out, indent=2)
        out.write('\n')

if __name__ == '__main__':
    main()
