from dataclasses import dataclass
from typing import Literal, Optional

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

