
## More Compact Representation of Automata to Defend against ReDoS Attacks



### Compile

```shell
g++ -O2 *.cpp match.o
```

### Run

```shell
./match.o pattern string
```

### Examples

You can first try a regex and a string to run our algorithm. 

For example:

```shell
./match.o "^(a*)$" "aa"
```

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

