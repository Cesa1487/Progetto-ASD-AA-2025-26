#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/**
 * @file priority_queue.h
 * @brief Interface for a generic Priority Queue based on a Ternary Heap.
 *
 * This data structure manages generic elements and supports efficient
 * insertion, removal, and lookup operations by combining a ternary heap
 * (for priority management) with a hash table (for O(1) lookups).
 */

/**
 * @brief Opaque handle for the priority queue structure.
 */
typedef struct PriorityQueue PriorityQueue;

/**
 * @brief Creates a new empty priority queue.
 *
 * @param compare Comparison function for priority (must return >0 if the first argument has higher priority).
 * @param hash Hash function for the elements (required for the internal index map).
 * @return A pointer to the new PriorityQueue, or NULL on error.
 * @complexity O(1)
 */
PriorityQueue* priority_queue_create(int (*compare)(const void*, const void*),
                                     unsigned long (*hash)(const void*));

/**
 * @brief Inserts an element into the priority queue.
 *
 * @param pq Pointer to the priority queue.
 * @param data Pointer to the element to insert.
 * @return 1 if insertion is successful, 0 otherwise (e.g., element already exists or memory error).
 * @complexity O(log_3 N) -> O(log N)
 */
int priority_queue_push(PriorityQueue* pq, void* data);

/**
 * @brief Checks if an element is present in the queue.
 *
 * Thanks to the auxiliary hash table, this check is performed efficiently.
 *
 * @param pq Pointer to the priority queue.
 * @param data Element to look for.
 * @return 1 if the element is present, 0 otherwise.
 * @complexity O(1) average
 */
int priority_queue_contains(const PriorityQueue* pq, const void* data);

/**
 * @brief Returns the element with the highest priority without removing it.
 *
 * @param pq Pointer to the priority queue.
 * @return Pointer to the highest priority element, or NULL if the queue is empty.
 * @complexity O(1)
 */
void* priority_queue_top(const PriorityQueue* pq);

/**
 * @brief Removes the element with the highest priority from the queue.
 *
 * @param pq Pointer to the priority queue.
 * @complexity O(log_3 N) -> O(log N)
 */
void priority_queue_pop(PriorityQueue* pq);

/**
 * @brief Removes a specific element from the queue, regardless of its position.
 *
 * @param pq Pointer to the priority queue.
 * @param data Element to remove.
 * @return 1 if the element was removed, 0 if it was not found.
 * @complexity O(log_3 N) -> O(log N) (lookup in O(1), heap restore in O(log N))
 */
int priority_queue_remove(PriorityQueue* pq, const void* data);

/**
 * @brief Returns the number of elements currently in the queue.
 *
 * @param pq Pointer to the priority queue.
 * @return Number of elements.
 * @complexity O(1)
 */
int priority_queue_size(const PriorityQueue* pq);

/**
 * @brief Frees the memory allocated for the priority queue.
 *
 * This function does not free the memory of the elements themselves (ownership belongs to the user),
 * but it frees the internal structures (dynamic array and hash table).
 *
 * @param pq Pointer to the priority queue to free.
 */
void priority_queue_free(PriorityQueue* pq);

#endif