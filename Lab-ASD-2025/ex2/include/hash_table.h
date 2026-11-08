#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct hash_table HashTable;

/**
 * @brief Create a new hash table.
 *
 * @param f1 Comparator function: returns 0 if two keys are equal.
 * @param f2 Hash function: returns a hash value for a given key.
 * @return Pointer to a newly allocated HashTable.
 *
 * @note The hash table does not take ownership of keys or values.
 *       The caller is responsible for freeing any allocated keys/values.
 */
HashTable *hash_table_create(int (*f1)(const void *, const void *), unsigned long (*f2)(const void *));

/**
 * @brief Insert or update a key-value pair.
 *
 * @param ht Hash table instance.
 * @param key Pointer to the key.
 * @param value Pointer to the value.
 *
 * If the key already exists, its value is replaced.
 */
void hash_table_put(HashTable *, const void *key, const void *value);

/**
 * @brief Retrieve the value associated with a key.
 *
 * @param ht Hash table instance.
 * @param key Pointer to the key.
 * @return Pointer to the value, or NULL if the key does not exist.
 */
void *hash_table_get(const HashTable *, const void *key);

/**
 * @brief Check if a key is present in the table.
 *
 * @param ht Hash table instance.
 * @param key Pointer to the key.
 * @return 1 if the key exists, 0 otherwise.
 */
int hash_table_contains_key(const HashTable *, const void *key);

/**
 * @brief Remove a key-value pair from the table.
 *
 * @param ht Hash table instance.
 * @param key Pointer to the key to remove.
 */
void hash_table_remove(HashTable *, const void *key);

/**
 * @brief Get the number of key-value pairs in the table.
 *
 * @param ht Hash table instance.
 * @return Number of elements currently stored.
 */
int hash_table_size(const HashTable *);

/**
 * @brief Get all keys in the table.
 *
 * @param ht Hash table instance.
 * @return NULL-terminated array of keys. Caller must free() the array,
 *         but not the keys themselves.
 */
void **hash_table_keyset(const HashTable *);

/**
 * @brief Get all keys in the table.
 *
 * @param ht Hash table instance.
 * @return NULL-terminated array of keys. Caller must free() the array,
 *         but not the keys themselves.
 */
void hash_table_free(HashTable *);

#endif