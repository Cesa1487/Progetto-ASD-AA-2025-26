# Esercizio 1 — Algoritmo di Ordinamento Ibrido (QuickSort + SelectionSort)

## Descrizione

Implementazione di una libreria di ordinamento generica in C che combina QuickSort e SelectionSort. L'algoritmo passa a SelectionSort quando la dimensione del sotto-array scende al di sotto di una soglia `k`, al fine di ridurre l'overhead delle chiamate ricorsive.

**Specifiche di implementazione:**
* Scelta del Pivot: **Mediano di Tre** (per evitare il caso peggiore O(n²) su array ordinati o inversamente ordinati).
* Prototipo generico: `void hybridsort(void *base, size_t nitems, size_t size, size_t k, int (*compar)(const void *, const void *))`
* Supporto per file binari di record da 36 bytes (ID, float, int64, stringa).

Compilazione:
make all

Unit Testing:
./bin/test_ex1
* I test sono automatici (con assert), focalizzati e indipendenti. Se qualcosa fallisce, il processo termina con un errore; in assenza di errori stampa un messaggio finale.

Esecuzione (programma principale): ./bin/main_ex1 <input_file> <output_file> <field> <k>

Comandi per misurare le prestazioni: time, gtimeout

Dataset: records.bin (20M di record).

Metodologia di misura:
* L'obiettivo dell'analisi è verificare l'efficienza dell'approccio ibrido al variare della soglia k su un dataset di 20 milioni di record. Se l'esecuzione supera i 5 minuti, viene riportato "TIMEOUT".

## Risultati

FAIL : Timeout   
CRASH : segmentation fault 

| Soglia `k` | Field 1 `(ID)` | Field 2 `(Float)` | Field 3 `(Int64)` | Field 4 `(Stringa)` |
|----------|----------:|----------:|------------------:|--------------------:|
| 0        |   ⟨4.835⟩ |   ⟨24.298⟩ |          ⟨22.793⟩ |             ⟨CRASH⟩ |
| 10       |    ⟨3.570⟩ |    ⟨23.165⟩ |          ⟨21.746⟩ |             ⟨CRASH⟩ |
| 50       |    ⟨4.052⟩ |    ⟨23.146⟩ |          ⟨21.481⟩ |             ⟨CRASH⟩ |
| 100      |    ⟨5.262⟩ |    ⟨23.657⟩ |          ⟨22.000⟩ |             ⟨CRASH⟩ |
| 500      |    ⟨12.641⟩ |    ⟨30.244⟩ |          ⟨29.563⟩ |             ⟨CRASH⟩ |
| 1000     |    ⟨22.840⟩ |    ⟨39.489⟩ |          ⟨40.275⟩ |             ⟨CRASH⟩ |
| 5000     |    ⟨2:47.53⟩ |    ⟨2:00.49⟩ |         ⟨2:07.78⟩ |             ⟨CRASH⟩ |
| 10000    |    ⟨FAIL⟩ |    ⟨3:47.89⟩ |         ⟨3:57.96⟩ |             ⟨CRASH⟩ |
| 1000000  |    ⟨FAIL⟩ |    ⟨FAIL⟩ |            ⟨FAIL⟩ |              ⟨FAIL⟩ |

---

## Analisi e Conclusioni

1. I risultati corrispondono alle aspettative teoriche?

   Sì, i risultati ottenuti riflettono il comportamento teorico atteso dagli algoritmi impiegati:
* Per valori di k piccoli (k≤100): L'algoritmo esibisce prestazioni ottimali. QuickSort domina l'ordinamento con la sua complessità O(n log n), mentre il SelectionSort interviene solo per sottovettori piccolissimi
* Per valori di k alti (k≥5000): I tempi degenerano rapidamente (passando dai secondi ai minuti, fino al TIMEOUT per k=1000000).

2. Esiste un valore ottimale di k? Se sì, quale e perché?
   
   Dai dati si nota che il valore ottimale di soglia si attesta intorno a k=10. Per questo valore, si registrano i tempi minimi assoluti per tutti i campi funzionanti:
* Field 1 (ID): 3.57 secondi
* Field 2 (Float): 23.1 secondi.
* Field 3 (Int): 21.7 secondi.

  Questo valore è ottimale perché a questa dimensione il costo delle operazioni del SelectionSort è inferiore al costo di gestione dello stack di sistema richiesto per creare nuovi frame di attivazione nelle chiamate ricorsive del QuickSort.

3. L'approccio ibrido offre vantaggi rispetto agli algoritmi puri?

   si

* Rispetto al QuickSort puro: L'approccio ibrido con k=10 offre un miglioramento tangibile delle prestazioni. Nel caso del Field 1, il tempo scende da 4.83 a 3.57 secondi
* Rispetto al SelectionSort puro: Il SelectionSort puro è computazionalmente inapplicabile per 20M di record (il test a k=1000000 fallisce per timeout dopo 5 minuti)

4. Differenze significative tra i diversi tipi di campi e Anomalie

   Si notano due comportamenti molto diversi:

* Field 1 (ID): È nettamente più veloce rispetto a Float e Int. Questo potrebbe dipendere dal fatto che i record nel dataset binario siano già parzialmente ordinati per ID. Grazie alla scelta del pivot "Mediano di Tre", il QuickSort performa in modo eccellente su dati pre-ordinati, avvicinandosi al caso migliore e garantendo partizioni sempre bilanciate.
* Field 2 (Float) e Field 3 (Int): Mostrano tempi di esecuzione molto simili tra loro, indicando che i dati su questi campi sono distribuiti in modo sufficientemente casuale.

5. Risoluzione dei problemi riscontrati

L'analisi ha evidenziato due criticità:

* TIMEOUT per k≥10000: Il valore di k viene impostato troppo alto per la natura dell'algoritmo di base.
* CRASH sul Field 4 (Stringhe): L'ordinamento fallisce per tutti i valori di k, dovuto a un fattore tecnico nell'implementazione:

* Stack Overflow per elementi duplicati: Il dataset contiene molte parole ripetute. Un QuickSort standard a due vie, in presenza di troppi elementi uguali al pivot, rischia di sbilanciarsi creando partizioni asimmetriche, portando a una profondità di ricorsione O(n) che causa il collasso dello stack di memoria (Segmentation Fault). 

* Soluzione Analizzata: Ho  valutato e testato un'implementazione basata sul quicksort a 3 vie (che raggruppa gli elementi in < pivot, == pivot, > pivot). Questa soluzione risolveva efficacemente il crash, portando il tempo di esecuzione a soli 15 secondi(Valore riferito con k=1000). 

* Decisione Finale: Nonostante la stabilità sui duplicati, i test hanno evidenziato che la versione a 3 vie introduceva un notevole overhead (inutili operazioni di swap) nel caso di array già ordinati o con elementi unici (es. Field 1), peggiorandone drasticamente le prestazioni rispetto alla versione a due vie. Conclusione: Decido di mantenere l'implementazione standard a 2 vie poiché garantisce prestazioni eccellenti sulla maggior parte dei casi d'uso (Field 1, 2, 3), accettando il limite sul caso dei duplicati.