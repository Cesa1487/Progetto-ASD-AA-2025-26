#ifndef BFS_H
#define BFS_H

#include "graph.h"

/**
 * @brief Perform a breadth-first traversal from a start node.
 *
 * The visit uses a hash-table to track visited nodes (same compare/hash used by the graph).
 * Returns a NULL-terminated array of node pointers in visit order. The array is heap-allocated
 * and must be freed by the caller. Node pointers are borrowed (not owned by BFS).
 *
 * @param gr       Graph handle.
 * @param start    Start node pointer; must be present in the graph.
 * @param compare  Equality function for nodes.
 * @param hash     Hash function for nodes.
 * @return NULL-terminated array of nodes in BFS order, or NULL if @p start is not in @p gr
 *         or on allocation failure.
 *
 * @complexity O(N + M) on the reachable subgraph.
 */
void** breadth_first_visit(Graph gr, void* start,
                           int (*compare)(const void*, const void*),
                           unsigned long (*hash)(const void*));

#endif