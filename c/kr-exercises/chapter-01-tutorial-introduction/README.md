---
layout: default
title: "Chapter 1: A Tutorial Introduction"
permalink: /c/kr-exercises/chapter-01-tutorial-introduction
---

# Chapter 1: A Tutorial Introduction

Exercises from "The C Programming Language" (2nd Edition) by Brian W. Kernighan and Dennis M. Ritchie.

## Exercises

| # | Description | Source |
|---|-------------|--------|
| 1-01 | Hello, World | [1-01.c](1-01.c) |
| 1-02 | Experiment with escape sequences (`\c`) | [1-02.c](1-02.c) |
| 1-03 | Print heading above temperature conversion table | [1-03.c](1-03.c) |
| 1-04 | Print Celsius to Fahrenheit table | [1-04.c](1-04.c) |
| 1-05 | Print temperature table in reverse order (300 to 0) | [1-05.c](1-05.c) |
| 1-06 | Verify that `getchar() != EOF` is 0 or 1 | [1-06.c](1-06.c) |
| 1-07 | Print the value of EOF | [1-07.c](1-07.c) |
| 1-08 | Count blanks, tabs, and newlines | [1-08.c](1-08.c) |
| 1-09 | Replace multiple blanks with single blank | [1-09.c](1-09.c) |
| 1-10 | Replace tabs with `\t`, backspaces with `\b`, backslashes with `\\` | [1-10.c](1-10.c) |
| 1-11 | Test the word count program | [1-11.c](1-11.c) |
| 1-12 | Print input one word per line | [1-12.c](1-12.c) |
| 1-13 | Print histogram of word lengths (vertical) | [1-13.c](1-13.c) |
| 1-14 | Print histogram of character frequencies | [1-14.c](1-14.c) |
| 1-15 | Rewrite temperature conversion using a function | [1-15.c](1-15.c) |
| 1-16 | Print length of arbitrarily long input lines | [1-16.c](1-16.c) |
| 1-17 | Print all lines longer than 80 characters | [1-17.c](1-17.c) |
| 1-18 | Remove trailing blanks/tabs, delete blank lines | [1-18.c](1-18.c) |
| 1-19 | Reverse input lines | [1-19.c](1-19.c) |
| 1-20 | Detab: replace tabs with spaces | [1-20.c](1-20.c) |
| 1-21 | Entab: replace spaces with tabs | [1-21.c](1-21.c) |
| 1-22 | Fold long lines at word boundaries | [1-22.c](1-22.c) |

## Additional Practice

| # | Description | Source |
|---|-------------|--------|
| 1.6 | Section 1.6 example: count digits, whitespace, others | [1.6-arrays.c](1.6-arrays.c) |
| 1-99 | RLE compression: `aaabbcccc` -> `a3b2c4` | [1-99.c](1-99.c) |
| 1-991 | Expand escape sequences: `\n` -> newline, `\t` -> tab | [1-991.c](1-991.c) |

## Compilation

```bash
gcc -Wall -Wextra -o exercise exercise.c
./exercise
```

Many exercises read from standard input. You can:
- Type input directly (Ctrl+D to end on Unix/Linux)
- Pipe a file: `./exercise < input.txt`
- Use echo: `echo "test input" | ./exercise`
