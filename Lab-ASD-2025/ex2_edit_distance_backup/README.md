# Esercizio 2 — Edit Distance

## Descrizione
Implementazione di una libreria in C per il calcolo della distanza di edit tra due stringhe, usando solo operazioni di inserimento e cancellazione (niente sostituzioni).

Funzioni implementate:
* int edit_distance(const char *s1, const char *s2) — versione ricorsiva pura.
* int edit_distance_dyn(const char *s1, const char *s2) — versione ricorsiva con programmazione dinamica.

Compilazione:
make all

Test automatici (Unit Testing):
./bin/test_ex2

Esecuzione (programma principale): ./bin/main_ex2 <dictionary.txt> <correctme.txt>

Dataset:
* dictionary.txt: lista di parole
* correctme.txt: testo contenente parole da verificare/correggere.

Metodologia di misura:
* verificare il corretto funzionamento della libreria e osservare le prestazioni.
* eseguire correctme.txt andando a osservare i risultati.

## Risultati

Andando ad eseguire correctme.txt, si ottengono i seguenti risultati:

* quando: acquando ando (distance=2)

* avevo: avevo (distance=0)

* cinqve: achive acie acline acne actine bine bucine cabine caie cane caneve canine canove capine carine cave ce cecine cene cerine cetine (distance=4)

* anni: anni (distance=0)

* mia: a aia ama ambia ami amica amida amina ampia (distance=2)

* made: ade (distance=1)

* mi: ami (distance=1)

* perpeteva: aperta aperte appetiva caperete (distance=5)

* sempre: aspre (distance=3)

* che: che (distance=0)

* la: a ala (distance=1)

* felicita: afelica afelici (distance=3)

* e: be ce (distance=1)

* la: a ala (distance=1)

* chiave: achive cave (distance=2)

* della: bella bidella budella cella (distance=2)

* vita: avita (distance=1)

* quando: acquando ando (distance=2)

* andai: andai (distance=0)

* a: a (distance=0)

* squola: asola assuola (distance=3)

* mi: ami (distance=1)

* domandrono: addomandarono (distance=3)

* come: acme acrome bome ce (distance=2)

* vuolessi: avessi avulsi avvolgessi (distance=4)

* essere: cesserei (distance=2)

* da: a (distance=1)

* grande: ande brande (distance=2)

* io: aio bio (distance=1)

* scrissi: ascrissi (distance=1)

* selice: alice celie (distance=3)

* mi: ami (distance=1)

* dissero: adissero (distance=1)

* che: che (distance=0)

* non: ano anona anone bon canon (distance=2)

* avevo: avevo (distance=0)

* capito: capito (distance=0)

* il: ai al bi bile bili (distance=2)

* corpito: carpito (distance=2)

* e: be ce (distance=1)

* io: aio bio (distance=1)

* dissi: adissi (distance=1)

* loro: alloro boro (distance=2)

* che: che (distance=0)

* non: ano anona anone bon canon (distance=2)

* avevano: avevano (distance=0)

* capito: capito (distance=0)

* la: a ala (distance=1)

* wita: aita (distance=2)

## Risultati

In generale, restituisce per ogni parola non presente nel dizionario una lista di possibili correzioni a distanza minima. I risultati dimostrano la correttezza della libreria.