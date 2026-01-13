# Lezione 5 - Stringhe in C

**Video di riferimento:** [Lezione 5 - Stringhe](https://www.youtube.com/watch?v=SWWHqgSwQFw)

---

## 1. Format Specifiers in printf

### 1.1 Come Funziona printf

Riprendiamo il nostro programma "Hello World":

```c
#include <stdio.h>

int main(void) {
    printf("Hello World\n");
    return 0;
}
```

`printf` prende una **stringa di formato** dove:
- Alcuni caratteri vengono stampati **letteralmente**
- Altri sono **segnaposto** (format specifiers) che iniziano con `%`

**Esempio con placeholder:**

```c
#include <stdio.h>

int main(void) {
    printf("Hello World, %d\n", 42);
    return 0;
}
```

**Output:**
```
Hello World, 42
```

Il `%d` indica a `printf`:
- Il **formato** del dato (decimale intero)
- Che c'è **un argomento** dopo la stringa di formato

---

## 2. Caratteri Letterali (char literals)

### 2.1 Apici Singoli vs Doppi

In C, a differenza di Python, **apici singoli e doppi hanno significati diversi**:

| Sintassi | Tipo | Esempio | Valore |
|----------|------|---------|--------|
| `'A'` | `char` (singolo carattere) | `char c = 'A';` | 65 (valore ASCII) |
| `"A"` | `char[]` (stringa) | `char s[] = "A";` | {'A', '\0'} |

**Regola chiave:** Gli apici singoli `''` contengono **esattamente un carattere** e rappresentano il suo valore numerico ASCII.

---

### 2.2 Lo Standard ASCII

Per vedere la tabella ASCII completa su sistemi Unix-like:

```bash
$ man ascii
```

**Esempi di valori ASCII:**

| Carattere | Valore Decimale | Valore Esadecimale |
|-----------|----------------|-------------------|
| `'A'` | 65 | 0x41 |
| `'B'` | 66 | 0x42 |
| `'a'` | 97 | 0x61 |
| `'0'` | 48 | 0x30 |
| `' '` (spazio) | 32 | 0x20 |
| `'\n'` | 10 | 0x0A |
| `'\0'` (NULL) | 0 | 0x00 |

---

### 2.3 char Come Numero Intero

**Concetto fondamentale:** In C, `char` è un **tipo numerico**.

```c
char c = 'A';
printf("%d\n", c);  // stampa 65
printf("%c\n", c);  // stampa A
```

**Viceversa:**

```c
char c = 65;
printf("%c\n", c);  // stampa A
printf("%d\n", c);  // stampa 65
```

**Sono equivalenti!**

---

### 2.4 Mixare Letterali e Numeri

Puoi mischiare valori ASCII e caratteri letterali:

```c
printf("[%c%c%c%c]\n", 65, 66, 67, 68);
// Output: [ABCD]

printf("[%c%c%c%c]\n", 65, 66, 67, 'D');
// Output: [ABCD] (identico!)

printf("[%c%c%c%c]\n", 'A', 'B', 'C', 'D');
// Output: [ABCD] (identico!)
```

Tutte e tre le versioni producono lo stesso risultato perché `'D'` e `68` sono la stessa cosa.

---

### 2.5 Caratteri Fuori ASCII

Se inserisci caratteri non-ASCII (es. caratteri Unicode, emoji), il comportamento dipende dall'encoding:

```c
char c = 'è';  // Potrebbe essere mappato a più byte
```

**Attenzione:** `char` è solo 8 bit. Caratteri Unicode richiedono encoding multi-byte (UTF-8) o tipi più grandi (wide characters).

---

## 3. Array in C

### 3.1 Dichiarazione e Inizializzazione

**Sintassi base:**

```c
int a[] = {10, 5, 50, 100, 7};
```

Le **parentesi graffe** `{}` servono per inizializzare gli elementi dell'array.

**Accesso tramite indici (0-indexed):**

```c
printf("%d %d %d\n", a[0], a[1], a[2]);
// Output: 10 5 50
```

- `a[0]` → primo elemento (10)
- `a[1]` → secondo elemento (5)
- `a[2]` → terzo elemento (50)

---

### 3.2 Dimensione dell'Array

**Esplicita:**
```c
int a[5] = {10, 5, 50, 100, 7};  // Array di 5 elementi
```

**Implicita (dedotta dal compilatore):**
```c
int a[] = {10, 5, 50, 100, 7};  // Dimensione dedotta: 5
```

**Parziale:**
```c
int a[10] = {1, 2, 3};  // Primi 3 inizializzati, altri 7 a zero
```

---

## 4. Stringhe Come Array di char

### 4.1 Rappresentazione Base

**In C, le stringhe sono array di `char` terminati da `\0` (NULL terminator).**

**Metodo 1: Inizializzazione manuale con array**

```c
char str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
printf("%s\n", str);
// Output: Hello
```

**Memoria:**
```
┌───┬───┬───┬───┬───┬───┐
│ H │ e │ l │ l │ o │ \0│
└───┴───┴───┴───┴───┴───┘
 [0] [1] [2] [3] [4] [5]
```

Lo `\0` (o semplicemente `0`) è il **null terminator** - segnala la fine della stringa.

---

### 4.2 Il Null Terminator È Fondamentale

**Cosa succede senza `\0`?**

```c
char str[5] = {'H', 'e', 'l', 'l', 'o'};  // MANCA \0!
printf("%s\n", str);
```

**Possibile output:**
```
Hello???�����
```

`printf` legge i byte **finché non incontra uno `\0`**, quindi continua a leggere memoria **non inizializzata** (garbage) oltre la fine dell'array!

**Visualizzazione:**
```
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ H │ e │ l │ l │ o │ ? │ ? │ ? │ ? │ ? │ \0│
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
                      ↑ garbage values
```

`printf` stamperà tutti i caratteri garbage finché non trova casualmente uno `\0` in memoria.

---

### 4.3 Esempio Pratico del Problema

```c
char a = 10;
char str[] = {'t', 'e', 's', 't'};  // MANCA \0!
printf("%s\n", str);
char b = 20;
```

**Cosa potrebbe stampare:**
- `test` + caratteri garbage
- Il valore di `a` (10) o `b` (20) potrebbero apparire come caratteri strani
- Continua finché trova uno `\0` casuale in memoria

**Non è predicibile! È undefined behavior.**

---

### 4.4 Metodo Semplificato: String Literals

**Metodo 2: Inizializzazione con stringa letterale (raccomandato)**

```c
char str[6] = "Hello";
printf("%s\n", str);
// Output: Hello
```

Il compilatore:
1. Converte automaticamente in `{'H', 'e', 'l', 'l', 'o', '\0'}`
2. Aggiunge il null terminator automaticamente

**Ancora più semplice (dimensione automatica):**

```c
char str[] = "Hello";
printf("%s World\n", str);
// Output: Hello World
```

`sizeof(str)` restituisce `6` (5 caratteri + `\0`).

---

### 4.5 Warning su Dimensioni

**Array troppo piccolo:**
```c
char str[3] = "Hello";  // WARNING!
```

```bash
warning: initializer-string for char array is too long
```

Il compilatore tronca la stringa e **non aggiunge** il null terminator!

**Array troppo grande:**
```c
char str[10] = "Hello";  // OK, nessun warning
```

Gli elementi rimanenti vengono inizializzati a `\0`:
```
┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ H │ e │ l │ l │ o │ \0│ \0│ \0│ \0│ \0│
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
```

---

## 5. Manipolazione di Stringhe

### 5.1 Modificare Singoli Caratteri

Puoi modificare caratteri individuali tramite indici:

```c
char str[] = "Hello";

str[3] = 'p';
printf("%s\n", str);  // Output: Helpo
```

**Usando valori ASCII:**

```c
str[3] = 112;  // 'p' in ASCII è 112
printf("%s\n", str);  // Output: Helpo
```

**Identico!** Ricorda: `char` è un tipo numerico.

---

### 5.2 Incrementare Caratteri

Puoi fare aritmetica sui caratteri:

```c
char str[] = "Hello";

str[3]++;  // 'l' (108) diventa 'm' (109)
printf("%s\n", str);  // Output: Helmo
```
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢰⣟⡶⣭⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⢵⣶⣭⣍⣛⠻⣿⣿⣿⣿⣿⣿⣿⣿⣶⠆⡇⡌⢷⡝⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡙⣷⠰⣍⢿⡈⢿⣿⣿⣿⣿⣿⠿⣣⡾⢁⣿⡆⢳⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⣿⣿⣿⣿⣿⡈⣇⢹⣆⠻⡌⢿⡿⣛⣭⠴⢛⣩⣴⣿⣿⢃⡼⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⡇⢾⣷⣮⡻⢿⣿⡿⣃⡾⢨⣿⡇⢻⠈⣶⢋⣴⣾⣿⣿⣿⡟⣡⠟⢡⣦⡹⣿⣿⣿⣿⡿⠛⡛⢿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣦⠹⡏⠻⡌⢃⡾⢋⣴⣿⣿⡇⢸⣤⠇⣾⣿⣿⣿⣿⣿⢰⠋⠀⣸⠏⢻⡌⢿⣿⡿⣁⡟⡿⢘⣿⣿⣿⣿⣿⣿⣿
⣿⣿⡿⠛⣛⠻⢿⣿⠟⣰⢇⡆⢱⣾⢁⣿⣿⣿⣿⣇⡓⣈⣂⣿⠟⣿⣿⣿⣿⣄⢿⢛⠏⣼⣆⡛⠶⢠⡾⢋⠀⣶⠲⣌⢿⣿⣿⣿⣿⣿
⣿⣿⣷⡈⢿⡿⣇⣁⡾⢃⣾⣇⢸⡇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣏⣴⣿⣿⣿⣿⣿⣷⣶⣾⣿⣿⡇⣼⡟⢰⣿⡀⠿⠀⡙⣧⢹⣿⣿⣿⣿
⣿⣿⣿⣿⠌⣻⠈⡟⢱⣿⣿⣿⣦⡙⠈⣿⡇⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡘⠿⠈⣿⣿⣷⣾⡗⢸⠂⣿⣿⣿⣿
⣿⣿⣿⡟⣸⠃⡆⠇⢸⣿⣿⣿⣿⣿⣿⡟⢸⣿⣿⣿⣿⣿⣿⠿⠟⠛⠿⣿⡿⠿⠿⠿⣿⣿⣿⣿⣿⣿⣇⣿⣿⠟⠉⠶⠋⠀⠈⠿⠿⠛
⣿⡀⠈⠁⠉⠈⠀⠰⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣿⣿⡟⢁⣶⠈⠹⢓⡈⢴⠀⣘⣶⡌⢻⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠲
⡉⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠁⠸⣿⣷⠁⣿⣿⡆⢹⣿⣿⡿⠀⠛⢿⣿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⣤⣤
⣿⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⢿⡿⠃⠀⠈⠁⠀⠀⠀⠀⠈⠿⢿⡿⠃⠀⠀⠀⠀⠀⢰⣷⣍⣿
⣷⣤⡄⢦⠀⠀⠀⠀⠀⠀⠻⣿⣿⣿⡋⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⠁⠀⠀⠀⠀⠀⢀⣼⣿⡿⣿
⣿⣿⢃⣘⢷⣄⠀⠀⠀⠀⠀⠈⠻⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣭⣿⡿⢃⣼
⡿⠟⢸⡟⠳⠞⣃⣄⠀⠀⠀⠀⠀⠘⢿⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⡖⣻⢰
⣇⢻⡘⠻⢦⡙⢿⣿⣧⣄⠀⠀⠀⠀⠀⠈⠙⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⡿⠿⢛⣡⠏⣰
⣿⣌⢷⣌⠳⣶⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⠃⣿⣉⢁⣾⣿
⣿⣿⣷⣝⠳⠌⢻⣿⣿⡝⢿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⣦⡍⢹⢸⣿⣿
⣿⣿⣿⣿⣵⢆⣼⣿⣿⣿⣼⣿⣿⣿⠆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠟⣛⣥⡶⢛⣼⣿⣿


**Altro esempio:**

```c
char c = 'A';
c += 32;  // Conversione maiuscolo → minuscolo
printf("%c\n", c);  // Output: a
```

Questo funziona perché in ASCII:
- `'A'` = 65, `'a'` = 97
- Differenza = 32

---

### 5.3 Implementare printf("%s") Manualmente

Possiamo reimplementare la stampa di stringhe usando `%c`:

```c
char str[] = "Hello";
int i = 0;

while (str[i] != '\0') {  // o semplicemente: while (str[i])
    printf("%c", str[i]);
    i++;
}
printf("\n");
// Output: Hello
```

**Versione più compatta:**

```c
while (str[i] != 0) {  // 0 è equivalente a '\0'
    printf("%c", str[i++]);
}
```

Questo è essenzialmente ciò che fa `printf` con `%s`!

---

## 6. Sottostringa "Gratis"

### 6.1 Il Concetto

In C, ottenere una **sottostringa** è "gratis" (zero costo) perché le stringhe sono solo puntatori a byte in memoria.

**Esempio:**

```c
char *s = "Hello";
s += 2;  // Sposta il puntatore di 2 posizioni
printf("%s\n", s);
// Output: llo
```

**Cosa è successo:**

Prima:
```
┌───┬───┬───┬───┬───┬───┐
│ H │ e │ l │ l │ o │ \0│
└───┴───┴───┴───┴───┴───┘
  ↑
  s punta qui
```

Dopo `s += 2`:
```
┌───┬───┬───┬───┬───┬───┐
│ H │ e │ l │ l │ o │ \0│
└───┴───┴───┴───┴───┴───┘
          ↑
          s punta qui
```

`printf("%s")` parte da dove punta `s` e legge fino a `\0`.

---

### 6.2 Perché È "Gratis"?

**In linguaggi come Python o JavaScript:**
```python
s = "Hello"
sub = s[2:]  # Crea una NUOVA stringa in memoria
```

**In C:**
```c
char *s = "Hello";
s += 2;  // Sposta solo il puntatore (nessuna copia!)
```

Non viene creata alcuna nuova stringa. Stai semplicemente leggendo la stessa memoria da un punto diverso.

**Vantaggi:**
- Zero allocazioni
- Zero copie
- Velocità massima
- Overhead minimo

**Svantaggi:**
- Devi gestire manualmente la memoria
- Facile fare errori (andare oltre i limiti)

---

## 7. Range dei char

### 7.1 char Come Tipo Signed

Le stringhe sono array di `char`, che è un intero a **8 bit**.

**Range di `char` (signed):**
- Minimo: -128
- Massimo: +127

**Range di `unsigned char`:**
- Minimo: 0
- Massimo: 255

---

### 7.2 ASCII e il Valore 0

Lo **standard ASCII** è definito per valori da 0 a 127:
- 0 = `NULL` (il null terminator `\0`)
- 32-126 = caratteri stampabili
- 0-31, 127 = caratteri di controllo (non stampabili)

**Perché `\0` termina le stringhe:**
- È il valore 0
- Non è un carattere stampabile
- È facile da testare (`if (str[i] == 0)` o semplicemente `if (!str[i])`)

---

## 8. Riepilogo Visivo

### 8.1 Rappresentazione in Memoria

```c
char str[] = "Hello";
```

**In memoria:**
```
Indirizzo:  0x1000  0x1001  0x1002  0x1003  0x1004  0x1005
           ┌──────┬──────┬──────┬──────┬──────┬──────┐
Valore:    │  72  │ 101  │ 108  │ 108  │ 111  │  0   │
           │ 'H'  │ 'e'  │ 'l'  │ 'l'  │ 'o'  │ '\0' │
           └──────┴──────┴──────┴──────┴──────┴──────┘
Indice:      [0]    [1]    [2]    [3]    [4]    [5]
```

**sizeof(str)** = 6 byte

---

### 8.2 Confronto Tipi

```c
char c = 'A';        // Singolo carattere, valore 65
char s[] = "A";      // Stringa, {'A', '\0'}, 2 byte
char *p = "Hello";   // Puntatore a stringa letterale
```

**Differenze chiave:**

| Dichiarazione | Tipo | Dimensione | Modificabile? |
|---------------|------|-----------|---------------|
| `char c = 'A'` | Singolo char | 1 byte | Sì |
| `char s[] = "A"` | Array di char | 2 byte | Sì |
| `char *p = "A"` | Puntatore | Dipende (punta a 2 byte) | **No** (undefined behavior) |

---

## 9. Esercizi con le Stringhe

### Esercizio 1: Lunghezza di una Stringa

Scrivi una funzione che calcola la lunghezza di una stringa (senza usare `strlen`):

```c
int string_length(char str[]) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

int main(void) {
    char s[] = "Hello";
    printf("Lunghezza: %d\n", string_length(s));  // Output: 5
    return 0;
}
```

---

### Esercizio 2: Convertire in Maiuscolo

Scrivi una funzione che converte tutti i caratteri minuscoli in maiuscoli:

```c
void to_uppercase(char str[]) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;  // Differenza tra maiuscolo e minuscolo
        }
        i++;
    }
}

int main(void) {
    char s[] = "Hello World";
    to_uppercase(s);
    printf("%s\n", s);  // Output: HELLO WORLD
    return 0;
}
```

---

### Esercizio 3: Invertire una Stringa

Inverti i caratteri di una stringa:

```c
void reverse_string(char str[]) {
    int len = 0;
    // Trova la lunghezza
    while (str[len] != '\0') {
        len++;
    }
    
    // Inverti scambiando caratteri
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int main(void) {
    char s[] = "Hello";
    reverse_string(s);
    printf("%s\n", s);  // Output: olleH
    return 0;
}
```

---

### Esercizio 4: Conta Vocali

Conta il numero di vocali in una stringa:

```c
int count_vowels(char str[]) {
    int count = 0;
    int i = 0;
    
    while (str[i] != '\0') {
        char c = str[i];
        // Converti in minuscolo per semplificare
        if (c >= 'A' && c <= 'Z') {
            c += 32;
        }
        
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        i++;
    }
    
    return count;
}

int main(void) {
    char s[] = "Hello World";
    printf("Vocali: %d\n", count_vowels(s));  // Output: 3
    return 0;
}
```

---

## 10. Approfondimenti

### Link di riferimento

- [C Strings - cppreference](https://en.cppreference.com/w/c/string/byte)
  - Documentazione completa sulle funzioni per stringhe in C
  
- [ASCII Table](https://www.asciitable.com/)
  - Tabella ASCII completa con valori decimali, esadecimali e binari
  
- [String Manipulation in C](https://www.geeksforgeeks.org/strings-in-c/)
  - Tutorial ed esempi sulle stringhe in C

### Spazio per ulteriori approfondimenti


---

## Note Finali

**Punti chiave da ricordare:**

1. **char è un tipo numerico:**
   - `'A'` = 65 in ASCII
   - Puoi fare aritmetica: `'A' + 1 = 'B'`

2. **Stringhe = Array di char + `\0`:**
   - Il null terminator è **obbligatorio**
   - Senza `\0` → undefined behavior, garbage output

3. **Inizializzazione:**
   - `char s[] = "Hello"` → aggiunge `\0` automaticamente
   - `char s[] = {'H', 'e', 'l', 'l', 'o'}` → MANCA `\0`, pericoloso!

4. **Flessibilità e Rischi:**
   - Le stringhe sono "gratis" (puntatori)
   - Ma devi gestire tu la memoria e i limiti
   - Errori comuni: buffer overflow, mancanza di `\0`

5. **printf e format specifiers:**
   - `%s` → stringa (array di char con `\0`)
   - `%c` → singolo carattere
   - `%d` → valore numerico del char

**Le stringhe in C sono semplici ma potenti - sono solo array di byte. Questa semplicità dà velocità ma richiede attenzione ai dettagli!**
