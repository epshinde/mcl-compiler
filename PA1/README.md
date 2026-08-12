# Stage 1 — Lexical Analyzer

Hand-written lexer for MCL. Reads source character-by-character and produces a stream of `LexItem` tokens (type, lexeme text, line number), with dedicated error tokens for malformed identifiers, unterminated strings, unrecognized symbols, and other bad input.

## Files

| File | Purpose |
|---|---|
| `lex.h` | Token enum (`Token`) and the `LexItem` class |
| `lex.cpp` | Tokenizer implementation — `getNextToken()`, keyword recognition, string/char/number scanning |
| `main.cpp` | CLI test driver that runs the lexer over a file and reports token statistics |
| `test.cpp` | Ad hoc scratch file used while debugging file I/O; not part of the lexer itself |

Recognized tokens include the MCL keywords (`if`, `else`, `print`, `int`, `float`, `char`, `string`, `bool`, `program`, `true`, `false`), identifiers, integer/real/string/char/boolean constants, arithmetic/relational/logical operators, compound-assignment operators (`+= -= *= /= %=`), and delimiters.

## Building

```
g++ -std=c++17 -o lexer main.cpp lex.cpp
```

## Running

```
./lexer <input-file> [flags]
```

Flags (combine as needed):

| Flag | Effect |
|---|---|
| `-all` | Print every token as it's scanned |
| `-num` | Report numeric constants |
| `-str` | Report string constants |
| `-char` | Report char constants |
| `-id` | Report identifiers |
| `-kw` | Report keywords |
| `-bool` | Report boolean constants |

With no flags, the driver just prints summary counts (total tokens, identifiers, numerics, booleans, strings/chars).

## Test cases

The top-level files (`comments`, `constants`, `idents`, `integers`, `validops`, `invchar1`, `invstr1`, `badarg`, ...) are individual MCL source snippets, several deliberately malformed to exercise error handling (invalid characters, invalid strings, invalid symbols). `PA 1 Test Cases/` holds the matching expected (`.correct`) output for each. `Lextest` is an instructor-provided shell script for running the test suite in the course's lab environment; it's environment-specific and not needed to build or run the lexer.
