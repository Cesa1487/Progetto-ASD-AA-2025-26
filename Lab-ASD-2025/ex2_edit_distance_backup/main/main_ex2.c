#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/edit_distance.h"

#define MAX_WORD_LEN 128
#define MAX_DICTIONARY_SIZE 100000

/**
 * @brief Clean a word by removing all non-alphabetic characters except apostrophes
 * and converting letters to lowercase.
 *
 * @param s string to clean
 */
void clean_word(char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if (isalpha((unsigned char)s[i]) || s[i] == '\'')
            s[j++] = tolower((unsigned char)s[i]);
        i++;
    }
    s[j] = '\0';
}

/**
 * @brief Main program for exercise 2.
 *
 * Loads a dictionary and a text file, then for each word in the text
 * computes the minimum edit distance (insertions/deletions only) to words
 * in the dictionary, printing all dictionary words at minimum distance.
 *
 * @param argc number of arguments
 * @param argv command line arguments: argv[1] = dictionary path, argv[2] = text path
 * @return 0 on success, nonzero on error
 */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <dictionary.txt> <correctme.txt>\n", argv[0]);
        return 1;
    }

    /* Load dictionary into memory */
    FILE *fdict = fopen(argv[1], "r");
    if (!fdict) {
        perror("Failed to open dictionary");
        return 2;
    }
    char **dictionary = malloc(MAX_DICTIONARY_SIZE * sizeof(char*));
    int dict_size = 0;
    char buffer[MAX_WORD_LEN];
    while (fgets(buffer, sizeof(buffer), fdict)) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        clean_word(buffer);
        if (strlen(buffer) == 0) continue;
        dictionary[dict_size] = strdup(buffer);
        dict_size++;
        if (dict_size >= MAX_DICTIONARY_SIZE) break;
    }
    fclose(fdict);

    /* Open text to correct */
    FILE *ftext = fopen(argv[2], "r");
    if (!ftext) {
        perror("Failed to open file to correct");
        return 3;
    }

    /* Process each word in the text */
    while (fscanf(ftext, "%127s", buffer) == 1) {
        clean_word(buffer);
        if (strlen(buffer) == 0) continue;

        int min_dist = -1;
        char *best_matches[MAX_DICTIONARY_SIZE];
        int n_best = 0;

        /* Compute minimum edit distance and collect best matches */
        for (int i = 0; i < dict_size; ++i) {
            int dist = edit_distance_dyn(dictionary[i], buffer);
            if (min_dist == -1 || dist < min_dist) {
                min_dist = dist;
                n_best = 0;
                best_matches[n_best++] = dictionary[i];
            } else if (dist == min_dist) {
                    best_matches[n_best++] = dictionary[i];
            }
        }

        /* Print the results for this word */
        printf("%s:", buffer);
        for (int i = 0; i < n_best; ++i) {
            printf(" %s", best_matches[i]);
        }
        printf(" (distance=%d)\n", min_dist);
    }
    fclose(ftext);

    /* Free allocated memory */
    for (int i = 0; i < dict_size; ++i) free(dictionary[i]);
    free(dictionary);

    return 0;
}