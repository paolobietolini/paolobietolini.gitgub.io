## 1. Il compilatore C e le flag

Il compilatore `cc` può essere eseguito con diverse **flag** (opzioni) che ne modificano il comportamento.

### Visualizzare l’assembly

Usando la flag `-S`:

```bash
cc -S programma.c
```

il compilatore **non produce un eseguibile**, ma genera il **codice assembly** corrispondente al programma C.
Questo permette di vedere cosa viene prodotto internamente durante la compilazione.

### Ottimizzazione

È possibile abilitare l’**ottimizzazione** con flag come:

```bash
cc -O2 programma.c
```

`-O2` indica un livello di ottimizzazione intermedio (più veloce del codice non ottimizzato, ma senza eccessivi compromessi).

---

## 2. Il preprocessore

Prima della vera compilazione, il codice C viene analizzato da un altro programma chiamato **preprocessore**.

### Cos’è una direttiva del preprocessore?

Una **direttiva** è un’istruzione che:

* inizia con `#`
* viene elaborata **prima della compilazione**

Esempio:

```c
#include <stdio.h>
```

Questa **non è una istruzione C**, ma una direttiva del preprocessore.

---

## 3. La direttiva `#include`

La direttiva:

```c
#include <stdio.h>
```

dice al preprocessore di:

* **prendere il contenuto del file `stdio.h`**
* **inserirlo nel programma** prima della compilazione

### A cosa serve `stdio.h`?

`stdio.h` contiene:

* i **prototipi** di funzioni come `printf`
* le definizioni necessarie per l’input/output standard

Senza includerlo, il compilatore **non sa** come è fatta `printf()`.

---

## 4. Header file (`.h`)

L’estensione `.h` significa **Header**.

Gli header contengono solitamente:

* prototipi di funzioni
* definizioni di costanti
* dichiarazioni di strutture e tipi

Esempio:

```c
#include <stdio.h>
```

È anche possibile includere file locali:

```c
#include "file.c"
```

(anche se **non è una buona pratica**: di solito si includono `.h`, non `.c`).

---

## 5. Il prototipo di `printf`

Il prototipo di `printf` (da `man 3 printf`) è:

```c
int printf(const char *restrict format, ...);
```

Questo prototipo:

* dice al compilatore **come deve essere chiamata la funzione**
* permette il **controllo dei tipi** degli argomenti

### Domanda importante

> Se includessi solo questo prototipo al posto di `stdio.h`, funzionerebbe?

✔ **Sì, funzionerebbe**: il compilatore avrebbe le informazioni necessarie per compilare il codice.

❌ **Ma non è consigliato**, perché:

* `stdio.h` contiene **altre definizioni necessarie**
* potresti avere problemi di compatibilità
* perdi portabilità e sicurezza

---

## 6. Riassunto rapido

* `cc -S` → genera codice assembly
* `cc -O2` → abilita ottimizzazioni
* Le direttive (`#include`, `#define`, ecc.) sono gestite dal **preprocessore**
* `#include <stdio.h>` permette di usare `printf`
* `.h` significa **header**
* Scrivere a mano il prototipo di `printf` funziona, ma **non è buona pratica**



