#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../include/record.h"

/**
 * @file record.c
 * @brief Utility functions for handling Record structures.
 *
 * This module provides:
 *  - Comparator functions suitable for use with qsort, to sort arrays of ::Record
 *    by different fields.
 *  - Input/output helpers to read and write records from/to binary files.
 *  - Functions to load an entire file of records into memory and save them back.
 *  - A simple pretty-printer for debugging purposes.
 */

/**
 * @brief Helper function to safely compare two unsigned 64-bit integers.
 *
 * @param x First integer
 * @param y Second integer
 * @return -1 if x < y, 0 if x == y, +1 if x > y
 */
static int cmp_u64(uint64_t x, uint64_t y) {
    return (x > y) - (x < y);
}

static int cmp_i64(int64_t x, int64_t y) {
    return (x > y) - (x < y);
}

/**
 * @brief Comparator for sorting records by id in ascending order.
 *
 * @param a Pointer to the first record
 * @param b Pointer to the second record
 * @return Negative if a < b, 0 if equal, positive if a > b
 */
int cmp_by_id(const void *a, const void *b) {
    const Record *ra = a;
    const Record *rb = b;
    return cmp_u64(ra->id, rb->id);
}

/**
 * @brief Comparator for sorting records by field1 (floating point).
 *
 * @param a Pointer to the first record
 * @param b Pointer to the second record
 * @return Negative if a < b, 0 if equal, positive if a > b
 */
int cmp_by_field1(const void *a, const void *b) {
    const Record *ra = a;
    const Record *rb = b;
    if (ra->field1 < rb->field1) return -1;
    if (ra->field1 > rb->field1) return 1;
    return 0;
}

/**
 * @brief Comparator for sorting records by field2 (signed 64-bit integer) in ascending order.
 *
 * @param a Pointer to the first record
 * @param b Pointer to the second record
 * @return Negative if a < b, 0 if equal, positive if a > b
 */
int cmp_by_field2(const void *a, const void *b) {
    const Record *ra = a;
    const Record *rb = b;
    return cmp_i64(ra->field2, rb->field2);
}

/**
 * @brief Comparator for sorting records by field3 (fixed-length string) in ascending order.
 *
 * @param a Pointer to the first record
 * @param b Pointer to the second record
 * @return Negative if a < b, 0 if equal, positive if a > b
 */
int cmp_by_field3(const void *a, const void *b) {
    const Record *ra = a;
    const Record *rb = b;
    return strncmp(ra->field3, rb->field3, 16);
}

/**
 * @brief Reads a single record from a binary file.
 *
 * The record is read from the current file position. The file must be opened
 * in binary read mode (e.g. `"rb"`).
 *
 * @param file    Pointer to an open FILE object.
 * @param record  Pointer to a ::Record structure where the data will be stored.
 * @return 0 on success, -1 on error or if end-of-file is reached before reading.
 */
int read_record(FILE *file, Record *record) {
    if (!file || !record) return -1;
    size_t r = fread(record, sizeof(Record), 1, file);
    return (r == 1) ? 0 : -1;
}

/**
 * @brief Writes a single record to a binary file.
 *
 * The record is written at the current file position. The file must be opened
 * in binary write mode (e.g. `"wb"` or `"ab"`).
 *
 * @param file    Pointer to an open FILE object.
 * @param record  Pointer to the ::Record to write.
 * @return 0 on success, -1 on error.
 */
int write_record(FILE *file, const Record *record) {
    if (!file || !record) return -1;
    size_t w = fwrite(record, sizeof(Record), 1, file);
    return (w == 1) ? 0 : -1;
}

/**
 * @brief Loads all records from a binary file into memory.
 *
 * The function:
 *  - Determines the file size.
 *  - Verifies that the size is a multiple of sizeof(Record).
 *  - Allocates an array large enough to hold all records.
 *  - Reads all records into the allocated buffer.
 *
 * On success, @p *records will point to the newly allocated array and
 * @p *count will contain the number of records read. The caller is
 * responsible for freeing the allocated memory using free().
 *
 * @param file     Pointer to an open FILE object (opened in `"rb"` mode).
 * @param records  Output pointer that will receive the allocated array of records.
 * @param count    Output pointer that will receive the number of records loaded.
 * @return 0 on success, -1 on error.
 */
int load_records_from_file(FILE *file, Record **records, size_t *count) {
    if (!file || !records || !count) return -1;

    if (fseek(file, 0, SEEK_END) != 0) return -1;
    long size = ftell(file);
    if (size < 0) return -1;
    if (size % sizeof(Record) != 0) return -1;

    size_t n = (size_t)(size / (long)sizeof(Record));
    if (fseek(file, 0, SEEK_SET) != 0) return -1;

    Record *buf = malloc(n * sizeof(Record));
    if (!buf) return -1;

    size_t rd = fread(buf, sizeof(Record), n, file);
    if (rd != n) {
        free(buf);
        return -1;
    }

    *records = buf;
    *count = n;
    return 0;
}

/**
 * @brief Saves an array of records to a binary file.
 *
 * All @p count records are written sequentially starting from the current
 * file position. The file must be opened in binary write mode (e.g. `"wb"`).
 *
 * @param file     Pointer to an open FILE object.
 * @param records  Pointer to the array of records to write.
 * @param count    Number of records in the array.
 * @return 0 on success, -1 on error.
 */
int save_records_to_file(FILE *file, const Record *records, size_t count) {
    if (!file || !records) return -1;
    size_t wr = fwrite(records, sizeof(Record), count, file);
    return (wr == count) ? 0 : -1;
}

/**
 * @brief Prints a record to standard output in a human-readable format.
 *
 * The output includes all fields of the record:
 *  - id (unsigned 64-bit integer)
 *  - field1 (floating-point)
 *  - field2 (signed 64-bit integer)
 *  - field3 (up to 16 characters, treated as a string)
 *
 * @param record Pointer to the record to print. If NULL, the function does nothing.
 */
void print_record(const Record *record) {
    if (!record) return;
    printf("Record{id=%llu, field1=%f, field2=%lld, field3='%.*s'}\n",
           (unsigned long long)record->id,
           record->field1,
           (long long)record->field2,
           16, record->field3);
}