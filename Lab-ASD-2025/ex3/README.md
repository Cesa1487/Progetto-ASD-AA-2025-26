# Esercizio 3 — Heap Ternario e Scheduling

## Descrizione

L'obiettivo dell'esercizio è l'implementazione di una **Coda con Priorità (PriorityQueue)** basata su un **Heap Ternario** (ogni nodo ha fino a 3 figli). La struttura dati è generica e supporta operazioni efficienti di inserimento, estrazione del massimo, e rimozione di elementi arbitrari.

Successivamente, la struttura è stata utilizzata per simulare un sistema di **scheduling di task non-preemptive** a processore singolo.

### Caratteristiche dell'Implementazione

1.  **Heap Ternario**:
    * A differenza di un heap binario standard, l'heap ternario riduce l'altezza dell'albero (base del logaritmo 3 invece di 2).
    * Questo comporta un numero minore di livelli da attraversare durante le operazioni di *sift-up* e *sift-down*, al costo di un maggior numero di confronti per nodo (bisogna confrontare il padre con 3 figli invece di 2).

2.  **Supporto Generico (`void*`)**:
    * La coda accetta puntatori generici. L'ordinamento è delegato a una funzione di comparazione fornita dall'utente.
    * Per garantire la funzionalità `contains` e `remove` in tempo sub-lineare, è stata integrata una **Tabella Hash** (riutilizzando l'implementazione dell'Esercizio 2).

Compilazione:
make all

Unit Testing:
./bin/test_ex3
* I test sono automatici (con assert), focalizzati e indipendenti. Se qualcosa fallisce, il processo termina con un errore; in assenza di errori stampa un messaggio finale.

Esecuzione (programma principale): $ ./main_ex3 <input_file> <output_file>

## Risultati Sperimentali

La simulazione è stata eseguita utilizzando il dataset tasks.csv fornito (contenente migliaia di task).

### Verifica Correttezza

L'output generato rispetta la logica non-preemptive:

1. Il processore non rimane inattivo se ci sono task disponibili arrivati.

2. Viene sempre scelto il task a priorità più alta tra quelli in coda.

3. Una volta iniziato, un task viene portato a termine senza interruzioni.

### Tempi di Esecuzione

Eseguendo la simulazione sul dataset completo:

* Tempo reale: 1.393 secondi

* Utilizzo memoria: Basso, proporzionale al numero di task "pendenti" simultaneamente nella coda.

## Conclusioni 

L'implementazione ha dimostrato che l'Heap Ternario è una struttura molto efficiente per gestire le priorità. Essendo l'albero più basso rispetto a uno binario, le operazioni di inserimento sono molto veloci.
Un punto fondamentale è stato l'uso della Tabella Hash di supporto, per capire subito dove si trova l'elemento e rimuoverlo in un attimo