import threading
import os
import json

from concurrent.futures import (
    ThreadPoolExecutor, as_completed
)
from pathlib import Path

from .models  import *
from .console import *
from .defs    import *

from .test import run_test_case, get_expectation

def _parse_jsonl_diagnostics(actual: FinishedResult):
    diagnostics = []
    output = actual.stdout + "\n" + actual.stderr

    errors: list[Exception] = []
    for line in output.splitlines():
        try:
            data = json.loads(line)
            if data['type'] == 'diag':
                diagnostics.append(data)
        except (json.JSONDecodeError, KeyError) as err:
            errors.append(err)

    if len(errors) != 0:
        if len(errors) == 1:
            print_info(f'failed to parse elc output: {errors[0]}')
        else:
            print_info(f'failed to parse elc output:')
            for i, err in enumerate(errors):
                print_info(f'  {i}: {err}')

        print_info('compilation stdout + stderr:')
        for line in output.splitlines():
            print_info(f'  {line}')

    return diagnostics

def _match_diagnostic(exp: DiagnosticExpectation, diag: dict) -> bool:
    if exp.severity != diag['severity']:
        return False
    if exp.code != diag['category']:
        return False

    if exp.lines is not None:
        # jsonl printer outputs 0-indexed lines and line numbers in diags.txt files are 1-indexed
        actual_lines = [r['start']['line'] + 1 for r in diag['span']['ranges']]
        if not all(l in actual_lines for l in exp.lines):
            return False

    return True

def _expand_expectations(diags: list[DiagnosticExpectation]) -> list[DiagnosticExpectation]:
    expanded = []
    for exp in diags:
        if exp.lines:
            for line in exp.lines:
                expanded.append(DiagnosticExpectation(severity=exp.severity, code=exp.code, lines=[line]))
        else:
            expanded.append(exp)
    return expanded

def report_failure(name: str, expected: TestExpectation, actual: TestResult):
    if isinstance(actual, TimedOutResult):
        print_timeout(name)
        print_info(f'  Timed out during {CLR_BOLD}{actual.stage}{CLR_RESET} stage')
        return

    print_fail(name)

    if isinstance(expected, NegativeTestExpectation):
        if actual.stage != 'compilation':
            print_info(f'  Error during {CLR_BOLD}{actual.stage}{CLR_RESET} stage (exitcode {actual.exitcode})')
            return

        diagnostics = _parse_jsonl_diagnostics(actual)
        expanded    = _expand_expectations(expected.diags)

        # missing diagnostics
        for exp in expanded:
            if not any(_match_diagnostic(exp, diag) for diag in diagnostics):
                line_str = ','.join(map(str, exp.lines)) + ':' if exp.lines else ''
                print_info(f'  missing diag: {CLR_BOLD}{line_str}{exp.severity}[{exp.code}]{CLR_RESET}')

        # unexpected diagnostics
        for diag in diagnostics:
            if not any(_match_diagnostic(exp, diag) for exp in expanded):
                actual_lines = [r['start']['line'] + 1 for r in diag['span']['ranges']]
                lines_str = ','.join(map(str, actual_lines))  + ':'
                print_info(f'  unexpected diag: {CLR_BOLD}{lines_str}{diag["severity"]}[{diag["category"]}]{CLR_RESET}')

    elif isinstance(expected, PositiveTestExpectation):
        if actual.stage != 'runtime':
             print_info(f'  Error during {CLR_BOLD}{actual.stage}{CLR_RESET} stage (exitcode {actual.exitcode})')
             return

        if actual.exitcode != expected.exitcode:
            print_info(f'  exitcode: expected {expected.exitcode}, actual {actual.exitcode}')

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

    if isinstance(expected, NegativeTestExpectation):
        if actual.stage != 'compilation':
            return False
        if actual.exitcode == 0:
            return False

        diagnostics = _parse_jsonl_diagnostics(actual)
        expanded    = _expand_expectations(expected.diags)

        # missing diagnostics
        for exp in expanded:
            if not any(_match_diagnostic(exp, diag) for diag in diagnostics):
                return False

        if expected.ignore_unexpected: return True

        # unexpected diagnostics
        for diag in diagnostics:
            if not any(_match_diagnostic(exp, diag) for exp in expanded):
                return False

        return True

    elif isinstance(expected, PositiveTestExpectation):
        if actual.stage != 'runtime':
            return False
        if actual.exitcode != expected.exitcode:
            return False
        if actual.stdout != expected.stdout:
            return False
        if actual.stderr != expected.stderr:
            return False
        return True

    assert False

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
                future = executor.submit(run_test_case, elc_bin, work_dir, path, name, isinstance(expected, NegativeTestExpectation), timeouts)
                future_to_test[future] = (name, expected)

            for future in as_completed(future_to_test):
                name, expected = future_to_test[future]
                handle_result(name, expected, future.result())
    else:
        for path, name, expected in tasks:
            handle_result(name, expected, run_test_case(elc_bin, work_dir, path, name, isinstance(expected, NegativeTestExpectation), timeouts))

    tested_count = passed_count + failed_count + skipped_count
    print(f'[{CLR_BLUE}===={CLR_RESET}] {CLR_BOLD}Synthesis: ', end='')
    print(f'Tested: {CLR_BLUE}{tested_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Passing: {CLR_GREEN}{passed_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Failing: {CLR_RED}{failed_count}{CLR_RESET}{CLR_BOLD} ', end='')
    print(f'| Skipped: {CLR_BLUE}{skipped_count}{CLR_RESET}{CLR_BOLD}', end='')
    print(CLR_RESET)

    return failed_count == 0
