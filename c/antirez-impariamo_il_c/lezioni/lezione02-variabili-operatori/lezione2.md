## La libreria `stdio`

La libreria **`stdio.h`** si occupa delle operazioni di **input/output**, come la stampa a schermo (`printf`) e la lettura da input (`scanf`, ecc.).

Per poter usare queste funzioni è necessario includere la libreria:

```c
#include <stdio.h>
```

---

## La funzione `main`

La funzione `main` è **il punto di ingresso di ogni programma C**: è la prima funzione che viene eseguita.

Una definizione corretta è:

```c
int main(void)
```

* `int` indica il **valore di ritorno** della funzione.
* `void` indica che la funzione **non accetta argomenti**.

Storicamente, in C è buona pratica specificare esplicitamente gli argomenti. Scrivere `main()` senza `void` è consentito in C moderno, ma è meno preciso.

⚠️ La funzione `main` è speciale:

* **non deve essere chiamata manualmente** (come `sum()`)
* viene chiamata direttamente dal sistema operativo

Se scrivessi:

```c
int principale(void) {
    printf("Hello, World");
}
```

otterrei un **errore di compilazione**, perché il compilatore si aspetta una funzione chiamata `main`.

---

## Valore di ritorno di `main`

Anche se `main` non viene chiamata da un’altra funzione del programma, il suo valore di ritorno è importante:

* viene letto dal **sistema operativo**
* per convenzione (Unix):

  * `0` → esecuzione riuscita
  * valore diverso da `0` → errore

Esempi:

```bash
./a.out; echo $?
```

Mostra il valore di ritorno del programma.

```bash
./a.out && ls
```

Grazie allo **short-circuit** di `&&`, `ls` viene eseguito solo se `a.out` restituisce `0`.

---

## Funzioni e parametri: esempio `sum`

Una funzione può:

* ricevere **argomenti**
* restituire un **valore**

Esempio:

```c
int sum(int a, int b)
```

* ritorna un `int`
* prende in input due interi (`a` e `b`)

### Implementazione completa (`sum.c`)

```c
#include <stdio.h>

int sum(int a, int b) {
    return a + b;
}

int main(void) {
    printf("Sum: %d\n", sum(10, 20));
    return 0;
}
```

---

## `printf` e funzioni variadiche

`printf` è l’abbreviazione di **print formatted**.

* il primo argomento è una **stringa di formato**
* i successivi sono i valori da inserire nei placeholder

Esempio:

```c
printf("Sum: %d\n", 30);
```

`printf` è una funzione **variadica**, cioè può accettare un numero variabile di argomenti.

### Errore comune

```c
printf("Sum: %d %d", sum(10, 4));
```

Qui dichiaro **due placeholder `%d`**, ma passo **un solo argomento**.

* il compilatore genera un **warning** (`insufficient arguments`)
* a runtime viene usato un **valore casuale**
* il comportamento è **indefinito** e potenzialmente pericoloso

---

## Variabili e tipi

In C **tutte le variabili devono essere dichiarate** con un tipo.

Esempio:

```c
int a = 20;
int b = 20;
int c = a + b;
```

Oppure in forma abbreviata:

```c
int a, b, c;

a = 20;
b = 20;
c = a + b;
```

L’assegnazione `c = a + b` è un’**espressione**.

---

## Variabili locali e scope

Le variabili dichiarate **all’interno di una funzione** sono variabili **locali**.

* esistono solo **dentro quella funzione**
* vengono create quando la funzione viene chiamata
* vengono distrutte quando la funzione termina

Esempio:

```c
int sum(int a, int b) {
    int c = a + b;
    return c;
}
```

La variabile `c`:

* è visibile solo in `sum`
* **non esiste più** dopo il ritorno dalla funzione

👉 La memoria usata viene liberata automaticamente (tipicamente dallo stack).

---

## Esempio senza funzioni (`sum2.c`)

```c
#include <stdio.h>

int main(void) {
    int a = 20;
    int b = 20;
    int c = a + b;

    printf("Sum: %d\n", c);
    return 0;
}
```

---

## Compilatore e warning

È buona pratica compilare abilitando i warning:

```bash
gcc -O2 -W -Wall hello.c
```

* `-W -Wall` attiva la maggior parte dei warning utili
* aiuta a individuare errori logici e comportamenti indefiniti

---

## Ordine delle funzioni e compilazione

Se in un file dichiari più funzioni, il compilatore deve **conoscere una funzione prima che venga usata**.

Nel caso di:

```c
int sum(int a, int b) { ... }

int main(void) {
    sum(10, 20);
}
```

`sum` deve essere:

* definita prima di `main`
* **oppure** dichiarata con un prototipo prima di `main`

