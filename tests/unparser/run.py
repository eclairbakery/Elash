#!/usr/bin/env python3
import subprocess
import argparse
import typing
import sys

from pathlib import Path
from signal import Signals
from typing import Optional
from dataclasses import dataclass

from concurrent.futures import ThreadPoolExecutor, as_completed

from tests.e2e.runner.suite   import collect_test_cases
from tests.e2e.runner.test    import resolve_test_paths
from tests.e2e.runner.models  import TestCase
from tests.e2e.runner.console import *

def run_test(runner_bin: Path, case: TestCase) -> Optional[tuple[int, str]]:
    input_files, skip_file = resolve_test_paths(case.path, case.name)
    if skip_file is not None and skip_file.is_file():
        return None

    for f in input_files:
        result = subprocess.run([str(runner_bin), str(f)], capture_output=True)
        if result.returncode != 0:
            return result.returncode, result.stderr.decode()

    return 0, ''

class CliArgs(argparse.Namespace):
    runner:   Path
    verbose:  bool
    parallel: Optional[int]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('runner', type=Path, help='Path to the unparser runner binary')
    parser.add_argument('-v', '--verbose', action='store_true')
    parser.add_argument(
        '-j', '--parallel', nargs='?',
        const=None, default=1, type=int,
        help='run in parallel with optional N workers limit'
    )

    args: CliArgs = typing.cast(CliArgs, parser.parse_args())

    test_cases = [c for c in collect_test_cases() if c.type == 'positive']

    passed_count  = 0
    failed_count  = 0
    crashed_count = 0
    skipped_count = 0

    with ThreadPoolExecutor(max_workers=args.parallel) as executor:
        futures = {
            executor.submit(run_test, args.runner, c): c for c in test_cases
        }

        for future in as_completed(futures):
            case = futures[future]
            result = future.result()

            if result is None:
                skipped_count += 1
                print_skip(case.name)
                continue

            exitcode, stderr = result

            if exitcode == 0:
                passed_count += 1
                if args.verbose:
                    print_pass(case.name)
            else:
                print_fail(case.name)
                if exitcode < 0:
                    crashed_count += 1
                    print_info(f'  crash: {Signals(-exitcode).name}')
                else:
                    failed_count += 1
                    print_info(f'  exit code: {exitcode}')

                if len(stderr) != 0:
                    print_info('  stderr:')
                    for line in stderr.split('\n'):
                        print_info('    ' + line)

    tested_count = passed_count + failed_count + crashed_count + skipped_count
    print(f'[{CLR_BLUE}===={CLR_RESET}] {CLR_BOLD}Synthesis: ', end='')
    print(f'Tested: {stat(tested_count, CLR_BLUE)}{CLR_BOLD} ', end='')
    print(f'| Passing: {stat(passed_count, CLR_GREEN)}{CLR_BOLD} ', end='')
    print(f'| Failing: {stat(failed_count, CLR_RED)}{CLR_BOLD} ', end='')
    print(f'| Crashing: {stat(crashed_count, CLR_ORANGE)} ', end='')
    if skipped_count != 0:
        print(f'| Skipped: {CLR_BLUE}{skipped_count}{CLR_RESET}', end='')
    print(CLR_RESET)

    return 1 if (failed_count + crashed_count) > 0 else 0

if __name__ == '__main__':
    main()
