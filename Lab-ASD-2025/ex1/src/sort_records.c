#include <stdio.h>
#include <stdlib.h>
#include "../include/sort.h"
#include "../include/record.h"

/**
 * @file sort_records.c
 * @brief Utility for loading, sorting, and saving arrays of Record structures.
 *
 * This module provides a wrapper that:
 *  - loads all records from a binary input file,
 *  - selects a comparator function based on the requested sorting field,
 *  - sorts the records using ::hybridsort,
 *  - saves the sorted array to the output binary file.
 *
 * It is meant as a high-level convenience function for sorting whole files
 * of ::Record structures.
 */

/**
 * @brief Loads, sorts, and writes an array of records.
 *
 * This function performs the following steps:
 *  1. Loads all ::Record elements from the input binary file.
 *  2. Chooses a comparator function based on the selected sorting field.
 *  3. Sorts the array using the hybrid sorting algorithm ::hybridsort
 *     (Quick Sort + Selection Sort with threshold @p k).
 *  4. Writes the sorted array back to the output binary file.
 *
 * The input and output files must already be opened in binary mode:
 *  - input  → "rb"
 *  - output → "wb"
 *
 * @param infile  Pointer to the binary input file.
 * @param outfile Pointer to the binary output file.
 * @param field   Sorting key:
 *                 - 1 → Record::id
 *                 - 2 → Record::field1 (float)
 *                 - 3 → Record::field2 (integer)
 *                 - 4 → Record::field3 (string, lexicographic)
 * @param k       Threshold for switching from Quick Sort to Selection Sort
 *                inside ::hybridsort.
 */
void sort_records(FILE *infile, FILE *outfile, size_t field, size_t k) {
    if (!infile || !outfile) {
        fprintf(stderr, "Errore: file non valido.\n");
        return;
    }

    Record *records = NULL;
    size_t count = 0;

    if (load_records_from_file(infile, &records, &count) != 0) {
        fprintf(stderr, "Errore durante la lettura del file di input.\n");
        return;
    }

    if (count == 0) {
        fprintf(stderr, "Nessun record da ordinare.\n");
        free(records);
        return;
    }

    // Select comparator function based on field
    int (*cmp_func)(const void *, const void *) = NULL;

    switch (field) {
        case 1:
            cmp_func = cmp_by_id;
            break;
        case 2:
            cmp_func = cmp_by_field1;
            break;
        case 3:
            cmp_func = cmp_by_field2;
            break;
        case 4:
            cmp_func = cmp_by_field3;
            break;
        default:
            fprintf(stderr, "Campo di ordinamento non valido: %zu\n", field);
            free(records);
            return;
    }

    hybridsort(records, count, sizeof(Record), k, cmp_func);

    if (save_records_to_file(outfile, records, count) != 0) {
        fprintf(stderr, "Errore durante la scrittura del file di output.\n");
    }

    free(records);
}