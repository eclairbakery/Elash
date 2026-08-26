#!/usr/bin/env python3
import subprocess
import argparse
import tempfile
import typing
import sys
import os
import signal
import random

from concurrent.futures import ThreadPoolExecutor, as_completed
from threading import Lock, Event
from pathlib import Path

CLR_BLUE   = '\033[0;34m'
CLR_RED    = '\033[0;31m'
CLR_RESET  = '\033[0m'

print_lock = Lock()

def report_failure(stage: str, returncode: int, i: int, seed: int):
    if returncode == 0:
        msg = "success"
    elif returncode < 0:
        signum = -returncode
        try:
            msg = signal.Signals(signum).name
        except ValueError:
            msg = f"signal {signum}"
    else:
        msg = f"status {returncode}"

    with print_lock:
        print(f'[{CLR_RED}FAIL{CLR_RESET}] {stage} failed with {msg} on iteration #{i + 1}, with seed = {seed}', file=sys.stderr)

def print_fuzz(i: int, seed: int):
    with print_lock:
        print(f'[{CLR_BLUE}FUZZ{CLR_RESET}] iteration {i + 1}, seed {seed}')

def run_fuzzer(fuzzer: Path, out_path: str, seed: int) -> int:
    with open(out_path, 'wb') as fout:
        result = subprocess.run([str(fuzzer), str(seed)], stdout=fout, stderr=subprocess.PIPE)

    if result.stderr: sys.stderr.buffer.write(result.stderr)
    return result.returncode

def run_elc(elc: Path, file_path: str, until: str) -> int:
    cmd = [str(elc), 'inspect', file_path, f'--until={until}']
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if result.stderr: sys.stderr.buffer.write(result.stderr)
    return result.returncode

class CliArgs(argparse.Namespace):
    fuzzer:  Path
    elc:     Path
    count:   int
    verbose: bool

    jobs: int | None

def do_fuzzing_stuff(args: CliArgs, i: int, shutdown: Event) -> int:
    seed = random.randint(0, 2**31 - 1)
    tmp: str
    with tempfile.NamedTemporaryFile(delete=False) as f:
        tmp = f.name

    try:
        if args.verbose:
            print_fuzz(i, seed)

        if shutdown.is_set(): return 0
        code = run_fuzzer(args.fuzzer, tmp, seed)
        if code != 0:
            if not shutdown.is_set():
                report_failure("fuzzer", code, i, seed)
            return code

        if shutdown.is_set(): return 0
        code = run_elc(args.elc, tmp, 'ast')
        if code != 0:
            if not shutdown.is_set():
                report_failure("parser check", code, i, seed)
            return code

        if shutdown.is_set(): return 0
        code = run_elc(args.elc, tmp, 'lir')
        if code < 0:
            if not shutdown.is_set():
                report_failure("elc", code, i, seed)
            return code
    except KeyboardInterrupt:
        # just in case
        return 0
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

    shutdown = Event()

    with ThreadPoolExecutor(max_workers=args.jobs) as executor:
        futures = {
            executor.submit(do_fuzzing_stuff, args, i, shutdown): i
            for i in range(args.count)
        }

        exit_code = 0
        try:
            for future in as_completed(futures):
                res = future.result()
                if res != 0:
                    exit_code = res
                    shutdown.set()
                    executor.shutdown(wait=False, cancel_futures=True)
                    break
        except KeyboardInterrupt:
            shutdown.set()
            executor.shutdown(wait=False, cancel_futures=True)
            return 130

    return exit_code

if __name__ == '__main__':
    sys.exit(main())
