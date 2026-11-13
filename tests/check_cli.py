#!/usr/bin/env python3
"""Check the shared CLI contract after building all three executables."""
import argparse
import math
from pathlib import Path
import re
import subprocess

TARGETS = ("regex_basicmatch", "regex_ReMatch", "regex_ReMatchplus")
CASES = (
    ("literal_positive", "^a$", "a", True),
    ("literal_negative", "^a$", "b", False),
    ("empty", "^a*$", "", True),
    ("star", "^(a*)$", "aa", True),
    ("star_negative", "^a*$", "ab", False),
    ("class", "^[bc]+$", "bcb", True),
    ("class_negative", "^[bc]+$", "bca", False),
    ("backref_positive", r"^(a)\1$", "aa", True),
    ("backref_negative", r"^(a)\1$", "ab", False),
    ("variable_backref_positive", r"^(ab*)\1$", "abbabb", True),
    ("variable_backref_negative", r"^(ab*)\1$", "abbab", False),
    ("empty_backref", r"^(a*)\1$", "", True),
    ("overlap", r"^(a)[bc]*[bc]*\1$", "abcbca", True),
)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--bin-root", type=Path,
                        default=Path(__file__).resolve().parents[1] / "code")
    parser.add_argument("--docker-image",
                        help="Run Linux binaries in this image using Docker.")
    args = parser.parse_args()
    bin_root = args.bin_root.resolve()

    def run(target, arguments):
        if args.docker_image:
            command = ["docker", "run", "--rm", "--network", "none",
                       "--memory", "2g", "-v", f"{bin_root}:/bins:ro",
                       args.docker_image, f"/bins/{target}/match"]
        else:
            command = [str(bin_root / target / "match")]
        return subprocess.run(command + arguments, capture_output=True,
                              text=True, timeout=20)

    checked = 0
    for target in TARGETS:
        for name, pattern, word, expected in CASES:
            result = run(target, [pattern, word])
            context = f"{target}/{name}: {result}"
            assert result.returncode == 0, context
            assert result.stderr == "", context
            assert result.stdout.endswith("\n"), context
            assert len(result.stdout.splitlines()) == 1, context
            fields = result.stdout.split()
            assert len(fields) == 5, context
            assert fields[0] == str(int(expected)), context
            for value in fields[1:]:
                assert value == "NA" or re.fullmatch(r"-?\d+\.\d{6}", value), context
                assert value == "NA" or math.isfinite(float(value)), context

            build, matching, total = fields[1:4]
            assert total != "NA" and float(total) >= 0, context
            if target == "regex_basicmatch" or r"\1" not in pattern:
                assert build == matching == "NA", context
            else:
                assert build != "NA" and matching != "NA", context
                assert float(build) >= 0 and float(matching) >= 0, context
                assert math.isclose(float(build) + float(matching),
                                    float(total), abs_tol=0.000002), context
            checked += 1

        for arguments in ([], ["^a$"], ["^a$", "a", "extra"], ["(", "a"]):
            result = run(target, arguments)
            assert result.returncode != 0, (target, arguments, result)
            assert not result.stdout and result.stderr, (target, arguments, result)
            checked += 1

    print(f"PASS: {checked} CLI checks across {len(TARGETS)} implementations")


if __name__ == "__main__":
    main()
