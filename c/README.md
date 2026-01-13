# Learning C

This repository contains my notes, exercises, and code while learning C programming.

## Structure

```
.
├── antirez-impariamo_il_c/  # Impariamo il C - Salvatore Sanfilippo
│   ├── lezioni/             # Course lessons
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
- [Lezione 1: Introduzione](lezioni/lezione01-introduzione/)
- [Lezione 2: Variabili e operatori](lezioni/lezione02-variabili-operatori/)
  - [Appendice: La vita delle variabili locali](lezioni/lezione02.5-appendice-variabili-locali/)
- [Lezione 3](lezioni/lezione03/)
- [Lezione 4](lezioni/lezione04/)
- [Lezione 5](lezioni/lezione05/)

### Controllo di Flusso e Ricorsione
- [Lezione 6: IF, GOTO e ricorsione](lezioni/lezione06-if-goto-ricorsione/)
- [Lezione 7: Ancora sulla ricorsione, while/for, switch](lezioni/lezione07-ricorsione-while-for-switch/)
- [Lezione 8: Implementiamo il Conway's Game of Life](lezioni/lezione08-game-of-life/)

### Puntatori
- [Lezione 9: Introduzione ai puntatori](lezioni/lezione09-introduzione-puntatori/)
- [Lezione 10: La matematica dei puntatori](lezioni/lezione10-matematica-puntatori/)
- [Lezione 11: Chiarimenti sui puntatori](lezioni/lezione11-chiarimenti-puntatori/)
- [Lezione 12: Un primo incontro con malloc()](lezioni/lezione12-malloc/)
- [Lezione 13: Il trucco dei metadati nascosti](lezioni/lezione13-metadati-nascosti/)

### Strutture Dati
- [Lezione 14: Le strutture del C](lezioni/lezione14-strutture/)
- [Lezione 15: Le struct come mattone delle strutture dati](lezioni/lezione15-struct-strutture-dati/)
- [Lezione 16: Stringhe con reference counting](lezioni/lezione16-struct-stringhe-refcount/)
- [Lezione 17: Design delle stringhe, hexdump()](lezioni/lezione17-stringhe-design-hexdump/)
- [Approfondimento: Reference counting](lezioni/lezione-approfondimento-refcount/)

### Concetti Avanzati
- [Lezione 18: Tipi opachi, typedef, file stdlib](lezioni/lezione18-tipi-opachi-typedef-file/)
- [Lezione 19: Le chiamate di sistema](lezioni/lezione19-chiamate-sistema/)
- [Lezione 20: Buffering della libc e file mappati in memoria](lezioni/lezione20-buffering-mmap/)
- [Lezione 21: Union e bitfield](lezioni/lezione21-union-bitfield/)
- [Lezione 22: I puntatori a funzione](lezioni/lezione22-puntatori-funzione/)
- [Lezione 28: Funzioni con numero variabile di argomenti](lezioni/lezione28-argomenti-variabili/)

### Progetto: Toy Forth
- [Lezione 23: Interprete Toy Forth (parte 1)](lezioni/lezione23-toy-forth-parte1/)
- [Lezione 24: Interprete Toy Forth (parte 2)](lezioni/lezione24-toy-forth-parte2/)
- [Lezione 25: Interprete Toy Forth (parte 3)](lezioni/lezione25-toy-forth-parte3/)
- [Lezione 26: Toy Forth - exec() (parte 4)](lezioni/lezione26-toy-forth-exec-parte4/)
- [Lezione 27: Toy Forth - registrazione funzioni (parte 5)](lezioni/lezione27-toy-forth-registrazione-parte5/)
- [Lezione 29: Toy Forth - primo programma](lezioni/lezione29-toy-forth-primo-programma/)

### Puntate Speciali
- [Come riesumare gli algoritmi dalla memoria](lezioni/lezione-speciale-algoritmi-memoria/)
- [Random variables](lezioni/lezione-speciale-random-variables/)

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
