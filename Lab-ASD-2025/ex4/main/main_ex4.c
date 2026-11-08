#include "../include/graph.h"
#include "../include/breadth_first_visit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief String equality comparator for node pointers.
 */
static int str_eq(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b) == 0;
}

/**
 * @brief string hash for node pointers.
 */
static unsigned long str_hash(const void *k) {
    const unsigned char *s = k;
    unsigned long h = 5381;
    int c;
    while ((c = *s++)) h = ((h << 5) + h) + c;
    return h;
}

/**
 * @brief Entry point: build graph, run BFS, write visit order to file.
 */
int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <italian_dist_graph.csv> <start_city> <output.txt>\n", argv[0]);
        return 1;
    }

    const char *csv_path = argv[1];
    const char *start_city = argv[2];
    const char *out_path = argv[3];

    FILE *fp = fopen(csv_path, "r");
    if (!fp) {
        perror("fopen csv");
        return 1;
    }

    Graph g = graph_create(/*labelled*/0, /*directed*/0, str_eq, str_hash);
    if (!g) { fclose(fp); fprintf(stderr, "graph_create failed\n"); return 1; }

    char line[1024];
    char place1[256], place2[256];
    float dist;
    int loaded_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;

        if (sscanf(line, "%255[^,],%255[^,],%f", place1, place2, &dist) == 3) {

            char *p1 = strdup(place1);
            char *p2 = strdup(place2);

            graph_add_node(g, p1);
            graph_add_node(g, p2);

            graph_add_edge(g, p1, p2, NULL);

            loaded_count++;
        }
    }
    fclose(fp);

    printf("Grafo caricato: %d nodi, %d archi letti.\n", graph_num_nodes(g), loaded_count);

    void **order = breadth_first_visit(g, (void*)start_city, str_eq, str_hash);
    if (!order) {
        fprintf(stderr, "Start city \"%s\" not found in graph or BFS failed.\n", start_city);
        graph_free(g);
        return 2;
    }

    FILE *out = fopen(out_path, "w");
    if (!out) {
        perror("fopen out");
        free(order);
        graph_free(g);
        return 1;
    }

    for (int i = 0; order[i]; ++i) {
        fprintf(out, "%s\n", (char*)order[i]);
    }
    fclose(out);

    free(order);
    graph_free(g);

    printf("BFS completata. Risultati scritti in %s\n", out_path);
    return 0;
}