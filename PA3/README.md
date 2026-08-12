# Stage 3 — Interpreter

Extends the [Stage 2 parser](../PA2) into a tree-walking interpreter: every grammar-rule function now also evaluates and returns a `Value`, so parsing and execution happen in the same recursive-descent pass. Declared variables are tracked in a symbol table, `if`/`else` branches execute conditionally, and `print` writes evaluated expressions to stdout.

See the [top-level README](../README.md#language-overview) for the full MCL grammar.

## Files

All source lives in `PA 3 Given Files/`:

| File | Purpose |
|---|---|
| `lex.h` / `lex.cpp` | Same lexer from Stage 1 |
| `val.h` / `val.cpp` | `Value` class — a tagged union over `int`/`float`/`bool`/`char`/`string` with overloaded arithmetic, relational, and logical operators (`+ - * / % == != > < && \|\| !`) that handle type coercion (e.g. int/float mixed arithmetic) |
| `parserInterp.h` | Declarations for every grammar-rule function, now threading a `Value&` for the evaluated result |
| `parserInterp.cpp` | The full parser + interpreter implementation |
| `GivenparserInterpPart.cpp` | The original instructor-provided starter (symbol table setup, `PrintStmt`, `ExprList`) that `parserInterp.cpp` was built out from — kept for reference, **not** part of the build (its symbols duplicate `parserInterp.cpp`) |
| `prog3.cpp` | CLI test driver |

## Building

From inside `PA 3 Given Files/`:

```
g++ -std=c++17 -o interpreter prog3.cpp lex.cpp val.cpp parserInterp.cpp
```

(Do not add `GivenparserInterpPart.cpp` to this command — it duplicates definitions already in `parserInterp.cpp` and will fail to link.)

## Running

```
./interpreter <input-file>
```

Prints `Successful Execution` or `Unsuccessful Interpretation` plus an error count, alongside any output from `print` statements in the program.

## Test cases

`PA 3 Given Files/PA 3 Test Cases/` contains MCL programs exercising declarations, arithmetic, control flow, and print statements, for regression-testing the interpreter.
