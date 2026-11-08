# Esercizio 3 — Tavole hash (con concatenamento) 

## Introduzione
L’obiettivo di questo esercizio era implementare in linguaggio C una **tabella hash generica con concatenamento**, dotata delle principali operazioni (inserimento, ricerca, rimozione, ecc.), corredarla di **unit test automatici**, e infine sviluppare un programma che, dato un file di testo (in particolare l’Iliade), individui la parola più frequente di lunghezza almeno pari a un valore minimo specificato.

---

## Sviluppo con supporto di un LLM
Per la realizzazione dell’esercizio mi sono avvalso del supporto di un **Large Language Model (LLM)**, in particolare ChatGPT.

- **Prompt iniziale**: ho chiesto suggerimenti per l’implementazione di una tabella hash generica in C con concatenamento e gestione di chiavi/valori generici tramite `void*`.
- **Primo output**: il modello ha proposto una struttura di base corretta, ma priva di gestione del ridimensionamento e con alcune semplificazioni.
- **Analisi critica**: l’output era utile come scheletro iniziale, ma incompleto rispetto ai requisiti (mancava ad esempio il raddoppio della capacità al superamento del load factor).
- **Risultato finale**: ho ottenuto un’implementazione che rispetta le specifiche richieste. Il processo di interazione con l’LLM ha accelerato la scrittura del codice, ma ha richiesto comunque un’analisi critica e vari aggiustamenti manuali.
- **Considerazioni generali**: l’uso dell’LLM è stato utile per chiarire dubbi e avere esempi iniziali. È stato necessario verificare attentamente la correttezza e la qualità del codice.

## Dettagli di implementazione
- **Struttura dati**: tabella hash con concatenamento (liste collegate nei bucket).
- **Genericità**: chiavi e valori sono gestiti come `void*`; l’utente deve fornire due funzioni:
    - un comparatore (`equals`) che ritorna 0 se due chiavi sono uguali;
    - una funzione di hash (`hash`) che calcola il valore di hash della chiave.
- **Funzionalità implementate**:
    - `hash_table_create`, `hash_table_put`, `hash_table_get`, `hash_table_contains_key`, `hash_table_remove`, `hash_table_size`, `hash_table_keyset`, `hash_table_free`.
- **Politica di ownership**: la tabella non libera automaticamente chiavi e valori; è responsabilità dell’utente gestire la memoria.
- **Gestione delle collisioni**: concatenamento tramite liste concatenate.
- **Resize**: la capacità viene raddoppiata quando il load factor supera 0.75.
- **Funzioni ausiliarie**:
    - `str_eq` (comparazione stringhe tramite `strcmp`)
    - `str_hash`
- **Programma principale (`main_ex3`)**:

Test automatici (Unit Testing):
./bin/test_ex2

Esecuzione (programma principale):
./main_ex3 <iliade.txt> <l. parole>

Dataset: iliade.txt

## Risultati

Andando ad eseguire il programma principale con il file di testo `iliade.txt` si ottengono i seguenti risultati:

<iliade.txt> <5>

* Most frequent word (len >= 5): "their" (872 occurrences)

<iliade.txt> <6>

* Most frequent word (len >= 6): "trojans" (562 occurrences)

<iliade.txt> <7>

* Most frequent word (len >= 7): "trojans" (562 occurrences)

<iliade.txt> <8>

* Most frequent word (len >= 8): "achaeans" (541 occurrences)

## Conclusioni

La tabella hash implementata ha permesso di contare in maniera efficiente le occorrenze delle parole nel testo.
* Le operazioni di inserimento e ricerca hanno mostrato tempi pressoché costanti.
* La funzione di resize ha garantito buone prestazioni evitando degradazioni dovute all’aumento del load factor.
* L’uso dell’LLM ha accelerato la fase di scrittura del codice.