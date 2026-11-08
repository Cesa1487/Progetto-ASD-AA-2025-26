#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/priority_queue.h"

/**
 * @file test_ex3.c
 * @brief Unit tests for the Priority Queue implementation.
 */

/**
 * @brief Integer comparator for testing.
 */
int int_cmp(const void* a, const void* b) {
    int va = *(const int*)a;
    int vb = *(const int*)b;
    return va - vb;
}

/**
 * @brief Simple hash function for integers (passed as pointers).
 */
unsigned long int_hash(const void* k) {
    return (unsigned long)(*(const int*)k);
}

/**
 * @brief Test basic push and pop operations (Max-Heap property).
 */
void test_simple_push_pop() {
    PriorityQueue *pq = priority_queue_create(int_cmp, int_hash);
    int a=10, b=20, c=5;

    priority_queue_push(pq, &a);
    priority_queue_push(pq, &b);
    priority_queue_push(pq, &c);

    assert(priority_queue_size(pq) == 3);
    assert(priority_queue_top(pq) == &b);

    priority_queue_pop(pq);
    assert(priority_queue_top(pq) == &a);

    priority_queue_pop(pq);
    assert(priority_queue_top(pq) == &c);

    priority_queue_free(pq);
    printf("test_simple_push_pop PASSED\n");
}

/**
 * @brief Test removal of an arbitrary element from the middle of the heap.
 */
void test_remove_middle() {
    PriorityQueue *pq = priority_queue_create(int_cmp, int_hash);
    int vals[] = {10, 20, 30, 40, 50, 5, 15};
    for(int i=0; i<7; i++) priority_queue_push(pq, &vals[i]);

    int res = priority_queue_remove(pq, &vals[2]);
    assert(res == 1);
    assert(priority_queue_size(pq) == 6);
    assert(priority_queue_contains(pq, &vals[2]) == 0);

    assert(priority_queue_top(pq) == &vals[4]);

    priority_queue_free(pq);
    printf("test_remove_middle PASSED\n");
}

int main() {
    test_simple_push_pop();
    test_remove_middle();
    return 0;
}