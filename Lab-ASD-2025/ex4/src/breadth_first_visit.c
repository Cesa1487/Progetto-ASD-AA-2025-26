/**
* @brief BFS implementation using only the public Graph API.
 *
 * Returns a NULL-terminated array of visited nodes in BFS order.
 * The caller must free the returned array.
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/breadth_first_visit.h"
#include "../include/hash_table.h"
#include "../include/graph.h"

/**
 * @brief Internal node structure for the queue.
 */
typedef struct QueueNode {
    void* data;
    struct QueueNode* next;
} QueueNode;

/**
 * @brief Internal structure representing a FIFO Queue.
 */
typedef struct {
    QueueNode *head, *tail;
} Queue;

/**
 * @brief Enqueues an element at the end of the queue.
 *
 * @param q Pointer to the queue.
 * @param data Pointer to the data to enqueue.
 */
static void enqueue(Queue* q, void* data) {
    QueueNode* n = malloc(sizeof(QueueNode));
    n->data = data;
    n->next = NULL;
    if (q->tail) q->tail->next = n;
    else q->head = n;
    q->tail = n;
}

/**
 * @brief Dequeues an element from the front of the queue.
 *
 * Removes the head node, frees its memory, and returns the stored data.
 *
 * @param q Pointer to the queue.
 * @return Pointer to the data derived from the head, or NULL if empty.
 */
static void* dequeue(Queue* q) {
    if (!q->head) return NULL;
    QueueNode* tmp = q->head;
    void* data = tmp->data;
    q->head = tmp->next;
    if (!q->head) q->tail = NULL;
    free(tmp);
    return data;
}

/**
 * @brief Checks if the queue is empty.
 *
 * @param q Pointer to the queue.
 * @return 1 if empty, 0 otherwise.
 */
static int is_empty(Queue* q) { return q->head == NULL; }

/**
 * @brief Perform a breadth-first traversal from a start node.
 *
 * The function allocates and returns an array of pointers to the nodes visited,
 * in the order they were visited. The array is NULL-terminated.
 * It uses a local Hash Table to track visited nodes efficiently.
 */
void** breadth_first_visit(Graph gr, void* start,
                           int (*compare)(const void*, const void*),
                           unsigned long (*hash)(const void*)) {

    if (!graph_contains_node(gr, start)) return NULL;

    HashTable* visited = hash_table_create(compare, hash);
    Queue q = {NULL, NULL};

    int capacity = graph_num_nodes(gr);
    void** result = malloc((capacity + 1) * sizeof(void*));
    int count = 0;

    enqueue(&q, start);
    hash_table_put(visited, start, (void*)1);

    while (!is_empty(&q)) {
        void* curr = dequeue(&q);
        result[count++] = curr;

        int n_neigh = graph_num_neighbours(gr, curr);
        if (n_neigh > 0) {
            void** neighbors = graph_get_neighbours(gr, curr);
            for (int i = 0; i < n_neigh; i++) {
                if (!hash_table_contains_key(visited, neighbors[i])) {
                    hash_table_put(visited, neighbors[i], (void*)1);
                    enqueue(&q, neighbors[i]);
                }
            }
            free(neighbors);
        }
    }

    result[count] = NULL;

    hash_table_free(visited);
    return result;
}