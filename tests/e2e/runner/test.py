import subprocess
import sys
import os

from pathlib import Path

from .models  import *
from .console import *
from .defs    import *

ELC_FLAGS = (
    '-O2',
    '-I', 'utils=utils/'
)

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
