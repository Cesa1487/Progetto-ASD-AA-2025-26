#include "../include/hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Siccome il compilatore di moodle non riconosce la funzione strdup
char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *new_s = malloc(len);
    if (new_s == NULL) return NULL;
    return (char *)memcpy(new_s, s, len);
}

/**
 * @brief Compare two C-strings for equality.
 */
static int str_eq(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

/**
 * @brief Compute hash value of a C-string using djb2 algorithm.
 */
static unsigned long str_hash(const void *k) {
    const unsigned char *s = k;
    unsigned long h = 5381;
    int c;
    while ((c = *s++)) h = ((h << 5) + h) + c;
    return h;
}

/**
 * @brief Reads the file and populates the hash table with word counts.
 * * @param fp Pointer to the open file.
 * @param ht Pointer to the hash table.
 * @param min_len Minimum length of words to consider.
 */
void process_text(FILE *fp, HashTable *ht, int min_len) {
    int c;
    char buffer[256];
    int len = 0;

    while ((c = fgetc(fp)) != EOF) {
        if (isalnum(c)) {
            if (len < 255) {
                buffer[len++] = tolower(c);
            }
        } else {
            if (len > 0) {
                buffer[len] = '\0';
                if (len >= min_len) {
                    int *count = hash_table_get(ht, buffer);
                    if (count) {
                        (*count)++;
                    } else {
                        char *key = my_strdup(buffer);
                        int *val = malloc(sizeof(int));
                        if (val && key) {
                            *val = 1;
                            hash_table_put(ht, key, val);
                        }
                    }
                }
                len = 0;
            }
        }
    }

    if (len > 0) {
        buffer[len] = '\0';
        if (len >= min_len) {
            int *count = hash_table_get(ht, buffer);
            if (count) {
                (*count)++;
            } else {
                char *key = my_strdup(buffer);
                int *val = malloc(sizeof(int));
                if (val && key) {
                    *val = 1;
                    hash_table_put(ht, key, val);
                }
            }
        }
    }
}

/**
 * @brief Helper to find and print the most frequent word.
 */
void print_most_frequent(HashTable *ht, int min_len) {
    void **keys = hash_table_keyset(ht);
    const char *max_word = NULL;
    int max_count = 0;

    for (int i = 0; keys[i]; i++) {
        int *count = hash_table_get(ht, keys[i]);
        if (*count > max_count) {
            max_count = *count;
            max_word = keys[i];
        }
    }

    if (max_word)
        printf("Most frequent word (len >= %d): '%s' (%d occurrences)\n", min_len, max_word, max_count);
    else
        printf("No valid words found with length >= %d.\n", min_len);

    free(keys);
}

/**
 * @brief Helper to clean up hash table content.
 */
void free_hash_table_content(HashTable *ht) {
    void **keys = hash_table_keyset(ht);
    for (int i = 0; keys[i]; i++) {
        int *c = hash_table_get(ht, keys[i]);
        free(c);
        free(keys[i]);
    }
    free(keys);
    hash_table_free(ht);
}


/**
 * @brief Main program
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file> <min_len>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    int min_len = atoi(argv[2]);
    HashTable *ht = hash_table_create(str_eq, str_hash);

    if (ht) {
        process_text(fp, ht, min_len);
        print_most_frequent(ht, min_len);
        free_hash_table_content(ht);
    }

    fclose(fp);
    return 0;
}