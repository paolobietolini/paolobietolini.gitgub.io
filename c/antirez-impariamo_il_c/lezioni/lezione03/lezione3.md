# Lezione 3 - Variabili, Ambito, Passaggio per Valore e Tipi di Dato nel C

**Video di riferimento:** [Lezione 3 - Variabili Globali](https://www.youtube.com/watch?v=mw4gUqsGPZw)

---

## 1. Ambito delle Variabili (Scope)

### 1.1 Variabili Locali

Le **variabili locali** sono dichiarate all'interno di una funzione e:
- Esistono solo durante l'esecuzione della funzione
- Vengono create all'ingresso della funzione
- Vengono distrutte all'uscita dalla funzione
- Non mantengono il loro valore tra chiamate successive

**Esempio:**

```c
void incr(void) {
    int x = 1;
    x = x + 1;
    printf("%d\n", x);
}

int main(void) {
    incr();  // stampa 2
    incr();  // stampa 2
    incr();  // stampa 2
    incr();  // stampa 2
    return 0;
}
```

**Output:**
```bash
2
2
2
2
```

La variabile locale `x` viene creata e distrutta ogni volta che `incr()` viene chiamata, quindi il valore è sempre `2`.

---

### 1.2 Variabili Globali

Le **variabili globali** sono dichiarate fuori da qualsiasi funzione e:
- Esistono per tutta la durata del programma
- Sono visibili da tutte le funzioni (dopo la loro dichiarazione)
- Mantengono il loro stato tra chiamate successive
- Vengono inizializzate automaticamente a `0` se non specificato

**Esempio:**

```c
int x = 0;  // variabile globale

void incr(void) {
    x = x + 1;
    printf("%d\n", x);
}

int main(void) {
    incr();  // stampa 1
    incr();  // stampa 2
    incr();  // stampa 3
    incr();  // stampa 4
    return 0;
}
```

**Output:**
```bash
1
2
3
4
```

Quando `incr` viene chiamata, il compilatore non trova la variabile `x` a livello locale e la cerca nell'ambito globale. Il suo **stato** persiste per tutta la durata del programma.

---

### 1.3 Variabili Statiche (static)

Le **variabili statiche locali** combinano le caratteristiche di entrambe:
- **Visibilità locale**: visibili solo all'interno della funzione che le dichiara
- **Lifetime globale**: mantengono il loro valore tra chiamate successive
- Vengono inizializzate **una sola volta** alla prima esecuzione

**Esempio:**

```c
void incr(void) {
    static int x = 0;  // inizializzata solo la prima volta
    x = x + 1;
    printf("%d\n", x);
}

int main(void) {
    incr();  // stampa 1
    incr();  // stampa 2
    incr();  // stampa 3
    incr();  // stampa 4
    
    // printf("%d\n", x);  // ERRORE: x non è visibile qui
    return 0;
}
```

**Output:**
```bash
1
2
3
4
```

Se provi ad accedere a `x` nel `main`, otterrai un errore di compilazione: `undeclared identifier`.

#### Variabili statiche globali

Le variabili statiche possono anche essere dichiarate nell'ambito globale:

```c
static int x = 0;  // statica globale
```

In questo caso:
- La variabile è visibile **solo nel file corrente** (internal linkage)
- Non è accessibile da altri file sorgente del progetto
- Utile per "nascondere" variabili globali e prevenire conflitti di nomi

#### Nota sulla sicurezza nei thread

Le variabili statiche locali **non sono thread-safe**. Se più thread accedono contemporaneamente alla stessa variabile statica, possono verificarsi problemi di sincronizzazione (race condition). 

Per proteggerle in ambienti multi-threading è necessario utilizzare meccanismi di sincronizzazione come i **mutex** (mutual exclusion).

---

## 2. Passaggio per Valore vs Riferimento

### 2.1 Passaggio per Valore (Pass by Value)

In C, **i parametri delle funzioni vengono passati per valore** di default. Questo significa che:
- La funzione riceve una **copia** del valore
- Modifiche al parametro **non influenzano** la variabile originale
- Ogni chiamata lavora su dati indipendenti

**Esempio che dimostra il problema:**

```c
int incr(int x) {
    x = x + 1;
    return x;
}

int main(void) {
    int a = 10;
    incr(a);  // a NON viene modificato
    printf("%d\n", a);  // stampa 10
    return 0;
}
```

**Output:**
```bash
10
```

Il valore di `a` rimane `10` perché `incr` lavora su una **copia** del valore di `a`.

**Per modificare effettivamente `a` devi riassegnare il risultato:**

```c
int main(void) {
    int a = 10;
    a = incr(a);  // riassegno il risultato
    printf("%d\n", a);  // stampa 11
    return 0;
}
```

### 2.2 Passaggio per Riferimento (con Puntatori)

Per passare per riferimento in C è necessario utilizzare i **puntatori**. Questo verrà approfondito nelle lezioni successive.

---

## 3. Tipi di Dato

### 3.1 Tipi Primitivi Base

In C esistono diversi tipi di dato primitivi:

| Tipo | Dimensione | Descrizione |
|------|-----------|-------------|
| `char` | 8 bit (1 byte) | Carattere o intero piccolo (-128 a 127) |
| `unsigned char` | 8 bit (1 byte) | Intero senza segno (0 a 255) |
| `short` | 16 bit (2 byte) | Intero corto |
| `int` | Dipende dall'architettura | Intero (tipicamente 32 bit) |
| `unsigned int` | Dipende dall'architettura | Intero senza segno |
| `long` | Dipende dall'architettura | Intero lungo |
| `float` | 32 bit | Numero in virgola mobile (precisione singola) |
| `double` | 64 bit | Numero in virgola mobile (precisione doppia) |

#### unsigned: interi senza segno

Gli interi possono essere dichiarati `unsigned` per rappresentare solo valori positivi:

```c
unsigned int x = 100;
```

**Proprietà importanti:**
- Gli interi `unsigned` seguono l'aritmetica modulo 2^n (dove n è il numero di bit)
- Le operazioni su `unsigned` **non possono andare in overflow** (wrapping garantito)
- Quando un `unsigned` supera il valore massimo, "riparte" da 0

---

### 3.2 Uso di printf con i tipi

**Esempio base:**

```c
int main(void) {
    int a = 10;
    float b = 1.234;
    printf("%d %f\n", a, b);  // output: 10 1.234000
    return 0;
}
```

**Specifier comuni per printf:**
- `%d` o `%i` → int
- `%u` → unsigned int
- `%f` → float/double
- `%c` → char
- `%s` → string
- `%x` o `%X` → esadecimale
- `%p` → puntatore

#### Cosa succede se usi lo specifier sbagliato?

```c
int main(void) {
    int a = 10;
    float b = 1.234;
    
    printf("%d %f\n", b, a);  // TIPI INVERTITI!
    return 0;
}
```

**Output:**
```bash
warning: format specifies type 'int' but the argument has type 'float'
warning: format specifies type 'double' but the argument has type 'int'

-1073741824 0.0000000
```

**Cosa è successo?**
- Il compilatore genera un warning ma compila comunque
- I valori stampati sono **spazzatura** (garbage) perché `printf` interpreta i byte in modo errato
- Il C lascia al programmatore la responsabilità di usare i tipi corretti

---

### 3.3 Conversione Implicita e Promozione dei Tipi

#### Perché il warning parla di `double` se abbiamo usato `float`?

Le funzioni **variadiche** (come `printf`, che accettano un numero variabile di argomenti) non conoscono i tipi a compile-time. Per questo motivo, il C applica delle **regole di promozione automatiche**:

**Regole di promozione per funzioni variadiche:**

1. **Interi più piccoli di `int`** → promossi a `int`
   ```c
   short s = 400;
   printf("%d\n", s);  // s viene promosso a int
   ```

2. **`float`** → promosso a `double`
   ```c
   float f = 1.5;
   printf("%f\n", f);  // f viene promosso a double
   ```

Questo spiega perché il warning menziona `double` anche quando passi un `float`.

#### Conversione nelle espressioni

Nelle operazioni aritmetiche, i tipi vengono promossi secondo regole specifiche:

```c
char c = 127;
int i = c + 1;  // c viene promosso a int prima dell'operazione
printf("%d\n", i);  // stampa 128
```

**Regole generali:**
- Tipi più piccoli di `int` → promossi a `int`
- `signed` + `unsigned` → il `signed` diventa `unsigned`
- Operazioni miste → il tipo "più piccolo" viene promosso al tipo "più grande"

---

### 3.4 Overflow e Wrapping

#### Comportamento con `unsigned` (garantito)

Con gli interi `unsigned`, il wrapping dopo overflow è **garantito dallo standard**:

```c
unsigned char c = 254;
c++;  // 255
c++;  // 0 (wrapping)
printf("%d\n", c);  // stampa 0
```

L'aritmetica modulo 2^n garantisce che:
- `255 + 1 = 0`
- `0 - 1 = 255`


#### Comportamento con `signed` (undefined behavior)

Con gli interi con segno, l'overflow è **undefined behavior**:

```c
char c = 127;  // valore massimo per char signed
c++;  // UNDEFINED BEHAVIOR!
```

**Cosa può succedere:**
- Potrebbe wrappare a -128 (comportamento tipico in two's complement)
- Potrebbe causare un crash
- Potrebbe dare risultati imprevedibili

**Il compilatore potrebbe darti un warning:**
```bash
warning: implicit conversion from 'int' to 'char' changes value from 128 to -128
```

#### Esempio di comportamento non standardizzato

```c
char c = 128;  // ATTENZIONE!
printf("%d\n", c);  // potrebbe stampare -128
```

Questo accade perché:
- `128` è un `int` (letterale)
- Viene convertito a `char` (8 bit con segno)
- In rappresentazione two's complement, `128` diventa `-128`

**Ma questo non è garantito dallo standard!**


L'operazione **modulo** (simbolo `%`) ti dà il **resto** di una divisione:

```
10 % 3 = 1    (10 diviso 3 fa 3 con resto 1)
15 % 4 = 3    (15 diviso 4 fa 3 con resto 3)
7 % 7 = 0     (7 diviso 7 fa 1 con resto 0)
```

**Aritmetica modulo N** significa: qualsiasi operazione, dopo averla fatta, dividi per N e prendi il resto.


**Perché 2^n?**

**n = numero di bit disponibili**

Per un `unsigned char` (8 bit):
- Valori possibili: da `0` a `255` 
- Totale: 256 valori = **2^8**

Quindi lavori in **modulo 256** (modulo 2^8).

---

## Come Funziona il Wrapping

Ogni operazione viene automaticamente "ridotta" modulo 2^n:

```c
unsigned char c = 255;  // valore massimo
c = c + 1;              // 256 % 256 = 0
// c vale ora 0
```

**Altro esempio:**

```c
unsigned char c = 254;
c = c + 5;  // 259

// 259 % 256 = 3  <--- Modulo 256 (2^8)
// c vale ora 3
```

**E all'indietro:**

```c
unsigned char c = 0;
c = c - 1;  // -1

// -1 % 256 = 255
// c vale ora 255
```

---

## Perché "Non Va in Overflow"?

**Tecnicamente NON è overflow** - è **wrapping garantito**.

**Overflow** = comportamento indefinito, errore  
**Wrapping** = comportamento definito dallo standard

Con `unsigned`:
- Il valore "gira" circolarmente: `0 → 1 → 2 → ... → 255 → 0 → 1 → ...`
- È come un orologio: dopo le 23:59 torna alle 00:00
- È **matematicamente definito** e **prevedibile**

Con `signed` invece:
- L'overflow è **undefined behavior**
- Il compilatore può fare quello che vuole
- Non puoi prevedere il risultato

---

## Esempio Concreto: Orologio

Un orologio a 12 ore è "modulo 12":

```
10:00 + 3 ore = 13:00 → 1:00 (13 % 12 = 1)
11:00 + 2 ore = 13:00 → 1:00
```

Un `unsigned char` è un "orologio a 256 ore":

```
250 + 10 = 260 → 4 (260 % 256 = 4)
```

---

## In Sintesi

**2^n** è il numero totale di valori rappresentabili con n bit.

L'aritmetica modulo 2^n garantisce che:
- Non esci mai dall'intervallo `[0, 2^n-1]`
- Il valore "gira" automaticamente
- Il comportamento è **definito** dallo standard C
- NON è overflow, è wrapping previsto e sicuro

Quindi quando dici "non va in overflow", intendi: **il wrapping è il comportamento corretto e garantito**, non un errore.

---

### 3.5 Rappresentazione dei Numeri in Virgola Mobile

#### Formato IEEE 754

I numeri in virgola mobile in C seguono lo standard IEEE 754:

- **`float`** → 32 bit (fp32, precisione singola)
- **`double`** → 64 bit (fp64, precisione doppia)

#### Struttura di un numero in virgola mobile

Un numero in virgola mobile è composto da tre parti:

1. **Segno** (S): 1 bit
2. **Esponente** (E): 8 bit (float) o 11 bit (double)
3. **Mantissa/Frazione** (M): 23 bit (float) o 52 bit (double) [IEEE 754](https://en.wikipedia.org/wiki/IEEE_754)

**Formula generale:**
```
valore = (-1)^S × M × 2^E
```

#### Dalla notazione scientifica al formato IEEE

**Notazione scientifica:**
```
123.45 = 1.2345 × 10^2
```

**Notazione binaria normalizzata:**
```
5.25 (decimale) = 101.01 (binario) = 1.0101 × 2^2
```

Dove:
- `1.0101` è la mantissa (normalizzata)
- `2` è l'esponente

#### Esempio pratico

```c
float f = 1.5;
printf("%f\n", f);   // stampa 1.500000
printf("%a\n", f);   // stampa 0x1.8p+0 (notazione esadecimale)
```

La notazione `%a` mostra la rappresentazione esadecimale:
- `0x1.8` → mantissa in esadecimale
- `p+0` → esponente in base 2

#### Limitazioni dei float

**Precisione limitata:**
```c
float f = 0.1 + 0.2;
printf("%.20f\n", f);  // NON è esattamente 0.3!
// output: 0.30000001192092895508
```

Questo accade perché molti numeri decimali non hanno rappresentazione esatta in binario.

---

## 4. Approfondimenti

### Link di riferimento

- [Integer Promotion Rules - Intel](https://www.intel.com/content/www/us/en/docs/programmable/683846/21-3/integer-promotion-rules.html)
  
- [Integer Promotions in C - GeeksforGeeks](https://www.geeksforgeeks.org/c/integer-promotions-in-c/)
  
- [Numero in virgola mobile - Wikipedia](https://it.wikipedia.org/wiki/Numero_in_virgola_mobile)

- [Conversione di tipo](https://it.wikipedia.org/wiki/Conversione_di_tipo)

- [Print formatted data to stdout](https://cplusplus.com/reference/cstdio/printf/)

- [Mutexes in C - Medium](https://medium.com/@sherniiazov.da/mutexes-in-c-ac2b0f1a6d34)

- [How Floating-Point Numbers Are Represented](https://www.youtube.com/watch?v=bbkcEiUjehk)
  
- [Introduction to Floating-Point Formats (FP32, FP16, BF16)](https://apxml.com/courses/how-to-build-a-large-language-model/chapter-20-mixed-precision-training-techniques/introduction-floating-point-formats)

### Spazio per ulteriori approfondimenti



---

## Note Finali

**Punti chiave da ricordare:**

1. **Scope delle variabili:**
   - Locali → esistono solo nella funzione
   - Globali → esistono per tutto il programma
   - Statiche → visibilità locale, lifetime globale

2. **Passaggio parametri:**
   - Il C passa sempre **per valore**
   - Per modificare l'originale serve usare i puntatori

3. **Tipi di dato:**
   - Usa `unsigned` quando vuoi solo valori positivi e wrapping garantito
   - Gli overflow su `signed` sono undefined behavior
   - `float` ha precisione limitata, usa `double` quando serve maggiore precisione

4. **Printf:**
   - Usa sempre lo specifier corretto (`%d`, `%f`, ecc.)
   - Le funzioni variadiche promuovono automaticamente i tipi
   - Il compilatore ti avvisa ma compila comunque → sei tu responsabile!