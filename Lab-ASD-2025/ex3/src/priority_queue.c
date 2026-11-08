#include <stdio.h>
#include <stdlib.h>

#include "../include/priority_queue.h"
#include "../../ex2/include/hash_table.h"

#define PQ_INITIAL_CAPACITY 32

/**
 * @brief Internal structure of the Priority Queue.
 */
struct PriorityQueue {
    void **data;
    size_t size;
    size_t capacity;

    int (*cmp)(const void*, const void*);
    unsigned long (*hash)(const void*);

    HashTable *index_map;
};

/**
 * @brief Calculates the parent index in a ternary heap.
 * @param i Index of the current node.
 * @return Index of the parent node.
 */
static int parent(int i) {
    return (i - 1) / 3;
}

/**
 * @brief Calculates the index of the k-th child in a ternary heap.
 * @param i Index of the current node.
 * @param k Child index (1, 2, or 3).
 * @return Index of the child in the array.
 */
static int k_child(int i, int k) {
    return 3 * i + k;
}

/**
 * @brief Swaps two nodes in the heap and updates their positions in the index map.
 */
static void swap_nodes(PriorityQueue *pq, int i, int j) {
    void *temp = pq->data[i];
    pq->data[i] = pq->data[j];
    pq->data[j] = temp;

    hash_table_put(pq->index_map, pq->data[i], (void*)(size_t)(i + 1));
    hash_table_put(pq->index_map, pq->data[j], (void*)(size_t)(j + 1));
}

/**
 * @brief Restores the heap property by moving up towards the root (Sift-Up).
 * Used after insertion or when a node's priority increases.
 */
static void sift_up(PriorityQueue *pq, int idx) {
    while (idx > 0) {
        int p = parent(idx);
        if (pq->cmp(pq->data[idx], pq->data[p]) > 0) {
            swap_nodes(pq, idx, p);
            idx = p;
        } else {
            break;
        }
    }
}

/**
 * @brief Restores the heap property by moving down towards leaves (Sift-Down).
 * Used after extraction or when a node's priority decreases.
 * In a ternary heap, it compares the node with the largest of its (up to) 3 children.
 */
static void sift_down(PriorityQueue *pq, int idx) {
    while (1) {
        int max_idx = idx;

        for (int k = 1; k <= 3; k++) {
            int child = k_child(idx, k);
            if (child < (int)pq->size) {
                if (pq->cmp(pq->data[child], pq->data[max_idx]) > 0) {
                    max_idx = child;
                }
            } else {
                break;
            }
        }

        if (max_idx != idx) {
            swap_nodes(pq, idx, max_idx);
            idx = max_idx;
        } else {
            break;
        }
    }
}

/**
 * @brief Resizes the internal array if capacity is reached.
 */
static void resize_if_needed_pq(PriorityQueue *pq) {
    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        pq->data = realloc(pq->data, pq->capacity * sizeof(void*));
    }
}

PriorityQueue* priority_queue_create(int (*compare)(const void*, const void*),
                                     unsigned long (*hash)(const void*)) {
    if (!compare || !hash) return NULL;

    PriorityQueue *pq = malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;

    pq->data = malloc(PQ_INITIAL_CAPACITY * sizeof(void*));
    pq->size = 0;
    pq->capacity = PQ_INITIAL_CAPACITY;
    pq->cmp = compare;
    pq->hash = hash;

    pq->index_map = hash_table_create(compare, hash);
    if (!pq->index_map) {
        free(pq->data);
        free(pq);
        return NULL;
    }

    return pq;
}

int priority_queue_push(PriorityQueue* pq, void* data) {
    if (!pq || !data) return 0;

    if (hash_table_contains_key(pq->index_map, data)) return 0;

    resize_if_needed_pq(pq);

    int idx = (int)pq->size;
    pq->data[idx] = data;

    hash_table_put(pq->index_map, data, (void*)(size_t)(idx + 1));

    pq->size++;
    sift_up(pq, idx);
    return 1;
}

int priority_queue_contains(const PriorityQueue* pq, const void* data) {
    if (!pq || !data) return 0;
    return hash_table_contains_key(pq->index_map, data);
}

void* priority_queue_top(const PriorityQueue* pq) {
    if (!pq || pq->size == 0) return NULL;
    return pq->data[0];
}

int priority_queue_remove(PriorityQueue* pq, const void* data);

void priority_queue_pop(PriorityQueue* pq) {
    if (!pq || pq->size == 0) return;
    priority_queue_remove(pq, pq->data[0]);
}

int priority_queue_remove(PriorityQueue* pq, const void* data) {
    if (!pq || !data) return 0;

    if (!hash_table_contains_key(pq->index_map, data)) return 0;

    int idx = (int)(size_t)hash_table_get(pq->index_map, data) - 1;

    hash_table_remove(pq->index_map, data);

    if (idx == (int)pq->size - 1) {
        pq->size--;
        return 1;
    }

    void *last_elem = pq->data[pq->size - 1];
    pq->data[idx] = last_elem;

    hash_table_put(pq->index_map, last_elem, (void*)(size_t)(idx + 1));

    pq->size--;

    if (idx > 0 && pq->cmp(pq->data[idx], pq->data[parent(idx)]) > 0) {
        sift_up(pq, idx);
    } else {
        sift_down(pq, idx);
    }

    return 1;
}

int priority_queue_size(const PriorityQueue* pq) {
    return pq ? (int)pq->size : 0;
}

void priority_queue_free(PriorityQueue* pq) {
    if (!pq) return;
    hash_table_free(pq->index_map);
    free(pq->data);
    free(pq);
}