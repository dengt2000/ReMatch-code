
## More Compact Representation of Automata to Match Regular Expressions with Backreferences


### Environment requirements (Ubuntu/Linux)

The command-line programs target Linux: they include `<sys/sysinfo.h>` and use
`/proc/<pid>/status` for memory measurements. Native macOS and Windows builds
require platform-specific changes.

Required build dependencies:

- A C++17-capable compiler (`g++` on Ubuntu).
- Standard C/C++ development headers and tools (`build-essential`).
- Boost development headers (`libboost-dev`) for ReMatch and ReMatch+.
  The code uses the header-only `boost::dynamic_bitset`; no Boost linker flags
  are needed.

Verified environment: Ubuntu 24.04 (ARM64 container), GCC 13.3.0, and Boost
1.83. All three implementations compile with the command below and pass the
literal and backreference positive/negative smoke tests.

On Ubuntu, install the dependencies with:

```shell
sudo apt-get update
sudo apt-get install -y build-essential libboost-dev
```

### Compile

Run the following from the repository root to build ReMatch+:

```shell
cd code/regex_ReMatchplus
g++ -std=c++17 -O2 *.cpp -o match
```

To build the other implementations, run the same `g++` command inside
`code/regex_ReMatch` (ReMatch) or `code/regex_basicmatch` (basicMatch).
Each directory is a separate executable; do not compile all three directories
together. The `-o match` option names the output executable.

If compilation reports `boost/dynamic_bitset.hpp: No such file or directory`,
install `libboost-dev`. If it reports missing `sys/sysinfo.h`, check that the
build is running in Linux with the development headers installed.

### Run

From the directory containing the compiled executable, supply both a pattern
and an input string:

```shell
./match 'pattern' 'string'
```

Use single quotes around patterns so that the shell preserves backslashes and
other regular-expression characters. Always provide both arguments.

### Examples

```shell
./match '^(a*)$' 'aa'
./match '^(a)\1$' 'aa'
./match '^(a)\1$' 'ab'
```

The second example should match, while the third should not.

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
