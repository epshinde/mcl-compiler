# MCL Compiler

A three-stage compiler front end and tree-walking interpreter for **MCL (Mini C-Like Language)**, a small imperative language with C-style syntax. Built from scratch in C++ (no parser/lexer generator tools — hand-written recursive-descent parsing throughout) for a Programming Language Concepts course.

The pipeline is split into three self-contained stages, each building on the previous one:

| Stage | Directory | What it does |
|---|---|---|
| 1. Lexical Analyzer | [`PA1/`](PA1) | Tokenizes MCL source into a stream of typed tokens, with error reporting for malformed input |
| 2. Parser | [`PA2/`](PA2) | Recursive-descent parser that validates MCL programs against the language grammar |
| 3. Interpreter | [`PA3/`](PA3) | Extends the parser with expression evaluation, a typed `Value` system, and statement execution |

Two smaller, standalone recitation exercises are also included:

| Exercise | Directory | What it does |
|---|---|---|
| Markup Analyzer | [`RA2/`](RA2) | Parses a lightweight text markup format and reports formatting/paragraph statistics |
| Literal Classifier | [`RA3/`](RA3) | Scans tokens from a file and classifies/validates decimal, octal, hex, and binary integer literals |

## Language overview

MCL programs declare typed variables, assign and update them, and print results, with `if`/`else` branching and C-style compound statements. Grammar (recursive-descent, one rule per parsing function):

```
Prog      ::= PROGRAM IDENT CompStmt
StmtList  ::= Stmt { Stmt }
Stmt      ::= DeclStmt ; | ControlStmt ; | CompStmt
CompStmt  ::= '{' StmtList '}'
DeclStmt  ::= ( INT | FLOAT | BOOL | CHAR | STRING ) VarList
VarList   ::= Var [ = Expr ] { , Var [ = Expr ] }
ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
IfStmt    ::= IF ( Expr ) Stmt [ ELSE Stmt ]
PrintStmt ::= PRINT ( ExprList )
ExprList  ::= Expr { , Expr }
Expr      ::= LogANDExpr { || LogANDExpr }
LogANDExpr::= EqualExpr { && EqualExpr }
EqualExpr ::= RelExpr [ ( == | != ) RelExpr ]
RelExpr   ::= AddExpr [ ( < | > ) AddExpr ]
AddExpr   ::= MultExpr { ( + | - ) MultExpr }
MultExpr  ::= UnaryExpr { ( * | / | % ) UnaryExpr }
UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
```

Types: `int`, `float`, `bool`, `char`, `string`. Operators: arithmetic (`+ - * / %` and their compound-assignment forms), relational (`< >`), equality (`== !=`), and logical (`&& || !`).

## Building

Each stage is a standalone C++17 program with no external dependencies — only the standard library. Build with `g++` (or any C++17 compiler) directly from each stage's directory; see the README in each directory for the exact file list and example invocations.

## Repository layout

```
PA1/   Lexical analyzer (tokenizer)
PA2/   Recursive-descent parser (syntax checking only)
PA3/   Parser + interpreter (adds evaluation and execution)
RA2/   Text markup analyzer (standalone exercise)
RA3/   Integer literal classifier (standalone exercise)
```

Each directory contains its own test cases (and, where provided, expected `.correct` outputs) alongside the source.

## Notes

This was coursework for a Programming Language Concepts class. Where a stage was seeded from an instructor-provided starter file, that file is kept alongside the completed implementation for reference and is called out in that stage's README.
