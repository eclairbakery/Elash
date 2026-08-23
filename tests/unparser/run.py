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

from tests.e2e.runner.suite import collect_test_cases
from tests.e2e.runner.console import *

def run_test(runner_bin: Path, case) -> tuple[int, str]:
    input_file = case.path / 'input.eu'
    if not input_file.is_file():
        input_file = case.path
    if not input_file.is_file():
        error('ill-formed test case')

    result = subprocess.run([str(runner_bin), str(input_file)], capture_output=True)
    return result.returncode, result.stderr.decode()

class CliArgs(argparse.Namespace):
    runner:   Path
    parallel: Optional[int]

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('runner', type=Path, help='Path to the unparser runner binary')
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

    with ThreadPoolExecutor(max_workers=args.parallel) as executor:
        futures = {
            executor.submit(run_test, args.runner, c): c for c in test_cases
        }

        for future in as_completed(futures):
            case = futures[future]
            exitcode, stderr = future.result()

            if exitcode == 0:
                passed_count += 1
                print_pass(case.name)
            else:
                print_fail(case.name)
                if exitcode < 0:
                    crashed_count += 1
                    print_info(f'  crashed: {Signals(-exitcode).name}')
                else:
                    failed_count += 1
                    print_info(f'  exit code: {exitcode}')

                if len(stderr) != 0:
                    print_info('  stderr:')
                    for line in stderr.split('\n'):
                        print_info('    ' + line)

    tested_count = passed_count + failed_count + crashed_count
    print(f'[{CLR_BLUE}===={CLR_RESET}] {CLR_BOLD}Synthesis: ', end='')
    print(f'Tested: {CLR_BLUE}{tested_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Passing: {CLR_GREEN}{passed_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Failing: {CLR_RED}{failed_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Crashed: {CLR_ORANGE}{crashed_count}{CLR_RESET}{CLR_ORANGE}', end='')
    print(CLR_RESET)

    return 1 if (failed_count + crashed_count) > 0 else 0

if __name__ == '__main__':
    main()
