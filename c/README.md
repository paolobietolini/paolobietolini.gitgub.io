# Learning C

This repository contains my notes, exercises, and code while learning C programming.

## Structure

```
.
├── antirez-impariamo_il_c/  # Impariamo il C - Salvatore Sanfilippo
│   ├── antirez-impariamo_il_c/lezioni/             # Course lessons
│   └── esercizi/            # My exercises for the course
├── kr-exercises/            # K&R (The C Programming Language) exercises
│   ├── chapter-01-tutorial-introduction/
│   ├── chapter-02-types-operators-expressions/
│   ├── chapter-03-control-flow/
│   ├── chapter-04-functions-program-structure/
│   ├── chapter-05-pointers-arrays/
│   ├── chapter-06-structures/
│   ├── chapter-07-input-output/
│   └── chapter-08-unix-system-interface/
└── README.md
```

---

## Antirez Course - Impariamo il C

Course by Salvatore Sanfilippo (antirez).

### Fondamenti
- [Lezione 1: Introduzione](lezioni/introduzione)
- [Lezione 2: Variabili e operatori](antirez-impariamo_il_c/lezioni/lezione02-variabili-operatori/)
  - [Appendice: La vita delle variabili locali](antirez-impariamo_il_c/lezioni/lezione02.5-appendice-variabili-locali/)
- [Lezione 3](antirez-impariamo_il_c/lezioni/lezione03/)
- [Lezione 4](antirez-impariamo_il_c/lezioni/lezione04/)
- [Lezione 5](antirez-impariamo_il_c/lezioni/lezione05/)

### Controllo di Flusso e Ricorsione
- [Lezione 6: IF, GOTO e ricorsione](antirez-impariamo_il_c/lezioni/lezione06-if-goto-ricorsione/)
- [Lezione 7: Ancora sulla ricorsione, while/for, switch](antirez-impariamo_il_c/lezioni/lezione07-ricorsione-while-for-switch/)
- [Lezione 8: Implementiamo il Conway's Game of Life](antirez-impariamo_il_c/lezioni/lezione08-game-of-life/)

### Puntatori
- [Lezione 9: Introduzione ai puntatori](antirez-impariamo_il_c/lezioni/lezione09-introduzione-puntatori/)
- [Lezione 10: La matematica dei puntatori](antirez-impariamo_il_c/lezioni/lezione10-matematica-puntatori/)
- [Lezione 11: Chiarimenti sui puntatori](antirez-impariamo_il_c/lezioni/lezione11-chiarimenti-puntatori/)
- [Lezione 12: Un primo incontro con malloc()](antirez-impariamo_il_c/lezioni/lezione12-malloc/)
- [Lezione 13: Il trucco dei metadati nascosti](antirez-impariamo_il_c/lezioni/lezione13-metadati-nascosti/)

### Strutture Dati
- [Lezione 14: Le strutture del C](antirez-impariamo_il_c/lezioni/lezione14-strutture/)
- [Lezione 15: Le struct come mattone delle strutture dati](antirez-impariamo_il_c/lezioni/lezione15-struct-strutture-dati/)
- [Lezione 16: Stringhe con reference counting](antirez-impariamo_il_c/lezioni/lezione16-struct-stringhe-refcount/)
- [Lezione 17: Design delle stringhe, hexdump()](antirez-impariamo_il_c/lezioni/lezione17-stringhe-design-hexdump/)
- [Approfondimento: Reference counting](antirez-impariamo_il_c/lezioni/lezione-approfondimento-refcount/)

### Concetti Avanzati
- [Lezione 18: Tipi opachi, typedef, file stdlib](antirez-impariamo_il_c/lezioni/lezione18-tipi-opachi-typedef-file/)
- [Lezione 19: Le chiamate di sistema](antirez-impariamo_il_c/lezioni/lezione19-chiamate-sistema/)
- [Lezione 20: Buffering della libc e file mappati in memoria](antirez-impariamo_il_c/lezioni/lezione20-buffering-mmap/)
- [Lezione 21: Union e bitfield](antirez-impariamo_il_c/lezioni/lezione21-union-bitfield/)
- [Lezione 22: I puntatori a funzione](antirez-impariamo_il_c/lezioni/lezione22-puntatori-funzione/)
- [Lezione 28: Funzioni con numero variabile di argomenti](antirez-impariamo_il_c/lezioni/lezione28-argomenti-variabili/)

### Progetto: Toy Forth
- [Lezione 23: Interprete Toy Forth (parte 1)](antirez-impariamo_il_c/lezioni/lezione23-toy-forth-parte1/)
- [Lezione 24: Interprete Toy Forth (parte 2)](antirez-impariamo_il_c/lezioni/lezione24-toy-forth-parte2/)
- [Lezione 25: Interprete Toy Forth (parte 3)](antirez-impariamo_il_c/lezioni/lezione25-toy-forth-parte3/)
- [Lezione 26: Toy Forth - exec() (parte 4)](antirez-impariamo_il_c/lezioni/lezione26-toy-forth-exec-parte4/)
- [Lezione 27: Toy Forth - registrazione funzioni (parte 5)](antirez-impariamo_il_c/lezioni/lezione27-toy-forth-registrazione-parte5/)
- [Lezione 29: Toy Forth - primo programma](antirez-impariamo_il_c/lezioni/lezione29-toy-forth-primo-programma/)

### Puntate Speciali
- [Come riesumare gli algoritmi dalla memoria](antirez-impariamo_il_c/lezioni/lezione-speciale-algoritmi-memoria/)
- [Random variables](antirez-impariamo_il_c/lezioni/lezione-speciale-random-variables/)

---

## K&R Exercises

Exercises from "The C Programming Language" by Kernighan and Ritchie.

- [Chapter 1: A Tutorial Introduction](kr-exercises/chapter-01-tutorial-introduction/)
- [Chapter 2: Types, Operators and Expressions](kr-exercises/chapter-02-types-operators-expressions/)
- [Chapter 3: Control Flow](kr-exercises/chapter-03-control-flow/)
- [Chapter 4: Functions and Program Structure](kr-exercises/chapter-04-functions-program-structure/)
- [Chapter 5: Pointers and Arrays](kr-exercises/chapter-05-pointers-arrays/)
- [Chapter 6: Structures](kr-exercises/chapter-06-structures/)
- [Chapter 7: Input and Output](kr-exercises/chapter-07-input-output/)
- [Chapter 8: The UNIX System Interface](kr-exercises/chapter-08-unix-system-interface/)

---

## Compilation

```bash
gcc -o program program.c
./program
```

With warnings enabled (recommended):
```bash
gcc -Wall -Wextra -o program program.c
```
