#include <stdlib.h>
#include "../include/graph.h"
#include "../include/hash_table.h"

static int EMPTY = 0;

typedef struct {
    void* data;
    HashTable* edges;
} NodeInfo;

/**
 * @brief Internal concrete graph structure.
 */
struct graph {
    HashTable* nodes;
    int directed;
    int labelled;
    int (*compare)(const void*, const void*);
    unsigned long (*hash)(const void*);
    size_t num_edges;
};

/**
 * @brief Helper function to create a NodeInfo structure.
 * * Allocates memory for the node info and initializes its edge hash table.
 * * @param gr The graph instance (needed for compare/hash functions).
 * @param data The node data pointer.
 * @return Pointer to the newly allocated NodeInfo structure.
 */
static NodeInfo* create_node_info(Graph gr, void* data) {
    NodeInfo* info = malloc(sizeof(NodeInfo));
    info->data = data;
    info->edges = hash_table_create(gr->compare, gr->hash);
    return info;
}

/**
 * @brief Helper function to free a NodeInfo structure.
 * * Frees the edge hash table and the NodeInfo structure itself.
 * * @param info The NodeInfo structure to free.
 */
static void free_node_info(NodeInfo* info) {
    if (info) {
        hash_table_free(info->edges);
        free(info);
    }
}

/**
 * @brief Create an empty graph.
 */
Graph graph_create(int labelled, int directed, int (*compare)(const void*, const void*), unsigned long (*hash)(const void*)) {
    Graph gr = malloc(sizeof(struct graph));
    if (!gr) return NULL;

    gr->nodes = hash_table_create(compare, hash);
    gr->directed = directed;
    gr->labelled = labelled;
    gr->compare = compare;
    gr->hash = hash;
    gr->num_edges = 0;

    return gr;
}

/**
 * @brief Tell whether the graph is directed.
 */
int graph_is_directed(const Graph gr) { return gr->directed; }

/**
 * @brief Tell whether the graph is labelled.
 */
int graph_is_labelled(const Graph gr) { return gr->labelled; }

/**
 * @brief Add a node if not already present.
 */
int graph_add_node(Graph gr, const void* node) {
    if (!gr || !node) return 0;
    if (hash_table_contains_key(gr->nodes, node)) return 0;

    NodeInfo* info = create_node_info(gr, (void*)node);
    hash_table_put(gr->nodes, (void*)node, info);
    return 1;
}

/**
 * @brief Add edge (node1 -> node2). Mirrors on undirected graphs.
 */
int graph_add_edge(Graph gr, const void* node1, const void* node2, const void* label) {
    if (!gr || !node1 || !node2) return 0;
    if (gr->labelled && label == NULL) return 0;

    if (!graph_contains_node(gr, node1)) return 0;
    if (!graph_contains_node(gr, node2)) return 0;

    NodeInfo* src_info = (NodeInfo*)hash_table_get(gr->nodes, node1);

    if (hash_table_contains_key(src_info->edges, node2)) return 0;

    void* val_to_store = (label == NULL) ? &EMPTY : (void*)label;
    hash_table_put(src_info->edges, (void*)node2, val_to_store);

    if (!gr->directed) {
        NodeInfo* dest_info = (NodeInfo*)hash_table_get(gr->nodes, node2);
        if (gr->compare(node1, node2) != 0) {
            hash_table_put(dest_info->edges, (void*)node1, val_to_store);
        }
    }

    gr->num_edges++;
    return 1;
}

/**
 * @brief Check if a node exists.
 */
int graph_contains_node(const Graph gr, const void* node) {
    if (!gr || !node) return 0;
    return hash_table_contains_key(gr->nodes, node);
}

/**
 * @brief Check if edge (node1 -> node2) exists.
 */
int graph_contains_edge(const Graph gr, const void* node1, const void* node2) {
    if (!gr || !node1 || !node2) return 0;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node1);
    if (!info) return 0;
    return hash_table_contains_key(info->edges, node2);
}

/**
 * @brief Remove a node and all incident edges.
 */
int graph_remove_node(Graph gr, const void* node) {
    if (!graph_contains_node(gr, node)) return 0;

    NodeInfo* target_info = (NodeInfo*)hash_table_get(gr->nodes, node);

    if (!gr->directed) {
        size_t degree = hash_table_size(target_info->edges);
        gr->num_edges -= degree;

        void** neighbors = hash_table_keyset(target_info->edges);
        for (size_t i = 0; i < degree; i++) {
            void* neigh = neighbors[i];

            if (gr->compare(neigh, node) != 0) {
                NodeInfo* n_info = (NodeInfo*)hash_table_get(gr->nodes, neigh);
                hash_table_remove(n_info->edges, node);
            }
        }
        free(neighbors);
    } else {
        size_t out_degree = hash_table_size(target_info->edges);
        gr->num_edges -= out_degree;
        void** all_nodes = graph_get_nodes(gr);
        size_t count = graph_num_nodes(gr);

        for (size_t i = 0; i < count; i++) {
            void* curr = all_nodes[i];
            if (curr == node) continue;

            NodeInfo* curr_info = (NodeInfo*)hash_table_get(gr->nodes, curr);
            if (hash_table_contains_key(curr_info->edges, node)) {
                hash_table_remove(curr_info->edges, node);
                gr->num_edges--;
            }
        }
        free(all_nodes);
    }

    free_node_info(target_info);
    hash_table_remove(gr->nodes, node);
    return 1;
}

/**
 * @brief Remove edge (node1 -> node2).
 */
int graph_remove_edge(Graph gr, const void* node1, const void* node2) {
    if (!graph_contains_edge(gr, node1, node2)) return 0;

    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node1);
    hash_table_remove(info->edges, node2);

    if (!gr->directed) {
        NodeInfo* info2 = (NodeInfo*)hash_table_get(gr->nodes, node2);
        if (gr->compare(node1, node2) != 0) {
            hash_table_remove(info2->edges, node1);
        }
    }
    gr->num_edges--;
    return 1;
}

/**
 * @brief Number of nodes in the graph.
 */
int graph_num_nodes(const Graph gr) {
    return (int)hash_table_size(gr->nodes);
}

/**
 * @brief Edge count (undirected counted once). O(N) as per spec.
 */
int graph_num_edges(const Graph gr) {
    return (int)gr->num_edges;
}

/**
 * @brief Retrieve all nodes (NULL-terminated array). Caller frees the array.
 */
void** graph_get_nodes(const Graph gr) {
    return hash_table_keyset(gr->nodes);
}

/**
 * @brief Retrieve all edges (NULL-terminated array of Edge*). Caller frees items and array.
 */
Edge** graph_get_edges(const Graph gr) {
    if (gr->num_edges == 0) return NULL;

    Edge** result = malloc(gr->num_edges * sizeof(Edge*));
    if (!result) return NULL;

    void** nodes = graph_get_nodes(gr);
    size_t num_nodes = graph_num_nodes(gr);
    size_t k = 0;

    for (size_t i = 0; i < num_nodes; i++) {
        NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, nodes[i]);
        void** neighbors = hash_table_keyset(info->edges);
        size_t num_neigh = hash_table_size(info->edges);

        for (size_t j = 0; j < num_neigh; j++) {
            void* src = nodes[i];
            void* dst = neighbors[j];

            if (!gr->directed) {
                if (gr->compare(src, dst) > 0) {
                    continue;
                }
            }

            if (k >= gr->num_edges) break;

            Edge* e = malloc(sizeof(Edge));
            e->source = src;
            e->dest = dst;

            void* val = hash_table_get(info->edges, dst);
            e->label = (val == &EMPTY) ? NULL : val;

            result[k++] = e;
        }
        free(neighbors);
    }
    free(nodes);
    return result;
}

/**
 * @brief Get neighbours of a node (NULL-terminated array). Caller frees the array.
 */
void** graph_get_neighbours(const Graph gr, const void* node) {
    if (!graph_contains_node(gr, node)) return NULL;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node);
    return hash_table_keyset(info->edges);
}

/**
 * @brief Get neighbours of a node (NULL-terminated array). Caller frees the array.
 */
int graph_num_neighbours(const Graph gr, const void* node) {
    if (!graph_contains_node(gr, node)) return 0;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node);
    return (int)hash_table_size(info->edges);
}

/**
 * @brief Get the label of edge (node1 -> node2), or NULL.
 */
void* graph_get_label(const Graph gr, const void* node1, const void* node2) {
    if (!graph_contains_edge(gr, node1, node2)) return NULL;
    NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, node1);

    void* val = hash_table_get(info->edges, node2);
    if (val == &EMPTY) return NULL;
    return val;
}

/**
 * @brief Free internal structures.
 */
void graph_free(Graph gr) {
    if (!gr) return;
    void** nodes = graph_get_nodes(gr);
    size_t n = graph_num_nodes(gr);
    for(size_t i=0; i<n; i++) {
        NodeInfo* info = (NodeInfo*)hash_table_get(gr->nodes, nodes[i]);
        free_node_info(info);
    }
    free(nodes);
    hash_table_free(gr->nodes);
    free(gr);
}