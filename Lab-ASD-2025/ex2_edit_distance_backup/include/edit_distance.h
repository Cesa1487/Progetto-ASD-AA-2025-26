#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

/**
 * @brief Compute the edit distance between two strings using
 * only insertions and deletions.
 *
 * @param s1 target string
 * @param s2 source string
 * @return minimum number of operations to transform s2 into s1
 */
int edit_distance(const char *s1, const char *s2);

/**
 * @brief Compute the edit distance between two strings using
 * a recursive approach with memoization.
 *
 * @param s1 target string
 * @param s2 source string
 * @return minimum number of operations to transform s2 into s1
 */
int edit_distance_dyn(const char *s1, const char *s2);

#endif