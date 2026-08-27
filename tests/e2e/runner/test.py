import subprocess
import typing
import sys
import os
import re

from pathlib import Path

from .models  import *
from .console import *
from .defs    import *

ELC_FLAGS = (
    '-O2',
    '-I', 'utils=utils/'
)

def _parse_diag(line: str, ttype: TestType) -> DiagExpectation:
    # sev[code]: message
    message = None
    if line.count(':') >= 2:
        parts = line.rsplit(':', 1)
        line = parts[0]
        message = parts[1].strip()

    parts = line.split(':')
    if len(parts) == 1:
        # sev[code]
        content = parts[0]
        lines = None
    else:
        # 1,2,3:sev[code]
        lines = [int(l) for l in parts[0].split(',')]
        content = parts[1]

    match = re.match(r'(\w+)(?:\[(.+)\])?', content)
    if not match:
        error(f"ill-formed diagnostic: {line}")

    assert match != None # to make pyright happy

    severity, code = match.groups()
    if severity == 'warn': severity = 'warning'
    if severity not in ('error', 'warning', 'note'):
        error(f"ill-formed diagnostic: unknown severity '{severity}'")

    if ttype == 'positive' and severity == 'error':
        error("ill-formed diagnostic: 'error' not allowed in positive test cases")

    return DiagExpectation(
        severity=typing.cast(Severity, severity), code=code, lines=lines, message=message
    )

def get_expectation(test: TestCase) -> TestExpectation:
    name, path = test.name, test.path

    if path.is_file():
        if test.type == 'negative':
            error(f"ill-formed test '{name}': negative cases must be directories")
        return PositiveTestExpectation(exitcode=0, stdout='', stderr='', diags=[])

    lines, diags = [], []
    if (f := path.joinpath('diags.txt')).is_file():
        lines = [line.strip() for line in f.read_text().splitlines() if line.strip()]
        diags = [_parse_diag(line, test.type) for line in lines if line != '...']

    if test.type == 'negative':
        return NegativeTestExpectation(diags=diags, ignore_unexpected='...' in lines)

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

    return PositiveTestExpectation(exitcode=exitcode, stdout=stdout, stderr=stderr, diags=diags)

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


def resolve_test_paths(path: Path, name: str) -> tuple[list[Path], Optional[Path]]:
    if path.is_dir():
        input_files = sorted([f for f in path.iterdir() if f.suffix == '.eu'])
        if not input_files:
            error(f"ill-formed test case '{name}': no .eu files")
        skip_file = path / 'skip'
    else:
        input_files = [path]
        skip_file   = None

    return input_files, skip_file

def run_test_case(elc_bin: Path, work_dir: Path, case: TestCase, timeouts: Timeouts, elc_bin_mtime: float) -> TestResult | None:
    path, name = case.path, case.name
    is_negative = case.type == 'negative'

    input_files, skip_file = resolve_test_paths(path, name)
    if skip_file is not None and skip_file.is_file():
        return None

    test_dir = work_dir / name
    test_dir.mkdir(parents=True, exist_ok=True)

    obj_files = [test_dir / f"{f.stem}.o" for f in input_files]
    exe_name = "output.exe" if sys.platform == 'win32' else "output"
    exe = test_dir / exe_name

    # The compilation
    has_diags_file = path.is_dir() and path.joinpath('diags.txt').is_file()
    use_jsonl = is_negative or has_diags_file

    latest_mtime = elc_bin_mtime
    for f in input_files:
        latest_mtime = max(latest_mtime, f.stat().st_mtime)

    needs_compile = is_negative or has_diags_file or not all(o.is_file() and o.stat().st_mtime >= latest_mtime for o in obj_files)

    compilation_result = None
    if needs_compile:
        for f, o in zip(input_files, obj_files):
            cmd = [str(elc_bin), 'compile', str(f), '-o', str(o), *ELC_FLAGS]
            if use_jsonl:
                cmd.append('--jsonl')
            result = _run_stage(cmd, stage='compilation', timeout=timeouts.compile, cwd=script_dir)
            if isinstance(result, FinishedResult):
                result.jsonl = use_jsonl
            if isinstance(result, TimedOutResult) or result.exitcode != 0:
                return result
            if has_diags_file:
                compilation_result = result

    # The linking
    needs_link = not exe.is_file() or any(exe.stat().st_mtime < o.stat().st_mtime for o in obj_files)
    if needs_link:
        cc_cmd = ['cc', '-no-pie', *[str(o) for o in obj_files], '-o', str(exe)]
        if sys.platform == 'win32':
            cc_cmd.append('-mconsole')

        result = _run_stage(cc_cmd, stage='linking', timeout=timeouts.link)
        if isinstance(result, TimedOutResult) or result.exitcode != 0:
            return result

    # The runtime
    runtime_result = _run_stage([str(exe)], stage='runtime', timeout=timeouts.runtime)
    if isinstance(runtime_result, FinishedResult) and compilation_result is not None:
        runtime_result.compilation_result = compilation_result
    return runtime_result
