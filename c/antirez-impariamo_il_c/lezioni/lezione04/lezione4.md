# Lezione 4 - I Tipi in C: Portabilità e Dimensioni

**Video di riferimento:** [Lezione 4 - I tipi](https://www.youtube.com/watch?v=YNsXyasn4R4)

---

## 1. Portabilità dei Tipi in C

### 1.1 Il Problema delle Dimensioni

A differenza di molti linguaggi moderni, **in C i tipi non hanno dimensioni fisse**.

**Perché?**
Il linguaggio C ha l'ambizione di funzionare su **tutte le piattaforme disponibili**:
- Microcontrollori a 8 bit
- Sistemi embedded a 16 bit
- Desktop e server a 32/64 bit
- Mainframe e supercomputer

Quindi le dimensioni dei tipi **variano in base all'architettura**.

**Esempio:**
- `int` su un sistema a 16 bit → 16 bit
- `int` su un sistema moderno → tipicamente 32 bit (ma non garantito!)
- `int` su alcune architetture specifiche → potrebbe essere 64 bit

Lo standard C garantisce solo **relazioni di grandezza**:
```
sizeof(char) ≤ sizeof(short) ≤ sizeof(int) ≤ sizeof(long) ≤ sizeof(long long)
```

---

### 1.2 Tipi Signed e Unsigned

Se i tipi **non** sono preceduti dalla keyword `unsigned`, possono contenere valori sia positivi che negativi.

**Range per tipi signed:**
- Valori: da -2^(n-1) a +2^(n-1) - 1
- Un bit viene usato per il segno

**Range per tipi unsigned:**
- Valori: da 0 a 2^n - 1
- Tutti i bit sono usati per il valore

**Esempio con 32 bit:**

| Tipo | Range |
|------|-------|
| `int` (signed) | -2,147,483,648 a +2,147,483,647 |
| `unsigned int` | 0 a 4,294,967,295 |

---

## 2. L'Operatore sizeof()

### 2.1 Uso Base

L'operatore `sizeof()` restituisce la dimensione in **byte** di un tipo o variabile:

```c
#include <stdio.h>

int main(void) {
    int x = 5;
    printf("int is %d bytes\n", sizeof(x));
    return 0;
}
```

**Problema:** Questo codice genera un warning!

```bash
warning: format specifies type 'int' but the argument has type 'unsigned long'
```

**Perché?**
`sizeof()` restituisce un tipo `size_t`, che è un `unsigned long` (o simile), non un `int`.

---

### 2.2 Soluzioni al Warning

#### Soluzione 1: Usa il format specifier corretto

```c
printf("int is %lu bytes\n", (unsigned long)sizeof(x));
```

o più semplicemente:

```c
printf("int is %zu bytes\n", sizeof(x));  // %zu è per size_t
```

#### Soluzione 2: Fai il casting

```c
printf("int is %d bytes\n", (int)sizeof(x));
```

**Quale usare?**
- `%zu` è la soluzione moderna e corretta (C99+)
- Il casting funziona ma può perdere informazione su sistemi particolari

---

## 3. Limiti dei Tipi: limits.h

Per conoscere i valori minimi e massimi dei tipi sulla piattaforma corrente, usa la libreria `<limits.h>`:

```c
#include <stdio.h>
#include <limits.h>

int main(void) {
    printf("int min: %d, int max: %d\n", INT_MIN, INT_MAX);
    printf("char min: %d, char max: %d\n", CHAR_MIN, CHAR_MAX);
    printf("long min: %ld, long max: %ld\n", LONG_MIN, LONG_MAX);
    return 0;
}
```

**Costanti principali:**

| Costante | Descrizione |
|----------|-------------|
| `CHAR_MIN`, `CHAR_MAX` | Range di `char` |
| `SHRT_MIN`, `SHRT_MAX` | Range di `short` |
| `INT_MIN`, `INT_MAX` | Range di `int` |
| `LONG_MIN`, `LONG_MAX` | Range di `long` |
| `LLONG_MIN`, `LLONG_MAX` | Range di `long long` |
| `UCHAR_MAX` | Massimo di `unsigned char` |
| `UINT_MAX` | Massimo di `unsigned int` |

**Come funziona?**
Il preprocessore sostituisce queste costanti con i valori effettivi della piattaforma durante la compilazione.

---

## 4. Dimensioni Tipiche dei Tipi

Sebbene non garantite, queste sono le dimensioni più comuni su sistemi moderni:

| Tipo | Dimensione Tipica | Range Tipico (signed) | Note |
|------|-------------------|----------------------|------|
| `char` | 8 bit (1 byte) | -128 a 127 | Sempre 1 byte per definizione |
| `short` | 16 bit (2 byte) | -32,768 a 32,767 | Minimo 16 bit garantito |
| `int` | 32 bit (4 byte) | -2,147,483,648 a 2,147,483,647 | Minimo 16 bit garantito |
| `long` | 32/64 bit | Dipende dalla piattaforma | ≥ 32 bit garantito |
| `long long` | 64 bit (8 byte) | Enorme | Minimo 64 bit garantito (C99+) |

**Note importanti:**

- `char` è **sempre** 1 byte (per definizione dello standard)
- `long` su Linux 64-bit → 64 bit
- `long` su Windows 64-bit → 32 bit (!)
- `int` è tipicamente la "word size" naturale del processore

---

## 5. Il Problema degli Indirizzi di Memoria

### 5.1 Programmi Legacy e Portabilità

Nel passato, alcuni programmatori presumevano di poter salvare un **indirizzo di memoria** in un `int`:

```c
int ptr = (int)&variable;  // Pericoloso!
```

**Questo funzionava** quando:
- Gli indirizzi di memoria erano a 32 bit
- `int` era a 32 bit

**Questo NON funziona più** su sistemi moderni a 64 bit:
- Gli indirizzi di memoria sono a 64 bit
- `int` è ancora tipicamente a 32 bit
- Risultato: **undefined behavior**, crash, corruzione di memoria

### 5.2 La Soluzione: long o Tipi Dedicati

**Su molte piattaforme:**
```c
long ptr = (long)&variable;  // Più sicuro, ma non portabile
```

**Soluzione corretta (prossima sezione):**
```c
intptr_t ptr = (intptr_t)&variable;  // Sempre corretto
```

---

## 6. Tipi a Dimensione Fissa: stdint.h

La libreria `<stdint.h>` (C99) fornisce **tipi con dimensioni garantite**, essenziali per:
- Portabilità
- Comunicazione con hardware
- Formati di file binari
- Networking

### 6.1 Tipi a Dimensione Fissa

**Interi signed:**
```c
int8_t    // 8 bit con segno (-128 a 127)
int16_t   // 16 bit con segno
int32_t   // 32 bit con segno
int64_t   // 64 bit con segno
```

**Interi unsigned:**
```c
uint8_t   // 8 bit senza segno (0 a 255)
uint16_t  // 16 bit senza segno
uint32_t  // 32 bit senza segno
uint64_t  // 64 bit senza segno (0 a 18,446,744,073,709,551,615)
```

**Esempio pratico:**

```c
#include <stdint.h>
#include <stdio.h>

int main(void) {
    uint64_t big_number = 18446744073709551615ULL;  // Massimo uint64_t
    printf("Big number: %llu\n", big_number);
    return 0;
}
```

---

### 6.2 Tipi Speciali per Portabilità

**`size_t`** - Dimensione di un oggetto
- Intero senza segno
- Grande quanto necessario per contenere la dimensione di qualsiasi oggetto
- Ritornato da `sizeof()`
- Format specifier: `%zu`

```c
size_t s = sizeof(int);
printf("Size: %zu\n", s);
```

**`intptr_t`** - Può contenere un puntatore (signed)
- Garantito di poter contenere qualsiasi puntatore convertito a intero
- Utile per manipolazione di puntatori a basso livello

```c
intptr_t ptr = (intptr_t)&variable;
```

**`uintptr_t`** - Può contenere un puntatore (unsigned)
- Come `intptr_t` ma senza segno
- Usato per aritmetica dei puntatori

```c
uintptr_t ptr1 = (uintptr_t)&var1;
uintptr_t ptr2 = (uintptr_t)&var2;
ptrdiff_t diff = (intptr_t)(ptr2 - ptr1);  // Differenza può essere negativa
```

**Perché signed per i puntatori?**
L'aritmetica dei puntatori può produrre valori negativi:
- Un puntatore può essere "prima" di un altro in memoria
- Le differenze tra puntatori possono essere negative

---

### 6.3 Esempio: Prevenire Overflow

`uint64_t` è utile quando vuoi fare calcoli su valori grandi senza overflow:

```c
#include <stdint.h>
#include <stdio.h>

int main(void) {
    // Calcolo: quanti anni posso incrementare un contatore uint64_t
    // prima che vada in overflow, incrementandolo ogni secondo?
    
    uint64_t max_value = 18446744073709551615ULL;
    uint64_t seconds_per_year = 1000000000ULL * 3600 * 24 * 365;
    
    uint64_t years = max_value / seconds_per_year;
    
    printf("Anni prima dell'overflow: %llu\n", years);
    // Output: circa 584,942 anni!
    
    return 0;
}
```

**Quando usare uint64_t:**
- Timestamp in millisecondi
- Contatori che incrementano rapidamente
- Calcoli finanziari (es. centesimi)
- ID univoci
- Hash values

---

## 7. Funzioni e void

### 7.1 void come "Nessun Parametro"

Ritorniamo al nostro `main`:

```c
int main(void) {
    printf("Hello World\n");
    return 0;
}
```

**`void` tra gli argomenti significa:** questa funzione **non riceve parametri di alcun tipo**.

**Attenzione alla differenza:**

```c
int main(void)   // NON accetta parametri (esplicito)
int main()       // Lista parametri non specificata (legacy C, evitare)
```

In C moderno, **sempre specificare `void`** se la funzione non prende parametri.

---

### 7.2 void come Tipo di Ritorno

```c
void clear(void) {
    printf("\033[H\033[2J\033[3J");
}
```

**`void` come tipo di ritorno significa:** questa funzione **non restituisce alcun valore**.

Non devi (e non puoi) scrivere `return` con un valore:

```c
void clear(void) {
    printf("Clearing...\n");
    return;  // OK: return senza valore
    // return 0;  // ERRORE: non puoi restituire un valore
}
```

---

## 8. Escape Sequences e Caratteri Speciali

### 8.1 Come Funziona clear()

Per scrivere una funzione `clear()` che pulisce il terminale, possiamo vedere cosa fa il comando bash `clear`:

```bash
$ clear | hexdump -C
00000000  1b 5b 48 1b 5b 32 4a 1b  5b 33 4a            |.[H.[2J.[3J|
0000000b
```

**Interpretazione:**
- `1b` = 0x1B = 27 decimale = carattere ESC (escape)
- `[H` = muovi cursore in alto a sinistra
- `[2J` = cancella schermo
- `[3J` = cancella buffer scrollback

---

### 8.2 Rappresentare i Caratteri Speciali

Possiamo scrivere questi caratteri usando **escape sequences**:

**Notazione ottale** (prefisso `\0`):
```c
printf("\033[H\033[2J\033[3J");
```
- `\033` = 27 in ottale

**Notazione esadecimale** (prefisso `\x`):
```c
printf("\x1b[H\x1b[2J\x1b[3J");
```
- `\x1b` = 27 in esadecimale (0x1B)

**Entrambe producono lo stesso risultato!**

---

### 8.3 Funzione clear Completa

```c
#include <stdio.h>

void clear(void) {
    printf("\033[H\033[2J\033[3J");
}

int main(void) {
    clear();  // Pulisce il terminale
    printf("Hello World\n");
    return 0;
}
```

---

### 8.4 Escape Sequences Comuni

| Sequence | Descrizione | Valore |
|----------|-------------|--------|
| `\n` | Newline (a capo) | 10 (0x0A) |
| `\t` | Tab orizzontale | 9 (0x09) |
| `\r` | Carriage return | 13 (0x0D) |
| `\\` | Backslash letterale | 92 (0x5C) |
| `\"` | Doppio apice | 34 (0x22) |
| `\'` | Apice singolo | 39 (0x27) |
| `\0` | Carattere null | 0 (0x00) |
| `\033` | ESC (ottale) | 27 (0x1B) |
| `\x1b` | ESC (esadecimale) | 27 (0x1B) |

---

## 9. Buffer di printf e fflush

### 9.1 Come Funziona il Buffer

`printf()` **non stampa immediatamente** sullo schermo. Accumula l'output in un **buffer interno** finché:

1. Incontra un `\n` (newline) → **flush automatico**
2. Il buffer si riempie → **flush automatico**
3. Il programma termina → **flush automatico**
4. Chiami esplicitamente `fflush(stdout)` → **flush manuale**

**Esempio del problema:**

```c
printf("Loading");  // NON viene stampato subito!
sleep(5);           // Attende 5 secondi
printf("...\n");    // ORA stampa tutto insieme
```

**Output (dopo 5 secondi):**
```
Loading...
```

---

### 9.2 Soluzione: Forzare il Flush

**Opzione 1: Aggiungi \n**
```c
printf("Loading\n");  // Stampa immediatamente
sleep(5);
printf("Done!\n");
```

**Opzione 2: Usa fflush()**
```c
printf("Loading");
fflush(stdout);  // Forza la stampa immediata
sleep(5);
printf("...\n");
```

**Quando è importante?**
- Progress bar
- Prompt senza newline: `printf("Enter value: ");`
- Output interattivo
- Debug in tempo reale

---

## 10. Note sulla Sintassi C

### 10.1 Indentazione Non È Importante

A differenza di Python, l'indentazione in C è **puramente estetica**:

```c
// Tutte queste sono equivalenti:

if (x > 0) {
    printf("Positive\n");
}

if (x > 0) { printf("Positive\n"); }

if (x > 0)
{
printf("Positive\n");
}
```

**Importante:** Sono i **punti e virgola** (`;`) e le **parentesi graffe** (`{}`) che definiscono la struttura.

---

### 10.2 Parentesi Graffe Opzionali

Per istruzioni singole, le graffe sono opzionali:

```c
// Con graffe (raccomandato)
if (!x) {
    exit(0);
}

// Senza graffe (compatto)
if (!x) 
    exit(0);

// Tutto su una riga (molto compatto)
if (!x) exit(0);
```

**Raccomandazione:**
- Usa sempre le graffe per codice complesso
- Puoi ometterle solo per istruzioni molto brevi e semplici
- La leggibilità viene prima della brevità

**Pericolo senza graffe:**

```c
if (condition)
    printf("First\n");
    printf("Second\n");  // ATTENZIONE: sempre eseguito!
```

Solo la prima `printf` è nel blocco if!

---

### 10.3 Le Stringhe Sono Array di char

Una stringa in C è un **array di caratteri** terminato da `\0`:

```c
char str[] = "Hello";  // Equivale a: {'H', 'e', 'l', 'l', 'o', '\0'}
```

Ogni carattere è un `char` (1 byte).

**Esempio:**
```c
char str[] = "Hi";
printf("%zu\n", sizeof(str));  // stampa 3 (H, i, \0)
```

---

## 11. Approfondimenti

### Link di riferimento

- [C Data Types - cppreference](https://en.cppreference.com/w/c/language/arithmetic_types)
  - Riferimento completo sui tipi aritmetici in C
  
- [stdint.h Documentation](https://en.cppreference.com/w/c/types/integer)
  - Documentazione completa su tipi a dimensione fissa
  
- [ANSI Escape Codes](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797)
  - Guida completa alle escape sequences per il terminale
  
- [C11 Standard Draft](http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)
  - Standard ufficiale del linguaggio C (versione C11)

### Spazio per ulteriori approfondimenti


---

## Note Finali

**Punti chiave da ricordare:**

1. **Portabilità:**
   - I tipi in C non hanno dimensioni fisse
   - Usa `<limits.h>` per conoscere i limiti sulla tua piattaforma
   - Usa `<stdint.h>` quando hai bisogno di dimensioni garantite

2. **sizeof():**
   - Restituisce `size_t` (non `int`)
   - Usa `%zu` o fai casting a `(int)` per piccoli valori

3. **Tipi a dimensione fissa:**
   - `int32_t`, `uint64_t` → quando la dimensione conta
   - `size_t` → per dimensioni di oggetti
   - `intptr_t`, `uintptr_t` → per puntatori convertiti a interi

4. **printf:**
   - Ha un buffer interno che fa flush su `\n`
   - Usa `fflush(stdout)` per flush manuale
   - Le stringhe sono array di `char` terminati da `\0`

5. **Sintassi:**
   - L'indentazione è solo estetica
   - Le graffe sono opzionali per istruzioni singole (ma usale comunque!)
   - I punti e virgola sono obbligatori