# Lezione 2.5 - Le Variabili Locali e lo Stack

**Video di riferimento:** [Addendum lezione 2 - La vita delle variabili locali](https://www.youtube.com/watch?v=r6mU_IHXEps)

---

## 1. Concetti Fondamentali

### 1.1 ELI5: Cos'è lo Stack

Immagina una **pila di piatti** in cucina:

- Puoi mettere un piatto sopra (PUSH)
- Puoi togliere solo il piatto in cima (POP)
- Non puoi prendere il piatto in mezzo senza togliere prima quelli sopra

Lo stack del computer funziona **esattamente così**. È una zona di RAM organizzata come una pila.

**Dettaglio chiave**: lo stack in x86 cresce **al contrario** - verso gli indirizzi più bassi. È come se i piatti li accatastassi verso il basso invece che verso l'alto.

### 1.2 RSP e RBP - I Due Registri Chiave

**RSP (Stack Pointer)** = **"Dove si trova la cima della pila in questo momento"**
- Punta sempre all'ultimo elemento inserito
- Si sposta ogni volta che fai PUSH (scende) o POP (sale)
- È dinamico, cambia continuamente

**RBP (Base Pointer)** = **"Punto di riferimento fisso per questa funzione"**
- Viene salvato all'inizio di una funzione
- Serve come "metro" per trovare parametri e variabili locali
- Non si muove durante l'esecuzione della funzione
- È come mettere un segnaposto nella pila
- Contiene l'indirizzo di memoria dell'inizio della funzione corrente

**Perché RSP scende quando fai PUSH?**
Sì, esatto! Lo stack pointer (RSP/ESP) scende perché in x86 lo stack cresce verso indirizzi più bassi. Quando fai `PUSH`, RSP diminuisce (es: da 0x1000 a 0x0FFC).

---

## 2. Evoluzione dello Stack - Visualizzazione Completa

### Codice di esempio

```c
int sum(int a, int b) {
    int x = a + b;
    return x;
}

int main() {
    sum(10, 20);
}
```

### STEP 1: Nel main, prima di chiamare sum

```
Memoria (indirizzi alti → bassi)

0x2000  ┌─────────────┐
        │    ...      │
0x1FF8  ├─────────────┤
        │  main data  │
0x1FF0  └─────────────┘  ← RSP = 0x1FF0
                         ← RBP = 0x1FF0 (o altro valore del main)
```

**Registri:**
- RSP = 0x1FF0 (cima dello stack)
- RBP = 0x1FF0 (base del main)

---

### STEP 2: Push parametri

```asm
push 20    ; b
push 10    ; a
```

```
0x2000  ┌─────────────┐
        │    ...      │
0x1FF8  ├─────────────┤
        │  main data  │
0x1FF0  ├─────────────┤
        │     20      │  ← parametro b
0x1FE8  ├─────────────┤
        │     10      │  ← parametro a
0x1FE0  └─────────────┘  ← RSP = 0x1FE0 (sceso di 16 bytes)
                         ← RBP = 0x1FF0 (invariato)
```

**Registri:**
- RSP = 0x1FE0 (sceso!)
- RBP = 0x1FF0 (ancora quello del main)

---

### STEP 3: CALL sum

```asm
call sum   ; push automatico dell'indirizzo di ritorno
```

```
0x2000  ┌─────────────┐
        │    ...      │
0x1FF8  ├─────────────┤
        │  main data  │
0x1FF0  ├─────────────┤
        │     20      │  ← b
0x1FE8  ├─────────────┤
        │     10      │  ← a
0x1FE0  ├─────────────┤
        │ ret_address │  ← indirizzo di ritorno nel main
0x1FD8  └─────────────┘  ← RSP = 0x1FD8
                         ← RBP = 0x1FF0
```

**Registri:**
- RSP = 0x1FD8 (sceso ancora di 8 bytes)
- RBP = 0x1FF0 (ancora quello del main)

---

### STEP 4: Prologo di sum - Dettagliato

#### Istruzione 1: `push rbp`

"Salva il vecchio RBP del main sullo stack"

```
0x1FD8  ├─────────────┤
        │ ret_address │
0x1FD0  ├─────────────┤
        │   0x1FF0    │  ← ho salvato il RBP del main qui!
0x1FC8  └─────────────┘  ← RSP = 0x1FC8
```

**Registri:**
- RSP = 0x1FC8 (sceso)
- RBP = 0x1FF0 (ancora invariato!)

---

#### Istruzione 2: `mov rbp, rsp`

"Il nuovo RBP di sum = dove si trova RSP ora"

```
0x1FD8  ├─────────────┤
        │ ret_address │
0x1FD0  ├─────────────┤
        │   0x1FF0    │  ← vecchio RBP salvato
0x1FC8  └─────────────┘  ← RBP = 0x1FC8 (NUOVO segnaposto!)
                         ← RSP = 0x1FC8
```

**Registri:**
- RSP = 0x1FC8
- **RBP = 0x1FC8** (CAMBIATO! Questo è il segnaposto di sum)

**Ora da 0x1FC8 (RBP) posso:**
- Andare SU (offset positivi) → parametri, return address, vecchio RBP
- Andare GIÙ (offset negativi) → variabili locali (che creo dopo)

---

#### Istruzione 3: `sub rsp, 8`

"Fai spazio per le variabili locali"

```
0x1FD8  ├─────────────┤
        │ ret_address │
0x1FD0  ├─────────────┤
        │   0x1FF0    │  ← [RBP + 0] = vecchio RBP
0x1FC8  ├─────────────┤  ← RBP = 0x1FC8 (FERMO!)
        │      x      │  ← [RBP - 8] = variabile locale
0x1FC0  └─────────────┘  ← RSP = 0x1FC0 (sceso ancora)
```

**Registri:**
- RSP = 0x1FC0 (cima dello stack)
- RBP = 0x1FC8 (FERMO, non si muove più!)

#### Stack Frame Completo

```
          indirizzi alti
┌────────────────────────┐
│        ...             │
├────────────────────────┤
│        b               │ ← [RBP + 16]
├────────────────────────┤
│        a               │ ← [RBP + 8]
├────────────────────────┤
│ return address         │ ← [RBP + 4] (tecnicamente non lo usiamo così)
├────────────────────────┤
│ old RBP (0x1FF0)       │ ← [RBP + 0]
├────────────────────────┤  ← RBP = 0x1FC8 (punto di riferimento)
│ x (variabile locale)   │ ← [RBP - 8]
└────────────────────────┘  ← RSP = 0x1FC0
          indirizzi bassi
```

**Chiave del modello mentale:**
* Offset **positivi** da RBP → parametri
* Offset **negativi** da RBP → variabili locali
* `RBP` è il punto di riferimento stabile

---

### STEP 5: Esecuzione di sum

```asm
mov eax, [rbp + 8]    ; eax = a = 10
add eax, [rbp + 16]   ; eax = eax + b = 30
mov [rbp - 8], eax    ; x = 30
mov eax, [rbp - 8]    ; return value in eax
```

Lo stack **non cambia**, lavoriamo solo con i registri.
`EAX` contiene il valore di ritorno secondo la calling convention.

---

### STEP 6: Epilogo di sum - Il Ritorno

```asm
mov rsp, rbp    ; RSP torna dove era RBP
pop rbp         ; ripristino il vecchio RBP
ret             ; torno al chiamante
```

#### Dopo `mov rsp, rbp`:

```
PRIMA:
RBP = 0x1FC8
RSP = 0x1FC0

0x1FD0  ├─────────────┤
        │   0x1FF0    │  ← vecchio RBP salvato
0x1FC8  ├─────────────┤  ← RBP
        │      x      │
0x1FC0  └─────────────┘  ← RSP

DOPO mov rsp, rbp:
RBP = 0x1FC8
RSP = 0x1FC8 (uguale a RBP!)

0x1FD0  ├─────────────┤
        │   0x1FF0    │  ← sta per essere letto
0x1FC8  └─────────────┘  ← RBP e RSP (lo spazio di x è ora "garbage")
```

#### Dopo `pop rbp`:

```
DOPO pop rbp:
RBP = 0x1FF0 (RIPRISTINATO dal main!)
RSP = 0x1FD0

0x1FD8  ├─────────────┤
        │ ret_address │  ← sta per essere letto
0x1FD0  └─────────────┘  ← RSP
```

#### Dopo `ret`:

```
0x1FE0  ├─────────────┤
        │     10      │  ← RSP = 0x1FE0
0x1FD8  ├─────────────┤
        │ ret_address │  ← letto e messo in RIP (instruction pointer)
        └─────────────┘
```

**Il main ora deve fare:**
```asm
add rsp, 16    ; pulisco i parametri (10 e 20)
```

E RSP torna a 0x1FF0!

---

### Riepilogo dell'evoluzione dei registri

| Step | RSP      | RBP      | Dove punta RBP |
|------|----------|----------|----------------|
| 1    | 0x1FF0   | 0x1FF0   | main frame     |
| 2    | 0x1FE0   | 0x1FF0   | main frame     |
| 3    | 0x1FD8   | 0x1FF0   | main frame     |
| 4    | 0x1FC0   | **0x1FC8** | **sum frame!** |
| 6    | 0x1FD8   | 0x1FF0   | main frame (ripristinato) |

**Il meccanismo di ritorno:**
1. Non "torni fino al base pointer"
2. **RIPRENDI il vecchio RBP che avevi salvato all'inizio con `push rbp`!**
3. È come una matrioska: ogni funzione salva il segnaposto di chi l'ha chiamata, fa il suo lavoro, e poi lo ripristina

---

## 3. Teoria: Variabili, Registri e Memoria

### 3.1 Concetto fondamentale

**Le variabili sono un concetto del linguaggio di programmazione e del compilatore, non dell'hardware.**

Il processore mette a disposizione:
- **Registri**: memoria velocissima ma limitata
- **RAM**: memoria ampia ma più lenta

Il compilatore decide come mappare le variabili su queste risorse.

### 3.2 Registri

I registri costituiscono il livello più alto della gerarchia della memoria:
- Estremamente veloci
- Numero limitato
- Identificati da nomi simbolici (A, X, Y in 6502; RAX, RBX, RSP, RBP in x86)
- Usati per operazioni temporanee sui dati

### 3.3 Allocazione delle variabili locali

Le variabili locali possono essere allocate:

* **Nei registri** (se poche e frequentemente usate)
* **Nella memoria RAM**, tipicamente nello **stack**
* **Eliminate** tramite ottimizzazioni del compilatore

### 3.4 Calling Convention

Le **calling convention** definiscono:
- Come vengono passati i parametri (registri e/o stack)
- Come viene gestito il valore di ritorno
- Quali registri devono essere preservati
- Come viene gestito lo stack

Permettono l'interoperabilità tra codice compilato per la stessa architettura e ABI.

---

## 4. Differenze Architetturali

### 4.1 Architettura 6502 (semplice)

Nelle architetture semplici come il **6502**, il numero di registri è molto limitato.

**Caratteristiche:**
- Registri: A (accumulatore), X, Y (indici), SP (stack pointer)
- Le variabili e i parametri venivano memorizzati principalmente in **RAM**
- Uso frequente della **Zero Page**: zona di memoria accessibile più efficientemente
- I registri servivano per operare temporaneamente sui dati, non per conservarli a lungo

### 4.2 Architettura x86 (evoluta)

Nelle architetture più evolute come **x86**, la gestione è più sofisticata.

**Caratteristiche:**
- Molti più registri disponibili
- Stack gestito tramite registri dedicati (SP/RSP, BP/RBP)
- Il registro SP/RSP contiene l'**indirizzo** dello stack, non i dati stessi
- Il compilatore sceglie dinamicamente tra registri, stack o entrambi
- Ottimizzazioni avanzate possono eliminare variabili o RBP stesso

---

## 5. Stack Frame in Dettaglio (x86 32-bit)

### 5.1 Contesto

* Architettura: **x86 a 32 bit**
* Calling convention: **cdecl**
* Stack che cresce **verso indirizzi più bassi**
* Frame pointer attivo (`EBP`)

### 5.2 Meccanismo di chiamata

Quando viene chiamata una funzione:

1. **Chiamante** prepara i parametri (PUSH sullo stack)
2. **CALL** automaticamente fa PUSH dell'indirizzo di ritorno
3. **Funzione chiamata** esegue il prologo:
   - Salva il vecchio EBP
   - Imposta il nuovo EBP
   - Riserva spazio per variabili locali
4. **Funzione** esegue il suo codice
5. **Epilogo** ripristina lo stack e ritorna

### 5.3 Accesso ai dati tramite offset

Da EBP (Base Pointer) posso accedere a:

**Offset positivi** (verso l'alto nello stack):
- `[EBP + 8]` = primo parametro
- `[EBP + 12]` = secondo parametro
- `[EBP + 4]` = indirizzo di ritorno (raramente accessato direttamente)
- `[EBP + 0]` = vecchio EBP salvato

**Offset negativi** (verso il basso nello stack):
- `[EBP - 4]` = prima variabile locale
- `[EBP - 8]` = seconda variabile locale
- ecc.

### 5.4 Nota importante sui dati "cancellati"

Dopo un'operazione di POP (o dopo che lo Stack Pointer viene spostato manualmente con un'istruzione come `ADD ESP, 8`), **i dati non vengono cancellati fisicamente dalla memoria**. 

Essi rimangono scritti in quella locazione della RAM, ma vengono considerati "distrutti" perché:
- Lo Stack Pointer non punta più a loro
- Il sistema non ha più un modo formale per referenziarli
- Quei dati rimarranno lì finché una successiva chiamata a funzione o una nuova operazione di PUSH non li sovrascriverà con nuove informazioni

---

## 6. Esempi di Codice Assembly

### 6.1 Esempio 6502 - Fill memory con pattern

**Codice Assembly mostrato nel video:**

```asm
START:
    LDA #50
    JSR FILL_FIVE
    BRK

FILL_FIVE:
    TAX
    BEQ DONE
    LDY #$00
    LDA #$05
LOOP:
    STA $0200,Y
    INY
    DEX
    BNE LOOP
DONE:
    RTS
```

**Note sul simulatore:**
> Memory location $fe contains a new random byte on every instruction.
> Memory location $ff contains the ascii code of the last key pressed.
> 
> Memory locations $200 to $5ff map to the screen pixels. Different values will
> draw different colour pixels. The colours are:
> 
> $0: Black, $1: White, $2: Red, $3: Cyan, $4: Purple, $5: Green, 
> $6: Blue, $7: Yellow, $8: Orange, $9: Brown, $a: Light red, 
> $b: Dark grey, $c: Grey, $d: Light green, $e: Light blue, $f: Light grey

---

### 6.2 Esempio x86 - Funzione sum

**Codice C:**

```c
#include <stdio.h>

int sum (int a, int b) {
    return a + b;
}

int main(void) {
    return sum(10,20);
}
```

**Assembly generato (x86 32-bit, cdecl):**

```asm
_a$ = 8                                       ; size = 4
_b$ = 12                                      ; size = 4
int sum(int,int) PROC                         ; sum
        push    ebp
        mov     ebp, esp
        mov     eax, DWORD PTR _a$[ebp]
        add     eax, DWORD PTR _b$[ebp]
        pop     ebp
        ret     0
int sum(int,int) ENDP                         ; sum

_main   PROC
        push    ebp
        mov     ebp, esp
        push    20                            ; 00000014H
        push    10                            ; 0000000aH
        call    int sum(int,int)              ; sum
        add     esp, 8
        pop     ebp
        ret     0
_main   ENDP
```

---

### 6.3 Esempio dettagliato con variabile locale

**Codice C:**

```c
int sum(int a, int b) {
    int x = a + b;
    return x;
}
```

**Assembly con commenti:**

```asm
push ebp                  ; salva frame pointer del chiamante
mov ebp, esp              ; imposta il frame pointer per questa funzione
sub esp, 4                ; riserva spazio per variabile locale x

mov eax, [ebp + 8]        ; primo parametro (a)
mov edx, [ebp + 12]       ; secondo parametro (b)
add eax, edx              ; somma
mov [ebp - 4], eax        ; salva in x

mov eax, [ebp - 4]        ; valore di ritorno in EAX
mov esp, ebp              ; ripristina stack pointer
pop ebp                   ; ripristina frame pointer
ret                       ; ritorna (EAX contiene il risultato)
```

**Il chiamante:**

```asm
push 4                    ; parametro b
push 2                    ; parametro a
call sum                  ; chiama la funzione
add esp, 8                ; pulisce i parametri dallo stack (2 × 4 bytes)
```

**Spiegazione del flusso:**

- `push` spinge i parametri sullo stack prima della chiamata
- `call` spinge l'indirizzo di ritorno
- `EBP+8` e `EBP+12` sono gli offset per i parametri
- `EBP-4` è l'offset per la variabile locale x
- `pop ebp` e `ret` chiudono lo stack frame e tornano al chiamante

---

## 7. Note Finali

### 7.1 RBP/EBP non è obbligatorio

**EBP non è obbligatorio, ma è didatticamente potentissimo**

Serve per:
- Debug (stack trace, debugger)
- Capire il funzionamento dello stack
- Costruire il modello mentale

Nei compilatori ottimizzati:
- `EBP` può essere eliminato (omit frame pointer)
- Gli offset vengono calcolati direttamente da ESP/RSP
- Alcune variabili **non esistono più** (ottimizzate via)

**Ma il concetto resta:**

> Stack frame = parametri + return address + stato salvato + variabili locali

### 7.2 Il linguaggio C e i prototipi

Il linguaggio C, conoscendo il prototipo di una funzione, sa:
- Come preparare i parametri secondo la calling convention
- Come chiamarla conoscendone l'indirizzo in memoria

Per questo è possibile usare una funzione come `printf` semplicemente includendone il prototipo con `#include <stdio.h>`.

### 7.3 Principio generale

> In x86 a 32 bit, il compilatore può organizzare le funzioni usando uno stack frame basato su EBP, nel quale parametri e variabili locali sono accessibili tramite offset fissi. Questo modello, pur non universale, è fondamentale per comprendere il funzionamento delle chiamate di funzione a basso livello.

---

## 8. Strumenti Didattici

- **6502 Simulator**: https://skilldrick.github.io/easy6502/
  - Permette di scrivere ed eseguire codice assembly 6502
  - Visualizza registri e memoria in tempo reale
  
- **Compiler Explorer**: https://godbolt.org/
  - Mostra il codice assembly generato da vari compilatori
  - Supporta moltissimi linguaggi e architetture
  - Utile per vedere come il codice C viene tradotto in assembly

---

## 9. Approfondimenti

### Link di riferimento

- [Annotated x86 Disassembly - Microsoft](https://learn.microsoft.com/it-it/windows-hardware/drivers/debugger/annotated-x86-disassembly)
  - Documentazione ufficiale Microsoft su assembly x86
  - Spiegazione del debugging e disassembly

- [Assembly Basics - Baker Street](https://bakerst221b.com/docs/ttp/malware/analyse/basics/assebly/)
  - Introduzione alle basi dell'assembly
  - Contesto: analisi malware

### Ulteriori approfondimenti
- [do you know how "return" works under the hood?](https://www.youtube.com/watch?v=e46wHUjNDjE)
- [wtf is “the stack” ?](https://www.youtube.com/watch?v=CRTR5ljBjPM&)
- [Why is the CALLSTACK so Important? Assembly Tutorial for Beginners](https://www.youtube.com/watch?v=_HeYWi67mR4)
- [Programmazione Assembly (x86 e x86-64)](https://www.youtube.com/watch?v=NIhZdsWHBWM)
- [you can learn assembly in 10 minutes](https://www.youtube.com/watch?v=6S5KRJv-7RU)
- [Call Stacks - CS50 Shorts](https://www.youtube.com/watch?v=aCPkszeKRa4)
- [The Call Stack](https://www.youtube.com/watch?v=Q2sFmqvpBe0)
- [Stack-based memory allocation](https://en.wikipedia.org/wiki/Stack-based_memory_allocation)
- [Compiler Explorer](https://godbolt.org/)
- [Easy 6502](https://skilldrick.github.io/easy6502/)
