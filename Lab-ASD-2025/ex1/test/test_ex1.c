#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/sort.h"

/**
 * @file test_sort.c
 * @brief Unit tests for the generic hybrid sorting algorithm.
 *
 * This module defines a set of unit tests that exercise the ::hybridsort
 * function on integer arrays under different scenarios (empty, single element,
 * already sorted, reverse sorted, with duplicates, different values of k, etc.).
 */

/**
 * @brief Comparator function for integers.
 *
 * Can be passed to generic sorting algorithms such as ::hybridsort.
 *
 * @param a Pointer to the first integer.
 * @param b Pointer to the second integer.
 * @return Negative if *a < *b, 0 if equal, positive if *a > *b.
 */
int compare_ints(const void *a, const void *b) {
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

/**
 * @brief Unit test: sorting an empty array.
 *
 * Ensures that ::hybridsort correctly handles an empty array (nitems = 0)
 * without accessing memory or producing errors.
 */
void test_sort_empty_array() {
    int arr[1];
    hybridsort(arr, 0, sizeof(int), 5, compare_ints);

}

/**
 * @brief Unit test: sorting a single-element array.
 *
 * Ensures that ::hybridsort leaves a single-element array unchanged.
 */
void test_sort_single_element() {
    int arr[1] = {42};
    hybridsort(arr, 1, sizeof(int), 5, compare_ints);
    assert(arr[0] == 42);
}

/**
 * @brief Unit test: sorting an array of multiple integers.
 *
 * Verifies that ::hybridsort correctly sorts an array into ascending order.
 */
void test_sort_array() {
    int arr[] = {5, 2, 9, 1, 3, 7};
    int expected[] = {1, 2, 3, 5, 7, 9};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    hybridsort(arr, n, sizeof(int), 5, compare_ints);
    assert(memcmp(arr, expected, sizeof(arr)) == 0);
}

/**
 * @brief Unit test: sorting an already sorted array.
 *
 * Checks that ::hybridsort preserves the order of an already sorted array.
 */
void test_sort_already_sorted() {
    int arr[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3, 4, 5};
    hybridsort(arr, 5, sizeof(int), 5, compare_ints);
    assert(memcmp(arr, expected, sizeof(arr)) == 0);
}

/**
 * @brief Unit test: sorting a reverse-sorted array.
 *
 * Verifies that ::hybridsort correctly sorts a descending array into ascending order.
 */
void test_sort_reverse_sorted() {
    int arr[] = {9, 7, 5, 3, 1};
    int expected[] = {1, 3, 5, 7, 9};
    hybridsort(arr, 5, sizeof(int), 5, compare_ints);
    assert(memcmp(arr, expected, sizeof(arr)) == 0);
}

/**
 * @brief Unit test: sorting an array containing duplicate values.
 *
 * Ensures that ::hybridsort handles duplicates correctly and maintains
 * the correct multiplicity of each value.
 */
void test_sort_with_duplicates() {
    int arr[] = {4, 2, 4, 2, 1};
    int expected[] = {1, 2, 2, 4, 4};
    hybridsort(arr, 5, sizeof(int), 5, compare_ints);
    assert(memcmp(arr, expected, sizeof(arr)) == 0);
}

/**
 * @brief Unit test: sorting with k = 0.
 *
 * Verifies that passing k = 0 (edge case) does not break ::hybridsort and that
 * the array is still correctly sorted. Internally, k should be normalized.
 */
void test_sort_k_zero() {
    int arr[] = {8, 6, 7, 5};
    int expected[] = {5, 6, 7, 8};
    hybridsort(arr, 4, sizeof(int), 0, compare_ints);
    assert(memcmp(arr, expected, sizeof(arr)) == 0);
}

/**
 * @brief Unit test: sorting with a very large k.
 *
 * Checks the behavior of ::hybridsort when the threshold k is much larger
 * than the array size (forcing the Selection Sort branch).
 */
void test_sort_large_k() {
    int arr[] = {10, 1, 5, 7, 3};
    int expected[] = {1, 3, 5, 7, 10};
    hybridsort(arr, 5, sizeof(int), 9999, compare_ints);
    assert(memcmp(arr, expected, sizeof(arr)) == 0);
}

/**
 * @brief Main entry point for unit testing.
 *
 * Runs the complete suite of unit tests for ::hybridsort. If all assertions
 * succeed, prints a success message and returns EXIT_SUCCESS.
 *
 * @return EXIT_SUCCESS if all tests pass.
 */
int main() {
    test_sort_empty_array();
    test_sort_single_element();
    test_sort_array();
    test_sort_already_sorted();
    test_sort_reverse_sorted();
    test_sort_with_duplicates();
    test_sort_k_zero();
    test_sort_large_k();

    printf("All tests passed successfully.\n");
    return EXIT_SUCCESS;
}