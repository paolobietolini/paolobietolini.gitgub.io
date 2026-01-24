---
layout: default
title: "Chapter 8: The UNIX System Interface"
permalink: /c/kr-exercises/chapter-08-unix-system-interface
---

# Chapter 8: The UNIX System Interface

Exercises from "The C Programming Language" (2nd Edition) by Brian W. Kernighan and Dennis M. Ritchie.

## Exercises

| # | Description | Source |
|---|-------------|--------|
| 8-01 | Rewrite `cat` using read, write, open, close | - |
| 8-02 | Rewrite `fopen` and `_fillbuf` with fields instead of bit ops | - |
| 8-03 | Redesign `_flushbuf`, `fflush`, `fclose` | - |
| 8-04 | `fseek` for the library | - |
| 8-05 | Modify `fsize` to print other info (mode, links, etc.) | - |
| 8-06 | Implement `calloc` using `malloc` | - |
| 8-07 | `malloc` accepts size request of any type | - |
| 8-08 | `bfree`: free arbitrary block for malloc pool | - |

## Compilation

```bash
gcc -Wall -Wextra -o exercise exercise.c
./exercise
```
