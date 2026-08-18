import threading
import os

from concurrent.futures import (
    ThreadPoolExecutor, as_completed
)
from pathlib import Path

from .models  import *
from .console import *
from .defs    import *

from .test import run_test_case, get_expectation

def report_failure(name: str, expected: TestExpectation, actual: TestResult):
    if isinstance(actual, TimedOutResult):
        print_timeout(name)
        print_info(f'  Timed out during {CLR_BOLD}{actual.stage}{CLR_RESET} stage')
        return

    print_fail(name)

    is_unexpected_stage = False
    if expected.diags is not None:
        if actual.stage != 'compilation':
            is_unexpected_stage = True
    else:
        if actual.stage != 'runtime':
            is_unexpected_stage = True

    if is_unexpected_stage:
        print_info(f'  Error during {CLR_BOLD}{actual.stage}{CLR_RESET} stage (exitcode {actual.exitcode})')
        for stream in ['stdout', 'stderr']:
            content = getattr(actual, stream)
            if content:
                print_info(f'  {actual.stage} {stream}:')
                for line in content.splitlines():
                    print_info(f'    {line}')
    else:
        if actual.exitcode != expected.exitcode:
            print_info(f'  exitcode: expected {expected.exitcode}, actual {actual.exitcode}')

        if expected.diags:
            combined_output = actual.stdout + actual.stderr
            for code in expected.diags:
                if code not in combined_output:
                    print_info(f'  missing diag code: {CLR_BOLD}{code}{CLR_RESET}')

        if expected.stdout or actual.stdout:
            print_diff(expected.stdout, actual.stdout, 'stdout')
        if expected.stderr or actual.stderr:
            print_diff(expected.stderr, actual.stderr, 'stderr')

def _collect_test_items():
    test_items = []
    for p in script_dir.rglob('input.eu'):
        test_items.append(p.parent)
    for p in script_dir.rglob('*.eu'):
        if p.name != 'input.eu':
            test_items.append(p)
    return sorted(test_items)

def _is_success(expected: TestExpectation, actual: TestResult) -> bool:
    if isinstance(actual, TimedOutResult):
        return False

    if expected.diags is not None:
        if actual.stage != 'compilation':
            return False
        if actual.exitcode == 0:
            return False
        if expected.exitcode != 0 and actual.exitcode != expected.exitcode:
            return False
        combined_output = actual.stdout + actual.stderr
        for code in expected.diags:
            if code not in combined_output:
                return False
        return True
    else:
        if actual.stage != 'runtime':
            return False
        if actual.exitcode != expected.exitcode:
            return False
        if actual.stdout != expected.stdout:
            return False
        if actual.stderr != expected.stderr:
            return False
        return True

def run_suite(elc_bin: Path, work_dir: Path, jobs: Optional[int], timeouts: Timeouts) -> bool:
    passed_count  = 0
    failed_count  = 0
    skipped_count = 0

    test_items = _collect_test_items()

    tasks = []
    for path in test_items:
        name = str(path.relative_to(script_dir)).removesuffix(".eu")
        tasks.append((path, name, get_expectation(path, name)))

    def handle_result(name, expected, actual):
        nonlocal passed_count, failed_count, skipped_count
        if actual is None:
            print_skip(name)
            skipped_count += 1
        elif _is_success(expected, actual):
            print_pass(name)
            passed_count += 1
        else:
            report_failure(name, expected, actual)
            failed_count += 1

    if jobs is None or jobs > 1:
        with ThreadPoolExecutor(max_workers=jobs) as executor:
            future_to_test = {}
            for path, name, expected in tasks:
                future = executor.submit(run_test_case, elc_bin, work_dir, path, name, expected.diags is not None, timeouts)
                future_to_test[future] = (name, expected)

            for future in as_completed(future_to_test):
                name, expected = future_to_test[future]
                handle_result(name, expected, future.result())
    else:
        for path, name, expected in tasks:
            handle_result(name, expected, run_test_case(elc_bin, work_dir, path, name, expected.diags is not None, timeouts))

    tested_count = passed_count + failed_count + skipped_count
    print(f'[{CLR_BLUE}===={CLR_RESET}] {CLR_BOLD}Synthesis: ', end='')
    print(f'Tested: {CLR_BLUE}{tested_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Passing: {CLR_GREEN}{passed_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Failing: {CLR_RED}{failed_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Skipped: {CLR_BLUE}{skipped_count}{CLR_RESET}{CLR_BOLD}', end='')
    print(CLR_RESET)

    return failed_count == 0
