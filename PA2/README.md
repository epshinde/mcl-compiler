# Stage 2 — Recursive-Descent Parser

Extends the [Stage 1 lexer](../PA1) with a recursive-descent parser: one function per grammar rule (`Prog`, `StmtList`, `DeclStmt`, `IfStmt`, `Expr`, `RelExpr`, `PrimaryExpr`, ...), each consuming tokens and reporting syntax errors with line numbers. This stage validates program structure only — it does not evaluate expressions or execute statements (see [Stage 3](../PA3) for that).

See the [top-level README](../README.md#language-overview) for the full MCL grammar this parser implements.

## Files

| File | Purpose |
|---|---|
| `lex.h` / `lex.cpp` | Same lexer from Stage 1, reused as-is |
| `parser.h` | Declarations for every grammar-rule function |
| `GivenParserPart.cpp` | The parser implementation (built out from an instructor-provided starter file of the same name — the file was kept rather than renamed) |
| `prog2.cpp` | CLI test driver |

## Building

```
g++ -std=c++17 -o parser prog2.cpp lex.cpp GivenParserPart.cpp
```

## Running

```
./parser <input-file>
```

Prints `Successful Parsing` or `Unsuccessful Parsing` plus a syntax error count; each individual syntax error is reported with its line number as it's encountered.

## Test cases

`PA 2 Test Cases/` contains numbered MCL programs (`testprog1` ... `testprog19`) with matching `.correct` files holding the expected parser output, for regression-testing changes to the grammar implementation.
