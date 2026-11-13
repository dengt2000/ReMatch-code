
## More Compact Representation of Automata to Match Regular Expressions with Backreferences


### Environment

- Ubuntu/Linux
- C++17 compiler
- Boost development headers

Tested with Ubuntu 24.04 (ARM64), GCC 13.3.0, and Boost 1.83.

```shell
sudo apt-get update
sudo apt-get install -y build-essential libboost-dev
```

### Compile

From the repository root, build ReMatch+:

```shell
cd code/regex_ReMatchplus
g++ -std=c++17 -O2 *.cpp -o match
```

For ReMatch or basicMatch, use the same compilation command in
`code/regex_ReMatch` or `code/regex_basicmatch`, respectively.

### Run

```shell
./match 'pattern' 'string'
```

### Examples

```shell
./match '^(a*)$' 'aa'
./match '^(a)\1$' 'aa'  # match result: 1
./match '^(a)\1$' 'ab'  # match result: 0
```

All three implementations print one line with five space-separated fields:

```text
match_result build_ms match_ms total_ms memory_delta_mb
```

| Field | Meaning |
| --- | --- |
| `match_result` | `1` for a match, `0` otherwise. |
| `build_ms` | CPU time for initial automaton construction (ms). |
| `match_ms` | Remaining CPU time in the matching call, including cleanup (ms). |
| `total_ms` | Total CPU time for the matching call, including construction (ms). |
| `memory_delta_mb` | Change in resident memory (`VmRSS`), in MiB. |

Unavailable measurements are printed as `NA`. basicMatch reports `NA` for both
phase times because construction and matching are combined. ReMatch and
ReMatch+ also report `NA` for both phase times when there is no backreference.
When phase times are available, their sum equals `total_ms` up to rounding.

For `./match '^(a*)$' 'aa'`, an example output is:

```text
1 NA NA 0.012000 0.000000
```

The first field indicates a match. Timing and memory values vary between runs;
the memory field measures a change in RSS, not peak memory. Diagnostics go to
standard error. A successful invocation returns exit code `0` for either match
result; missing or extra arguments return `2`.

## Support Syntax

| **Feature**                        | **Description**                                              | **Example**                                       |
| ---------------------------------- | ------------------------------------------------------------ | ------------------------------------------------- |
| **ASCII Literal Characters**       | ASCII Characters are matched as they are (e.g., `a` matches the character `a`). | `a`, `1`, `.`                                     |
| **Dot (.)**                        | Matches any character except newline (`\n`).                 | `a.b` matches `acb`, `a1b`                        |
| **Star (\*)**                      | Matches the preceding element zero or more times.            | `a*` matches `, `a`, `aa`                         |
| **Plus (+)**                       | Matches the preceding element one or more times.             | `a+` matches `a`, `aa`, `aaa`                     |
| **Question Mark (?)**              | Matches the preceding element zero or one time (optional).   | `a?` matches `, `a`                               |
| **Character Classes([])**          | Defines a character class, matching any of the characters inside the brackets. Can include ranges. | `[a-z]`, `[0-9]`, `[abc]`                         |
| **Negated Character Classes([^])** | Defines a character class, matching any of the characters not inside the brackets. Can include ranges. | `[^a-z]`, `[^0-9]`, `[^abc]`                      |
| **Digit (\d)**                     | Matches any digit, equivalent to `[0-9]`.                    | `\d` matches `1`, `5`                             |
| **Non-Digit (\D)**                 | Matches any non-digit, equivalent to `[^0-9]`.               | `\D` matches a,&                                  |
| **Word Character (\w)**            | Matches any alphanumeric character or underscore, equivalent to `[a-zA-Z0-9_]`. | `\w` matches `a`, `1`, `_`                        |
| **Non-Word Character (\W)**        | Matches any  non-word characters                             | `\w` matches `\t`                                 |
| **Whitespace (\s)**                | Matches any whitespace character (space, tab, newline, etc.). | `\s` matches  `\t`                                |
| **Whitespace (\S)**                | Matches any non-whitespace character a                       | `\sS matches  `\a,1,_`                            |
| **Capture Group (())**             | Groups expressions together and captures the match.          | `(abc)` captures `abc`                            |
| **Non-capture Group (?:...)**      | Groups expressions without capturing the match.              | `(?:abc)` does not capture `abc`                  |
| **Backreferences**                 | Backreferences match the same text as previously matched by a capturing group. | (.)\\1 matches aa but does not match ab           |
| **Positive Lookahead (?=...)**     | Asserts that a match is followed by a certain pattern (without including it in the match).capturing groups and backreferences are not supported within lookahead assertion | `a(?=b)` matches `a` only if followed by `b`      |
| **Negative Lookahead (?!...)**     | Asserts that a match is not followed by a certain pattern.Asserts that a match is followed by a certain pattern (without including it in the match).capturing groups and backreferences are not supported within lookahead assertion | `a(?!b)` matches `a` only if not followed by `b`  |
| **Positive Lookbehind(?<=...)**    | Asserts that a match is preceded by a certain pattern (without including it in the match).Asserts that a match is followed by a certain pattern (without including it in the match).capturing groups and backreferences are not supported within lookbehind assertion | `a(?<=b)` matches `a` only if preceded by `b`     |
| **Negative Lookehind (?<!...)**    | Asserts that a match is not preceded by a certain pattern.Asserts that a match is followed by a certain pattern (without including it in the match).capturing groups and backreferences are not supported within lookbehind assertion | `a(<?!b)` matches `a` only if not preceded by `b` |

## Version
| Baseline                   | Version     | Year  |
|----------------------------|-------------|-------|
| PCRE                       | PCRE2-10.42 | 2022  |
| Boost Xpressive            | 1.74.0      | 2020  |
| Icu regex                  | 66.1        | 2020  |
| Python regex engine        | 3.10.12     | 2023  |
| Snort3 Community Rules     | 3.3.0.0     | 2024  |
| ClamAV Virus Database      | 62          | 2023  |
