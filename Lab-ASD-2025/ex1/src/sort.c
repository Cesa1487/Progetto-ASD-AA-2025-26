#include <stdlib.h>
#include "../include/sort.h"

/**
 * @file sort.c
 * @brief Generic hybrid sorting algorithm (Quick Sort + Selection Sort).
 *
 * This module provides the implementation of ::hybridsort, a generic
 * in-place sorting routine that combines:
 *  - **Quick Sort** with median-of-three pivot selection for large subarrays.
 *  - **Selection Sort** for small subarrays (size < k), improving performance
 *    on small partitions and reducing recursion overhead.
 *
 * The algorithm operates on raw memory buffers and requires a user-provided
 * comparator function, making it suitable for sorting arbitrary data types.
 */

/**
 * @brief Swaps the contents of two memory blocks of equal size.
 *
 * @param a Pointer to the first element.
 * @param b Pointer to the second element.
 * @param size Size in bytes of each element.
 */
static void swap(void *a, void *b, size_t size) {
    if (a == b) return;
    unsigned char *pa = a;
    unsigned char *pb = b;
    for (size_t i = 0; i < size; ++i) {
        unsigned char tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

/**
 * @brief Simple generic Selection Sort used for small subarrays.
 *
 * This function is used internally by ::hybridsort when the subarray
 * size falls below the threshold `k`.
 */
static void selection_sort_generic(unsigned char *base,
                                   size_t nitems,
                                   size_t size,
                                   int (*compar)(const void *, const void *)) {
    for (size_t i = 0; i < nitems; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < nitems; ++j) {
            if (compar(base + j * size, base + min_idx * size) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i)
            swap(base + i * size, base + min_idx * size, size);
    }
}

/**
 * @brief Median-of-three pivot selection.
 *
 * Orders the elements at positions `a`, `b`, and `c`, ensuring that
 * the median value ends up at position `b`.
 *
 * @return The index corresponding to the median among (a, b, c).
 */
static size_t median_of_three(unsigned char *base,
                              size_t size,
                              int (*compar)(const void *, const void *),
                              size_t a, size_t b, size_t c) {
    unsigned char *pa = base + a * size;
    unsigned char *pb = base + b * size;
    unsigned char *pc = base + c * size;

    if (compar(pa, pb) > 0) swap(pa, pb, size);
    if (compar(pb, pc) > 0) swap(pb, pc, size);
    if (compar(pa, pb) > 0) swap(pa, pb, size);

    return b;
}

/**
 * @brief Internal recursive Quick Sort for the hybrid algorithm.
 *
 * Switches to Selection Sort when `nitems < k`, otherwise applies the
 * standard Quick Sort partitioning with a median-of-three pivot.
 */
static void quicksort_rec(unsigned char *base,
                           size_t nitems,
                           size_t size,
                           size_t k,
                           int (*compar)(const void *, const void *)) {
    if (nitems <= 1)
        return;

    if (nitems < k) {
        selection_sort_generic(base, nitems, size, compar);
        return;
    }

    size_t lo = 0;
    size_t hi = nitems - 1;
    size_t mid = nitems / 2;
    size_t pivot_index = median_of_three(base, size, compar, lo, mid, hi);

    swap(base + pivot_index * size, base + hi * size, size);

    size_t store = 0;
    for (size_t i = 0; i < hi; ++i) {
        if (compar(base + i * size, base + hi * size) <= 0) {
            swap(base + i * size, base + store * size, size);
            store++;
        }
    }

    swap(base + store * size, base + hi * size, size);

    if (store > 1)
        quicksort_rec(base, store, size, k, compar);
    if (store + 1 < nitems)
        quicksort_rec(base + (store + 1) * size,
                            nitems - store - 1, size, k, compar);
}

/**
 * @brief Generic Hybrid Sort: Quick Sort + Selection Sort.
 *
 * Sorts an array of arbitrary elements using a hybrid approach:
 *
 *  - For subarrays smaller than `k`, Selection Sort is used.
 *  - For larger subarrays, Quick Sort with median-of-three pivot is applied.
 *
 * This function modifies the array in-place.
 *
 * @param base   Pointer to the first element of the array.
 * @param nitems Number of elements in the array.
 * @param size   Size in bytes of each element.
 * @param k      Threshold for switching to Selection Sort (k > 0).
 * @param compar User-provided comparator function.
 *               Must return <0, 0, or >0 like strcmp.
 */
void hybridsort(void *base,
                size_t nitems,
                size_t size,
                size_t k,
                int (*compar)(const void *, const void *)) {
    if (!base || !compar || nitems <= 1)
        return;

    if (k == 0)
        k = 1;

    quicksort_rec((unsigned char *)base, nitems, size, k, compar);
}