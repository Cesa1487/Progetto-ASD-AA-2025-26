#include "../include/edit_distance.h"
#include <string.h>
#include <stdlib.h>

/**
 * @brief Compute the edit distance between two strings using
 * a simple recursive algorithm.
 * Operations allowed: insertions and deletions only.
 *
 * @param s1 target string
 * @param s2 source string
 * @return minimum number of operations to transform s2 into s1
 */
int edit_distance(const char *s1, const char *s2) {
    if (*s1 == '\0') return strlen(s2);

    if (*s2 == '\0') return strlen(s1);

    if (*s1 == *s2) {
        return edit_distance(s1 + 1, s2 + 1);
    } else {
        int del = 1 + edit_distance(s1, s2 + 1);

        int ins = 1 + edit_distance(s1 + 1, s2);

        return (del < ins) ? del : ins;
    }
}

/**
 * @brief Helper function for edit_distance_dyn().
 * Recursive computation of edit distance with memoization.
 *
 * @param s1 target string
 * @param s2 source string
 * @param memo 2D array for memoization
 * @param i current index in s1
 * @param j current index in s2
 * @param len1 length of s1
 * @param len2 length of s2
 * @return minimum number of operations to transform s2[j:] into s1[i:]
 */
static int edit_distance_dyn_rec(const char *s1, const char *s2, int **memo, int i, int j, int len1, int len2) {
    if (memo[i][j] != -1) return memo[i][j];

    int res;

    if (i == len1) {
        res = len2 - j;
    } else if (j == len2) {
        res = len1 - i;
    } else if (s1[i] == s2[j]) {
        res = edit_distance_dyn_rec(s1, s2, memo, i + 1, j + 1, len1, len2);
    } else {
        int del = 1 + edit_distance_dyn_rec(s1, s2, memo, i, j + 1, len1, len2);
        int ins = 1 + edit_distance_dyn_rec(s1, s2, memo, i + 1, j, len1, len2);
        res = (del < ins) ? del : ins;
    }

    memo[i][j] = res;
    return res;
}

/**
 * @brief Compute the edit distance between two strings using
 * a recursive algorithm with memoization.
 * Operations allowed: insertions and deletions only.
 *
 * @param s1 target string
 * @param s2 source string
 * @return minimum number of operations to transform s2 into s1
 */
int edit_distance_dyn(const char *s1, const char *s2) {
    int len1 = strlen(s1);
    int len2 = strlen(s2);

    int **memo = (int **)malloc((len1 + 1) * sizeof(int *));
    for (int i = 0; i <= len1; ++i) {
        memo[i] = (int *)malloc((len2 + 1) * sizeof(int));
        for (int j = 0; j <= len2; ++j)
            memo[i][j] = -1;
    }

    int result = edit_distance_dyn_rec(s1, s2, memo, 0, 0, len1, len2);

    for (int i = 0; i <= len1; ++i)
        free(memo[i]);
    free(memo);

    return result;
}