#ifndef GRAPH_H
#define GRAPH_H

/**
 * @brief Opaque graph handle.
 */
typedef struct graph *Graph;

/**
 * @brief Opaque graph handle.
 */
typedef struct edge {
    void *source;
    void *dest;
    void *label;
} Edge;

/**
 * @brief Create an empty graph.
 *
 * @param labelled  1 = labelled graph (labels must be non-NULL), 0 = unlabelled (labels are ignored/NULL).
 * @param directed  1 = directed graph, 0 = undirected (edges are mirrored).
 * @param compare   node equality function used by internal hash tables.
 * @param hash      node hash function used by internal hash tables.
 * @return Graph handle or NULL on failure.
 *
 * @complexity O(1)
 */
Graph graph_create(int labelled, int directed,
                     int (*compare)(const void*, const void*),
                     unsigned long (*hash)(const void*));

/**
 * @brief Tell whether the graph is directed.
 * @return 1 if directed, 0 otherwise.
 * @complexity O(1)
 */
int graph_is_directed(const Graph gr);

/**
 * @brief Tell whether the graph is labelled.
 * @return 1 if labelled, 0 otherwise.
 * @complexity O(1)
 */
int graph_is_labelled(const Graph gr);

/**
 * @brief Add a node to the graph.
 *
 * @param node node pointer.
 * @return 1 if inserted, 0 if already present or on error.
 * @complexity O(1) average
 */
int graph_add_node(Graph gr, const void* node);

/**
 * @brief Add edge (node1 -> node2) with optional label.
 *
 * On undirected graphs the symmetric edge is added as well.
 * On labelled graphs @p label must be non-NULL; on unlabelled graphs label is ignored.
 *
 * @return 1 if inserted, 0 if already present or on error.
 * @complexity O(1) average (*) assuming constant average degree.
 */
int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label);

/**
 * @brief Check whether a node exists.
 * @return 1 if present, 0 otherwise.
 * @complexity O(1) average
 */
int graph_contains_node(const Graph gr, const void* node);

/**
 * @brief Check whether edge (node1 -> node2) exists.
 * @return 1 if present, 0 otherwise.
 * @complexity O(1) average (*)
 */
int graph_contains_edge(const Graph gr, const void* node1, const void* node2);

/**
 * @brief Remove a node and all incident edges.
 * @return 1 if removed, 0 if not found or on error.
 * @complexity O(N)
 */
int graph_remove_node(Graph gr, const void* node);

/**
 * @brief Remove edge (node1 -> node2).
 * @return 1 if removed, 0 if not found or on error.
 * @complexity O(1) average (*)
 */
int graph_remove_edge(Graph gr, const void* node1, const void* node2);

/**
 * @brief Number of nodes.
 * @return node count.
 * @complexity O(1)
 */
int graph_num_nodes(const Graph gr);

/**
 * @brief Number of edges (counted once in undirected graphs).
 * @return edge count.
 * @complexity O(N) as per specification.
 */
int graph_num_edges(const Graph gr);

/**
 * @brief Retrieve all nodes.
 * @return NULL-terminated array of node pointers (borrowed). Caller must free the array.
 * @complexity O(N)
 */
void** graph_get_nodes(const Graph gr);

/**
 * @brief Retrieve all edges.
 * @return NULL-terminated array of Edge*. Caller must free each Edge* and the array.
 * @complexity O(N)
 */
Edge** graph_get_edges(const Graph gr);

/**
 * @brief Get neighbours of a node.
 * @return NULL-terminated array of neighbour pointers (borrowed). Caller must free the array.
 * @complexity O(1) average (*)
 */
void** graph_get_neighbours(const Graph gr, const void* node);

/**
 * @brief Get the out-degree of a node.
 * @return number of neighbours (0 if node not found).
 * @complexity O(1)
 */
int graph_num_neighbours(const Graph gr, const void* node);

/**
 * @brief Get the label of edge (node1 -> node2).
 * @return label pointer or NULL if not labelled / edge not present.
 * @complexity O(1) average (*)
 */
void* graph_get_label(const Graph gr, const void* node1, const void* node2);

/**
 * @brief Free internal structures. Node/label pointers are not freed.
 */
void graph_free(Graph gr);

#endif