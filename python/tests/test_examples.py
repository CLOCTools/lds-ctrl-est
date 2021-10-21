import pathlib
import runpy
import pytest

# up just one level from file gets you to containing dir (tests)
# so we go up twice
examples = list(pathlib.Path(__file__, "..", "..", "examples").resolve().glob("*.py"))


@pytest.mark.parametrize("example", examples, ids=lambda example: example.stem)
def test_example(example):
    try:
        runpy.run_path(example)
    except SystemExit:  # allow for early termination of scripts
        pass