# Esercizio 4 — Grafi sparsi e Visita in Ampiezza (BFS)

## Descrizione

Implementazione in linguaggio C di una **libreria per grafi sparsi**, basata su una tabella hash (esercizio 2), con le seguenti caratteristiche:

* Rappresentazione tramite **tabella hash di tabelle hash** (nodo -> adiacenti).
* Supporto per grafi:
    * **diretti / non diretti**
    * **etichettati / non etichettati**
* Operazioni implementate:
    * `graph_add_node`, `graph_add_edge`, `graph_remove_node`, `graph_remove_edge`
    * `graph_contains_node`, `graph_contains_edge`
    * `graph_get_nodes`, `graph_get_edges`, `graph_get_neighbours`
    * `graph_num_nodes`, `graph_num_edges`
* Complessità:
    * Operazioni marcate `(*)` in traccia: **O(1) medio** (assumendo grado costante).
    * `graph_num_edges`: **O(N)** come richiesto.
* Implementazione dell’algoritmo di **visita in ampiezza (BFS)**:  
  `void** breadth_first_visit(Graph gr, void* start, int (*compare)(...), unsigned long (*hash)(...))`  
  Restituisce i nodi nell’ordine di visita (array `NULL`-terminato).

Test automatici (Unit Testing):
./bin/test_ex4

Esecuzione (programma principale):
./main_ex3-4 <italian_dist_graph.csv> <nodo di partenza> <output.txt>

Dataset: italian_dist_graph.csv

Ogni record contiene i seguenti dati:
* `place1`: (tipo stringa) nome della località "sorgente" (la stringa può contenere spazi ma non può contenere virgole);
* `place2`: (tipo stringa) nome della località "destinazione" (la stringa può contenere spazi ma non può contenere virgole);
* `distance`: (tipo float) distanza in metri tra le due località.

## Risultati

Per valutare concretamente il comportamento della visita in ampiezza, ho eseguito alcuni esperimenti scegliendo nodi di partenza rappresentativi. In particolare, ho considerato: 
* Un nodo appartenente alla componente gigante del grafo (città nota e con molte connessioni), per osservare il caso di una BFS estesa su migliaia di nodi; 
* Un nodo di una componente molto piccola, per evidenziare invece il comportamento limite della visita quando la connettività è minima.

Nodo di partenza: Milano

* Output: milano,
  piemonte,
  morcicchia,
  scoppio,
  montemartano,
  montignano,
  colle del marchese,
  mazzolini,
  villa fabbri,
  moriano,
ecc. (totale 17147 nodi)

Nodo di partenza: Levanzo

* Output: levanzo,
  de giergi (totale 2 nodi)

Non ho trovato nodi di partenza che rappresentassero un caso di BFS medi.

## Conclusioni

L’algoritmo di visita in ampiezza (BFS) si è comportato come atteso:
* Con Milano come nodo di partenza, la BFS ha attraversato quasi l’intero grafo, dimostrando che l’implementazione scala bene su componenti molto grandi.

* Con Levanzo, invece, la BFS ha visitato soltanto 2 nodi, evidenziando il caso limite di una componente isolata.

L’assenza di componenti di dimensione intermedia nel dataset porta a osservare soltanto scenari estremi (visite massime o minime), ma ciò è coerente con la struttura del grafo e non con limiti dell’algoritmo.

