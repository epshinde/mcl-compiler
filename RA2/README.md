# Recitation Exercise — Markup Analyzer

Standalone C++ program (not part of the [MCL compiler pipeline](../README.md)). Reads a text file containing a lightweight markup format and reports statistics on it.

Recognized directives: `%begin` / `%end` (paragraph boundaries), `%bold`, `%italic`, `%underline` (formatting tags), and `#` (comment lines). The program validates that every `%begin` has a matching `%end`, flags unrecognized `%`-directives, and tallies lines, words, paragraphs, and formatting tag counts.

## Building

```
g++ -std=c++17 -o markup_analyzer RA2_E_Shinde.cpp
```

## Running

```
./markup_analyzer
```

Prompts interactively for the input filename, then prints any validation warnings (unmatched `%begin`/`%end`, unrecognized directives) followed by a summary (total lines, commented lines, paragraph count, word count, and bold/italic/underline tag counts).

## Test cases

`infile2` – `infile9` (in this directory) are sample marked-up text files to run the analyzer against. `RA 2 Test Cases/` holds paired `case1` – `case9` inputs with matching expected (`.correct`) output.
