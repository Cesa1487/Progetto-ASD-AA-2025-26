#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/graph.h"
#include "../include/breadth_first_visit.h"

/**
 * @brief String equality comparator for tests.
 */
static int str_eq(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b) == 0;
}

/**
 * @brief string hash for tests.
 */
static unsigned long str_hash(const void *k) {
    const unsigned char *s = k;
    unsigned long h = 5381;
    int c;
    while ((c = *s++)) h = ((h << 5) + h) + c;
    return h;
}

/**
 * @brief Test: graph creation and flags.
 */
void test_create_and_flags() {
    Graph g = graph_create(1, 0, str_eq, str_hash);
    assert(graph_is_labelled(g) == 1);
    assert(graph_is_directed(g) == 0);
    graph_free(g);
}

/**
 * @brief Test: node insertion, presence and count.
 */
void test_add_and_contains_node() {
    Graph g = graph_create(0, 0, str_eq, str_hash);
    assert(graph_add_node(g, "roma") == 1);
    assert(graph_add_node(g, "torino") == 1);
    assert(graph_add_node(g, "roma") == 0);
    assert(graph_contains_node(g, "roma") == 1);
    assert(graph_contains_node(g, "milano") == 0);
    assert(graph_num_nodes(g) == 2);
    graph_free(g);
}

/**
 * @brief Test: directed edge insertion and duplicate check.
 */
void test_add_and_contains_edge() {
    Graph g = graph_create(0, 1, str_eq, str_hash);
    graph_add_node(g, "a");
    graph_add_node(g, "b");
    assert(graph_add_edge(g, "a", "b", NULL) == 1);
    assert(graph_contains_edge(g, "a", "b") == 1);
    assert(graph_contains_edge(g, "b", "a") == 0);
    assert(graph_add_edge(g, "a", "b", NULL) == 0);
    graph_free(g);
}

/**
 * @brief Test: neighbour retrieval and degree (undirected).
 */
void test_neighbours_and_num_neigh() {
    Graph g = graph_create(0, 0, str_eq, str_hash);

    graph_add_node(g, "x");
    graph_add_node(g, "y");
    graph_add_node(g, "z");

    graph_add_edge(g, "x", "y", NULL);
    graph_add_edge(g, "x", "z", NULL);
    assert(graph_num_neighbours(g, "x") == 2);

    void **n = graph_get_neighbours(g, "x");
    int found = 0;
    for (int i = 0; n[i]; i++) {
        if (strcmp(n[i], "y") == 0) found++;
        if (strcmp(n[i], "z") == 0) found++;
    }
    assert(found == 2);
    free(n);
    graph_free(g);
}

/**
 * @brief Test: BFS order and reachability count.
 */
void test_bfs_basic(void) {
    Graph g = graph_create(0, 0, str_eq, str_hash);

    graph_add_node(g, "a");
    graph_add_node(g, "b");
    graph_add_node(g, "c");
    graph_add_node(g, "d");

    graph_add_edge(g, "a", "b", NULL);
    graph_add_edge(g, "b", "c", NULL);
    graph_add_edge(g, "a", "d", NULL);

    void **ord = breadth_first_visit(g, "a", str_eq, str_hash);
    assert(ord != NULL);
    assert(strcmp((char*)ord[0], "a") == 0);
    int cnt = 0; while (ord[cnt]) cnt++;
    assert(cnt == 4);

    free(ord);
    graph_free(g);
}

/**
 * @brief Test: BFS order and reachability count.
 */
void test_bfs_start_not_found(void) {
    Graph g = graph_create(0, 0, str_eq, str_hash);
    graph_add_edge(g, "x", "y", NULL);
    void **ord = breadth_first_visit(g, "zzz", str_eq, str_hash);
    assert(ord == NULL);
    graph_free(g);
}

/**
 * @brief Test runner.
 */
int main() {
    test_create_and_flags();
    test_add_and_contains_node();
    test_add_and_contains_edge();
    test_neighbours_and_num_neigh();
    test_bfs_basic();
    test_bfs_start_not_found();
    printf("All graph tests + BFS passed!\n");
    return 0;
}