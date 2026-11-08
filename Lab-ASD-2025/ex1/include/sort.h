#ifndef SORT_H
#define SORT_H

#include <stdio.h>

/**
 * @file sort.h
 * @brief Generic hybrid sorting interface and record sorting utilities.
 *
 * This module provides a generic implementation of an in-place **hybrid sort**,
 * combining Quick Sort and Selection Sort. It also defines a helper function
 * to sort arrays of `Record` structures loaded from binary files.
 */

/**
 * @brief Generic implementation of the Hybrid Sort algorithm (Quick + Selection).
 *
 * Sorts an array of generic elements in ascending order using a hybrid approach:
 * - For subarrays smaller than a given threshold `k`, Selection Sort is used.
 * - For larger subarrays, Quick Sort with median-of-three pivot selection is applied.
 *
 * @param base   Pointer to the first element of the array.
 * @param nitems Number of elements in the array.
 * @param size   Size in bytes of each element.
 * @param k      Threshold value: for subarrays smaller than `k`, Selection Sort is used.
 * @param compar Comparator function defining the ordering rule.
 *               Must return <0 if a < b, 0 if equal, >0 if a > b.
 */
void hybridsort(void *base, size_t nitems, size_t size, size_t k, int (*compar)(const void *, const void *));

/**
 * @brief Loads, sorts, and writes records between binary files.
 *
 * This function:
 *  1. Loads all records from the input binary file.
 *  2. Sorts them according to the field selected by the user.
 *  3. Saves the sorted records into the output binary file.
 *
 * @param infile  Input binary file containing unsorted records (opened in "rb").
 * @param outfile Output binary file where sorted records will be written (opened in "wb").
 * @param field   Sorting key:
 *                - 1 → `field1` (float)
 *                - 2 → `field2` (integer)
 *                - 3 → `field3` (string)
 *                - 4 → `id`
 * @param k       Threshold value for the hybrid algorithm (used in @ref hybridsort).
 */
void sort_records(FILE *infile, FILE *outfile, size_t field, size_t k);

#endif