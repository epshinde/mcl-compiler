# Recitation Exercise — Integer Literal Classifier

Standalone C++ program (not part of the [MCL compiler pipeline](../README.md)). Reads whitespace-separated tokens from a file and classifies each numeric-looking token as a decimal, octal, hex (`0x`/`0X`), or binary (`0b`/`0B`) integer literal, validating that its digits actually match the claimed base (e.g. flags `0b12` as an invalid binary literal, `09` as an invalid octal literal).

## Building

```
g++ -std=c++17 -o literal_classifier RA3_E_Shinde.cpp
```

## Running

```
./literal_classifier <input-file>
```

Prints an error for each malformed literal (with its line number) followed by a summary: total lines, word count, and counts of valid decimal/hex/octal/binary literals found.

## Test cases

`infile1` – `infile7` (in this directory) are sample inputs. `RA 3 Test Cases/` holds expected (`.correct`) output covering invalid literals of each base, empty files, unopenable files, and files containing only whitespace or identifiers.
