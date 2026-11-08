#include <stdio.h>
#include <stdlib.h>
#include "../include/record.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <file_records>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror("Impossibile aprire il file");
        return 1;
    }

    printf("--- Visualizzazione primi 50 record ---\n");
    printf("Dimensione struct Record nel codice: %lu bytes (Dovrebbe essere 36)\n\n", sizeof(Record));

    for (int i = 0; i < 50; i++) {
        Record r;
        if (read_record(f, &r) == 0) {
            printf("[%d] ", i);
            print_record(&r);
        } else {
            printf("Fine del file o errore.\n");
            break;
        }
    }

    fclose(f);
    return 0;
}