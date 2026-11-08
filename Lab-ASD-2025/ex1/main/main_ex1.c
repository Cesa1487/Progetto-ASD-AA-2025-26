#include <stdio.h>
#include <stdlib.h>
#include "../include/sort.h"

/**
 * @file main.c
 * @brief Command-line tool to sort binary Record files.
 *
 * The program expects exactly four command-line arguments:
 *  - input_file:  path to the binary file containing unsorted records ("rb")
 *  - output_file: path to the binary file where sorted records will be written ("wb")
 *  - field:       sorting key:
 *                  - 1 → id
 *                  - 2 → field1 (float)
 *                  - 3 → field2 (integer)
 *                  - 4 → field3 (string, lexicographic)
 *  - k:           threshold parameter for the hybrid sorting algorithm
 *                 (used by ::hybridsort to switch between Quick Sort and
 *                 Selection Sort on small partitions)
 *
 * Internally, the program:
 *  - opens the input and output files in binary mode,
 *  - calls ::sort_records to load, sort and save all records,
 *  - closes the files and returns an appropriate exit status.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE if an error occurs.
 */
int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <input_file> <output_file> <field> <k>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *infile = fopen(argv[1], "rb");
    if (!infile) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    FILE *outfile = fopen(argv[2], "wb");
    if (!outfile) {
        perror("Error opening output file");
        fclose(infile);
        return EXIT_FAILURE;
    }

    size_t field = (size_t)atoi(argv[3]);
    size_t k = (size_t)atoi(argv[4]);

    sort_records(infile, outfile, field, k);

    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}