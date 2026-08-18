from dataclasses import dataclass
from typing import Literal, Optional

TestStage = Literal['compilation', 'linking', 'runtime']
Severity  = Literal['error', 'warning', 'note']

@dataclass
class Timeouts:
    compile: float
    link:    float
    runtime: float

@dataclass
class DiagnosticExpectation:
    severity: Severity
    code:     str
    lines:    Optional[list[int]]


@dataclass
class PositiveTestExpectation:
    exitcode:    int
    stdout:      str
    stderr:      str

@dataclass
class NegativeTestExpectation:
    diags: list[DiagnosticExpectation]
    ignore_unexpected: bool = False

TestExpectation = PositiveTestExpectation | NegativeTestExpectation


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

