#include "../include/hash_table.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

/**
 * @brief Internal entry structure for chaining.
 */
typedef struct entry {
    void *key;
    void *value;
    struct entry *next;
} Entry;

/**
 * @brief Internal hash table structure.
 */
struct hash_table {
    Entry **buckets;
    size_t capacity;
    size_t size;
    int  (*equals)(const void *, const void *);
    unsigned long (*hash)(const void *);
};

static void resize_if_needed(HashTable *ht);

/**
 * @brief Create a new hash table.
 */
HashTable *hash_table_create(int (*eq)(const void *, const void *),
                             unsigned long (*hash)(const void *)) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->capacity = INITIAL_CAPACITY;
    ht->size = 0;
    ht->equals = eq;
    ht->hash = hash;
    ht->buckets = calloc(ht->capacity, sizeof(Entry *));
    return ht;
}

/**
 * @brief Insert or update a key-value pair in the hash table.
 */
void hash_table_put(HashTable *ht, const void *key, const void *value) {
    resize_if_needed(ht);
    size_t idx = ht->hash(key) & (ht->capacity - 1);

    Entry *curr = ht->buckets[idx];
    while (curr) {
        if (ht->equals(curr->key, key)) {
            curr->value = (void *)value;
            return;
        }
        curr = curr->next;
    }

    Entry *e = malloc(sizeof(Entry));
    e->key = (void *)key;
    e->value = (void *)value;
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
    ht->size++;
}

/**
 * @brief Retrieve the value associated with a key.
 */
void *hash_table_get(const HashTable *ht, const void *key) {
    size_t idx = ht->hash(key) & (ht->capacity - 1);
    Entry *curr = ht->buckets[idx];
    while (curr) {
        if (ht->equals(curr->key, key))
            return curr->value;
        curr = curr->next;
    }
    return NULL;
}

/**
 * @brief Check if a key exists in the hash table.
 */
int hash_table_contains_key(const HashTable *ht, const void *key) {
    size_t idx = ht->hash(key) & (ht->capacity - 1);
    Entry *curr = ht->buckets[idx];
    while (curr) {
        if (ht->equals(curr->key, key))
            return 1;
        curr = curr->next;
    }
    return 0;
}

/**
 * @brief Remove a key-value pair from the hash table.
 */
void hash_table_remove(HashTable *ht, const void *key) {
    size_t idx = ht->hash(key) & (ht->capacity - 1);
    Entry *curr = ht->buckets[idx];
    Entry *prev = NULL;
    while (curr) {
        if (ht->equals(curr->key, key)) {
            if (prev) prev->next = curr->next;
            else ht->buckets[idx] = curr->next;
            free(curr);
            ht->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/**
 * @brief Return the number of stored key-value pairs.
 */
int hash_table_size(const HashTable *ht) {
    return (int)ht->size;
}

/**
 * @brief Get an array with all keys stored in the table.
 */
void **hash_table_keyset(const HashTable *ht) {
    void **keys = malloc(sizeof(void *) * (ht->size + 1));
    size_t idx = 0;
    for (size_t b = 0; b < ht->capacity; b++) {
        Entry *curr = ht->buckets[b];
        while (curr) {
            keys[idx++] = curr->key;
            curr = curr->next;
        }
    }
    keys[idx] = NULL;
    return keys;
}

/**
 * @brief Free the hash table structure.
 */
void hash_table_free(HashTable *ht) {
    for (size_t b = 0; b < ht->capacity; b++) {
        Entry *curr = ht->buckets[b];
        while (curr) {
            Entry *next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(ht->buckets);
    free(ht);
}

/**
 * @brief Double the capacity of the table if load factor exceeds threshold.
 */
static void resize_if_needed(HashTable *ht) {
    if ((double)ht->size / ht->capacity < LOAD_FACTOR)
        return;
    size_t new_cap = ht->capacity * 2;
    Entry **new_buckets = calloc(new_cap, sizeof(Entry *));
    for (size_t b = 0; b < ht->capacity; b++) {
        Entry *curr = ht->buckets[b];
        while (curr) {
            Entry *next = curr->next;
            size_t idx = ht->hash(curr->key) & (new_cap - 1);
            curr->next = new_buckets[idx];
            new_buckets[idx] = curr;
            curr = next;
        }
    }
    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_cap;
}