
## More Compact Representation of Automata to Match Regular Expressions with Backreferences


### Environment

- Ubuntu/Linux
- C++17 compiler
- Boost development headers


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

For ReMatch or basicMatch, use the same compilation command in directory 
`code/regex_ReMatch` or `code/regex_basicmatch`, respectively.

### Run

```shell
./match 'pattern' 'string'
```

### Examples

```shell
./match '^([ab]+)c\1$' 'ababababababababababababababababababababababababababababababababcabababababababababababababababababababababababababababababababab'
```

```output
1 0.791000 0.459000 1.250000 0.761719
```

`1` indicates a match; it takes 0.791ms to construct the range-position automaton and 0.459ms to match the string using the automaton. Overall, it takes 1.25ms and 0.761719MB memory to match the given string.

**Non-matching input**

```shell
./match '^([ab]+)c\1$' 'ababababababababababababababababababababababababababababababababcabababababababababababababababababababababababababababababababaa'
```

```text
0 0.819000 0.663000 1.482000 NA
```

`0` indicates no match; `NA` indicates that the memory usage is negligible.

## Supported Syntax

| Feature | Description | Examples |
| --- | --- | --- |
| ASCII literals | Match the specified character. | `a`, `1`, `_` |
| Dot (`.`) | Match any character except newline. | `a.b` |
| Star (`*`) | Repeat the preceding element zero or more times. | `a*` matches the empty string, `a`, or `aa`. |
| Plus (`+`) | Repeat the preceding element one or more times. | `a+` matches `a`, `aa`, or `aaa`. |
| Question mark (`?`) | Match the preceding element zero or one time. | `a?` matches the empty string or `a`. |
| Character classes (`[...]`) | Match one character from a set or range. | `[abc]`, `[a-z]`, `[0-9]` |
| Negated character classes (`[^...]`) | Match one character outside a set or range. | `[^abc]`, `[^a-z]` |
| Digit (`\d`) | Match an ASCII digit. | `\d` matches `5`. |
| Non-digit (`\D`) | Match a character other than an ASCII digit. | `\D` matches `a`. |
| Word character (`\w`) | Match an ASCII letter, digit, or underscore. | `\w` matches `a`, `1`, or `_`. |
| Non-word character (`\W`) | Match a character outside the word character class. | `\W` matches `!`. |
| Whitespace (`\s`) | Match a whitespace character. | `\s` matches a space or tab. |
| Non-whitespace (`\S`) | Match a non-whitespace character. | `\S` matches `a`. |
| Capturing group (`(...)`) | Group an expression and capture its matched text. | `(abc)` |
| Non-capturing group (`(?:...)`) | Group an expression without capturing its matched text. | `(?:abc)` |
| Backreference | Match the text captured by a referenced group. | `(.)\1` matches `aa`, but not `ab`. |
| Positive lookahead (`(?=...)`) | Require the following text to match the assertion without consuming it. | `a(?=b)` |
| Negative lookahead (`(?!...)`) | Require the following text not to match the assertion without consuming it. | `a(?!b)` |
| Positive lookbehind (`(?<=...)`) | Require the preceding text to match the assertion without consuming it. | `(?<=b)a` |
| Negative lookbehind (`(?<!...)`) | Require the preceding text not to match the assertion without consuming it. | `(?<!b)a` |

The lookaround examples above contain no capturing groups or backreferences
inside the assertions.

## Experiment Versions

| Baseline                   | Version     | Year  |
|----------------------------|-------------|-------|
| PCRE                       | PCRE2-10.42 | 2022  |
| Boost Xpressive            | 1.74.0      | 2020  |
| ICU regex                  | 66.1        | 2020  |
| Python regex engine        | 3.10.12     | 2023  |
| Snort3 Community Rules     | 3.3.0.0     | 2024  |
| ClamAV Virus Database      | 62          | 2023  |
