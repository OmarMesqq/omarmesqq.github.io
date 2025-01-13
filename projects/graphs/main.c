#include <stdio.h>
#include <assert.h>
#include "graph.h"

// Maximum number of vertices
#define MAX_GRAPH_SIZE 30

int main() {
    Graph* g = create_graph(MAX_GRAPH_SIZE);

    // 14 edges in the graph
    add_edge(g, 0, 1, 4.0);
    add_edge(g, 0, 8, 8.0);
    add_edge(g, 8, 1, 11.0);
    add_edge(g, 3, 1, 8.0);
    add_edge(g, 3, 9, 2.0);
    add_edge(g, 9, 8, 7.0);
    add_edge(g, 7, 8, 1.0);
    add_edge(g, 7, 9, 6.0);
    add_edge(g, 4, 3, 7.0);
    add_edge(g, 3, 6, 4.0);
    add_edge(g, 4, 6, 14.0);
    add_edge(g, 4, 5, 9.0);
    add_edge(g, 5, 6, 10.0);
    add_edge(g, 6, 7, 2.0);
    assert(g->ne == 14);

    printf("Graph as adjacency list:\n");
    print_as_adjacency_list(g);

    printf("Graph as adjacency matrix:\n");
    print_as_adjacency_matrix(g);

    printf("\n");
    bfs(g, 0);

    Graph* mst = kruskal(g);
    printf("Spanning tree as adjacency list:\n");
    print_as_adjacency_list(mst);

    printf("\nSpanning tree as adjacency matrix:\n");
    print_as_adjacency_matrix(mst);

    free_graph(mst);
    free_graph(g);

    return 0;
}