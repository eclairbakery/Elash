#!/usr/bin/env python3
import subprocess
import argparse
import tempfile
import typing
import sys
import os

from concurrent.futures import ProcessPoolExecutor, as_completed
from multiprocessing import Lock

from pathlib import Path

CLR_BLUE   = '\033[0;34m'
CLR_RESET  = '\033[0m'

print_lock = Lock()
def print_fuzz(i: int):
    with print_lock:
        print(f'[{CLR_BLUE}FUZZ{CLR_RESET}] Iteration {i}')

def run_fuzzer(fuzzer: Path, out_path: str) -> int:
    with open(out_path, 'wb') as fout:
        result = subprocess.run([str(fuzzer)], stdout=fout, stderr=subprocess.PIPE)

    if result.stderr: sys.stderr.buffer.write(result.stderr)
    return result.returncode

def run_elc(elc: Path, file_path: str) -> int:
    cmd = [str(elc), 'inspect', file_path, '--until=ast', '--emit=ast']
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.stderr: sys.stderr.buffer.write(result.stderr)
    return result.returncode

class CliArgs(argparse.Namespace):
    fuzzer:  Path
    elc:     Path
    count:   int
    verbose: bool

    jobs: int | None

def do_fuzzing_stuff(args: CliArgs, i: int) -> int:
    tmp: str
    with tempfile.NamedTemporaryFile(delete=False) as f:
        tmp = f.name

    try:
        if args.verbose:
            print_fuzz(i)

        fuzzer_exitcode = run_fuzzer(args.fuzzer, tmp)
        if fuzzer_exitcode != 0:
            sys.stderr.write(f'Fuzzer exited with non-zero status {fuzzer_exitcode} on iteration #{i+1}\n')
            return fuzzer_exitcode

        elc_exitcode = run_elc(args.elc, tmp)
        if elc_exitcode != 0:
            sys.stderr.write(f'Elash compiler failed with status {elc_exitcode} on iteration #{i+1}\n')
            return elc_exitcode;
    finally:
        os.unlink(tmp)

    return 0

def main() -> int:
    parser = argparse.ArgumentParser(description='Run fuzzer multiple times and inspect output.')
    parser.add_argument('fuzzer',type=Path,                     help='Path to the fuzzer binary')
    parser.add_argument('elc',   type=Path,                     help='Path to the elc binary');
    parser.add_argument('count', type=int,                      help='Number of iterations to run')
    parser.add_argument('-j', '--jobs', type=int, default=None, help='Number of parallel workers');
    parser.add_argument('-v', '--verbose', action='store_true', help='Enable verbose logging')
    args = typing.cast(CliArgs, parser.parse_args());

    with ProcessPoolExecutor(max_workers=args.jobs) as executor:
        futures = {
            executor.submit(do_fuzzing_stuff, args, i): i
            for i in range(args.count)
        }

        exit_code = 0
        for future in as_completed(futures):
            res = future.result()
            if res != 0:
                exit_code = res
                executor.shutdown(wait=False, cancel_futures=True)
                break

    return exit_code

if __name__ == '__main__':
    sys.exit(main())
