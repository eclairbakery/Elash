#!/usr/bin/env python

from pathlib import Path
from dataclasses import dataclass
from typing import Literal, Optional
from concurrent.futures import ThreadPoolExecutor, as_completed

import subprocess
import threading
import builtins
import argparse
import difflib
import typing
import sys
import os

ELC_FLAGS = (
    '-O2',
    '-I', 'utils=utils/'
)

# very advanced lock mechanism
PRINT_LOCK = threading.Lock()
def very_advanced_thread_safe_print_function_to_fix_issues_with_parallelism_version_1_0_0(*a, **k):
    with PRINT_LOCK: builtins.print(*a, **k)
print = very_advanced_thread_safe_print_function_to_fix_issues_with_parallelism_version_1_0_0

TestStage = Literal['compilation', 'linking', 'runtime']

@dataclass
class Timeouts:
    compile: float
    link:    float
    runtime: float

@dataclass
class TestExpectation:
    exitcode:    int
    stdout:      str
    stderr:      str
    diags: Optional[list[str]] = None

@dataclass
class FinishedResult:
    exitcode:    int
    stdout:      str
    stderr:      str
    stage:       TestStage

@dataclass
class TimedOutResult:
    stage: TestStage

TestResult = FinishedResult | TimedOutResult

script_dir = Path(__file__).resolve().parent

CLR_BLUE   = '\033[0;34m'
CLR_GREEN  = '\033[0;32m'
CLR_RED    = '\033[0;31m'
CLR_ORANGE = '\033[38;5;166m'
CLR_BOLD   = '\033[0;1m'
CLR_RESET  = '\033[0m'

def eprint(*a, **k):
    return print(*a, **k, file=sys.stderr)

def print_info(msg: str, err: bool = True):
    (eprint if err else print)(f'[{CLR_BLUE}----{CLR_RESET}] {msg}')

def print_pass(name: str):
    print(f'[{CLR_GREEN}PASS{CLR_RESET}] Test passed: {name}')
def print_skip(name: str):
    print(f'[{CLR_BLUE}SKIP{CLR_RESET}] Test skipped: {name}')
def print_fail(name: str):
    eprint(f'[{CLR_RED}FAIL{CLR_RESET}] Test failed: {name}')
def print_timeout(name: str):
    eprint(f'[{CLR_ORANGE}TIME{CLR_RESET}] Test timed out: {name}')

def error(*args):
    eprint(f'{CLR_RED}error: {CLR_RESET}', end='')
    eprint(*args)
    sys.exit(1)

def get_expectation(path: Path, name: str) -> TestExpectation:
    if path.is_file():
        return TestExpectation(exitcode=0, stdout='', stderr='')

    exitcode: int = 0
    if (f := path.joinpath('exitcode.txt')).is_file():
        try:
            exitcode = int(f.read_text())
        except ValueError:
            error(f"ill-formed test case '{name}': {f.joinpath('exitcode.txt')} should contain a valid integer")

    stdout: str = ''
    if (f := path.joinpath('stdout.txt')).is_file():
        stdout = f.read_text().strip()

    stderr: str = ''
    if (f := path.joinpath('stderr.txt')).is_file():
        stderr = f.read_text().strip()

    diags = None
    if (f := path.joinpath('diags.txt')).is_file():
        diags = [line.strip() for line in f.read_text().splitlines() if line.strip()]

    return TestExpectation(exitcode=exitcode, stdout=stdout, stderr=stderr, diags=diags)

def print_diff(expected: str, actual: str, stream_name: str):
    diff = difflib.unified_diff(
        expected.splitlines(keepends=True),
        actual.splitlines(keepends=True),
        fromfile=f'expected {stream_name}',
        tofile=f'actual {stream_name}',
    )
    for line in diff:
        print_info(f'  {line.rstrip()}')

def _run_stage(cmd: list[str], stage: TestStage, timeout: float, cwd: Path | None = None) -> TestResult:
    try:
        res = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=timeout,
            cwd=cwd,
        )
        return FinishedResult(
            exitcode=res.returncode,
            stdout=res.stdout.strip(),
            stderr=res.stderr.strip(),
            stage=stage,
        )
    except subprocess.TimeoutExpired:
        return TimedOutResult(stage=stage)


def resolve_test_paths(path: Path, name: str) -> tuple[Path, Path]:
    if path.is_dir():
        input_file = path / 'input.eu'
        if not input_file.is_file():
            error(f"ill-formed test case '{name}': no input.eu")
        skip_file = path / 'skip'
    else:
        input_file = path
        skip_file = path.parent / 'skip'

    return input_file, skip_file


def run_test_case(
    elc_bin: Path, work_dir: Path,
    path: Path, name: str,
    is_negative: bool,
    timeouts: Timeouts,
) -> TestResult | None:
    input_file, skip_file = resolve_test_paths(path, name)
    if skip_file.is_file():
        return None

    safe_name = name.replace(os.sep, '_').replace('/', '_')
    obj = work_dir / f'{safe_name}.o'
    exe_name = f'{safe_name}.exe' if sys.platform == 'win32' else safe_name
    exe = work_dir / exe_name

    # The compilation
    latest_mtime = max(input_file.stat().st_mtime, elc_bin.stat().st_mtime)
    needs_compile = is_negative or not obj.is_file() or obj.stat().st_mtime < latest_mtime

    if needs_compile:
        cmd = [str(elc_bin), 'compile', str(input_file), '-o', str(obj), *ELC_FLAGS]
        result = _run_stage(cmd, stage='compilation', timeout=timeouts.compile, cwd=script_dir)
        if isinstance(result, TimedOutResult) or result.exitcode != 0:
            return result

    # The linking
    needs_link = not exe.is_file() or exe.stat().st_mtime < obj.stat().st_mtime
    if needs_link:
        cc_cmd = ['cc', '-no-pie', str(obj), '-o', str(exe)]
        if sys.platform == 'win32':
            cc_cmd.append('-mconsole')

        result = _run_stage(cc_cmd, stage='linking', timeout=timeouts.link)
        if isinstance(result, TimedOutResult) or result.exitcode != 0:
            return result

    # The runtime
    return _run_stage([str(exe)], stage='runtime', timeout=timeouts.runtime)

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

class CliArgs(argparse.Namespace):
    elc_bin:         Path
    work_dir:        Path
    parallel:        Optional[int]

    compile_timeout: float
    link_timeout:    float
    runtime_timeout: float

def main():
    parser = argparse.ArgumentParser(description="Elash's end-to-end test runner");
    parser.add_argument('elc_bin',  type=Path, help='Path to elc binary');
    parser.add_argument('work_dir', type=Path, help='Path to working directory');
    parser.add_argument(
        '-j', '--parallel', nargs='?',
        const=None, default=1, type=int,
        help='run in parallel with optional N workers limit'
    );

    parser.add_argument(
        '-t', '--timeout', type=float, default=1.0,
        help='base timeout multiplier'
    )
    parser.add_argument('--compile-timeout', type=float, help='compile stage timeout (default: 1.5s * timeout)')
    parser.add_argument('--link-timeout',    type=float, help='link stage timeout (default: 5s * timeout)')
    parser.add_argument('--runtime-timeout', type=float, help='runtime stage timeout (default: 3s * timeout)')

    #very advanced static typing
    args: CliArgs = typing.cast(CliArgs, parser.parse_args())

    timeouts = Timeouts(
        compile = (args.compile_timeout or 1.5) * args.timeout,
        runtime = (args.runtime_timeout or 3.0) * args.timeout,
        link    = (args.link_timeout    or 5.0) * args.timeout,
    )

    elc_bin  = args.elc_bin.resolve()
    work_dir = args.work_dir.resolve()

    if not work_dir.exists():
        os.makedirs(str(work_dir), exist_ok=True)

    if not run_suite(elc_bin, work_dir, args.parallel, timeouts):
        sys.exit(1)

if __name__ == '__main__':
    main()
