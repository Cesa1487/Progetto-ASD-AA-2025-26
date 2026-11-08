#include "../include/hash_table.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int str_eq(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

static unsigned long str_hash(const void *k) {
    const unsigned char *s = k;
    unsigned long h = 5381;
    int c;
    while ((c = *s++)) h = ((h << 5) + h) + c;
    return h;
}

/**
 * @brief Test insertion and retrieval of a single element.
 */
void test_put_and_get() {
    HashTable *ht = hash_table_create(str_eq, str_hash);
    hash_table_put(ht, "pippo", "pluto");
    assert(strcmp(hash_table_get(ht, "pippo"), "pluto") == 0);
    hash_table_free(ht);
}

/**
 * @brief Test overwriting an existing value for the same key.
 */
void test_overwrite() {
    HashTable *ht = hash_table_create(str_eq, str_hash);
    hash_table_put(ht, "chiave", "val1");
    hash_table_put(ht, "chiave", "val2");
    assert(strcmp(hash_table_get(ht, "chiave"), "val2") == 0);
    hash_table_free(ht);
}

/**
 * @brief Test removal of an element and check size update.
 */
void test_remove_and_size() {
    HashTable *ht = hash_table_create(str_eq, str_hash);
    hash_table_put(ht, "a", "1");
    hash_table_put(ht, "b", "2");
    hash_table_remove(ht, "a");
    assert(hash_table_get(ht, "a") == NULL);
    assert(hash_table_size(ht) == 1);
    hash_table_free(ht);
}

/**
 * @brief Test that keyset returns all inserted keys.
 */
void test_keyset() {
    HashTable *ht = hash_table_create(str_eq, str_hash);
    hash_table_put(ht, "uno", "1");
    hash_table_put(ht, "due", "2");
    hash_table_put(ht, "tre", "3");
    void **keys = hash_table_keyset(ht);
    int found = 0, count = 0;
    for (int i = 0; keys[i]; i++) {
        count++;
        if (strcmp(keys[i], "uno") == 0) found++;
        if (strcmp(keys[i], "due") == 0) found++;
        if (strcmp(keys[i], "tre") == 0) found++;
    }
    assert(count == 3);
    assert(found == 3);
    free(keys);
    hash_table_free(ht);
}

/**
 * @brief Test the contains_key function on existing and missing keys.
 */
void test_contains_key() {
    HashTable *ht = hash_table_create(str_eq, str_hash);
    hash_table_put(ht, "foo", "bar");
    assert(hash_table_contains_key(ht, "foo") == 1);
    assert(hash_table_contains_key(ht, "baz") == 0);
    hash_table_free(ht);
}

/**
 * @brief Stress test with many inserted elements.
 */
void test_many_elements() {
    HashTable *ht = hash_table_create(str_eq, str_hash);
    char key[32], val[32];
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof(key), "key%d", i);
        snprintf(val, sizeof(val), "val%d", i);
        hash_table_put(ht, strdup(key), strdup(val));
    }
    assert(hash_table_size(ht) == 1000);
    for (int i = 0; i < 1000; i++) {
        snprintf(key, sizeof(key), "key%d", i);
        char *v = hash_table_get(ht, key);
        assert(v != NULL);
    }

    void **keys = hash_table_keyset(ht);
    for (int i = 0; keys[i]; i++) {
        char *k = keys[i];
        char *v = hash_table_get(ht, k);
        free(v);
        free(k);
    }
    free(keys);
    hash_table_free(ht);
}

/**
 * @brief Run all unit tests.
 */
int main() {
    test_put_and_get();
    test_overwrite();
    test_remove_and_size();
    test_keyset();
    test_contains_key();
    test_many_elements();
    printf("All hash table tests passed!\n");
    return 0;
}