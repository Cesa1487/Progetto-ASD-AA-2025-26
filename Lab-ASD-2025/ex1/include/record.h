#ifndef RECORD_H
#define RECORD_H

#include <stdio.h>
#include <stdint.h>

/**
 * @file record.h
 * @brief Definition of the Record data structure and related utility functions.
 *
 * This module defines the Record struct used to represent entries
 * in the dataset and provides functions for comparison, input/output,
 * and visualization of records.
 */

/**
 * @struct Record
 * @brief Represents a single record in the dataset.
 *
 * Each record contains:
 * - `id`: unique identifier (unsigned 64-bit integer)
 * - `field1`: a floating-point numeric field
 * - `field2`: a signed integer field
 * - `field3`: a fixed-length string field (up to 15 characters + terminator)
 */
typedef struct {
    uint64_t id;
    float field1;
    int64_t field2;
    char field3[16];
} __attribute__((packed)) Record;

/**
 * @brief Comparator function to sort records by id in ascending order.
 *
 * @param a Pointer to the first record.
 * @param b Pointer to the second record.
 * @return Negative if a < b, 0 if equal, positive if a > b.
 */
int cmp_by_id(const void *a, const void *b);

/**
 * @brief Comparator function to sort records by field1 (floating point).
 *
 * @param a Pointer to the first record.
 * @param b Pointer to the second record.
 * @return Negative if a < b, 0 if equal, positive if a > b.
 */
int cmp_by_field1(const void *a, const void *b);

/**
 * @brief Comparator function to sort records by field2 (integer) in ascending order.
 *
 * @param a Pointer to the first record.
 * @param b Pointer to the second record.
 * @return Negative if a < b, 0 if equal, positive if a > b.
 */
int cmp_by_field2(const void *a, const void *b);

/**
 * @brief Comparator function to sort records by field3 (string) in ascending order.
 *
 * @param a Pointer to the first record.
 * @param b Pointer to the second record.
 * @return Negative if a < b, 0 if equal, positive if a > b.
 */
int cmp_by_field3(const void *a, const void *b);

/**
 * @brief Reads a single record from a binary file.
 *
 * @param file File pointer opened in binary read mode ("rb").
 * @param record Pointer to the Record structure where data will be stored.
 * @return 1 if a record was successfully read, 0 otherwise.
 */
int read_record(FILE *file, Record *record);

/**
 * @brief Writes a single record to a binary file.
 *
 * @param file File pointer opened in binary write or append mode ("wb"/"ab").
 * @param record Pointer to the Record structure to be written.
 * @return 1 if written successfully, 0 otherwise.
 */
int write_record(FILE *file, const Record *record);

/**
 * @brief Loads all records from a binary file into memory.
 *
 * The function allocates a dynamic array to store all records and updates
 * the pointers passed as arguments.
 *
 * @param file File pointer opened in binary read mode ("rb").
 * @param records Output pointer to the dynamically allocated array of records.
 * @param count Output pointer to store the number of records read.
 * @return 0 on success, -1 on error.
 */
int load_records_from_file(FILE *file, Record **records, size_t *count);

/**
 * @brief Writes an array of records to a binary file.
 *
 * @param file File pointer opened in binary write mode ("wb").
 * @param records Pointer to the array of records to be written.
 * @param count Number of records in the array.
 * @return 0 on success, -1 on error.
 */
int save_records_to_file(FILE *file, const Record *records, size_t count);

/**
 * @brief Prints a record to standard output (useful for debugging).
 *
 * @param record Pointer to the record to print.
 */
void print_record(const Record *record);

#endif