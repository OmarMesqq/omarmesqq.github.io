#include "graph.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// Auxiliary functions
static void _pretty_print_matrix(float **matriz, int tam);
static int _compare_edges(const void* a, const void* b);
static int _find(int* parents, int vertice);
static void _unionSets(int* parents, int* rank, int u, int v);


Graph* create_graph(int nv) {
    Graph* g = (Graph*) malloc(sizeof(Graph));
    Neighbour** nb = (Neighbour**) malloc(nv * sizeof(Neighbour*));
    if (!g|| !nb) {
        printf("Memory allocation failed during graph creation.\n");
        return NULL;
    }

    g->nv = nv;
    g->ne = 0;
    g->nb = nb;

    // NULL-initialize all entries in the adjacency list
    for (int i = 0; i < nv; i++) {
        g->nb[i] = NULL;
    }

    return g;
}

void add_edge(Graph* g, int vertex1, int vertex2, float weight) {
    if (!g) {
        printf("Can't add edge in NULL graph!\n");
        return;
    }

    if (vertex1 >= g->nv || vertex2 >= g->nv) {
        printf("Vertex value exceeds the valid range of vertex indices of the graph!\n");
        return;
    }

    // Create the edge from vertex2 to vertex1.
    Neighbour* nb = (Neighbour*) malloc(sizeof(Neighbour));
    nb->currentNeighbour = vertex2;
    nb->weight = weight;
    nb->next = g->nb[vertex1];
    g->nb[vertex1] = nb;

    // Since the graph is undirected, also create the edge from vertex1 to vertex2
    nb = (Neighbour*)malloc(sizeof(Neighbour));
    nb->currentNeighbour = vertex1;
    nb->weight = weight;
    nb->next = g->nb[vertex2];
    g->nb[vertex2] = nb;

    g->ne++;
}

void print_as_adjacency_list(Graph* g) {
    if (!g) return;

    // Iterates all the vertices of the graph
    for (int i = 0; i < g->nv; i++) {
        Neighbour* v = g->nb[i];
        if (v == NULL) {
            continue;
        }

        printf("%d: ", i);
        // Iterates through the adjacency list of vertex i
        while (v) {
            printf("-> %d (weight: %.2f) ", v->currentNeighbour, v->weight);
            v = v->next;
        }
        printf("\n");
    }
}

void print_as_adjacency_matrix(Graph* g) {
    if (!g) return;

    float** matriz = (float**) malloc(g->nv * sizeof(float*));
    for (int i = 0; i < g->nv; i++) {
        matriz[i] = (float*) calloc(g->nv, sizeof(float));
    }

    for (int i = 0; i < g->nv; i++) {
        Neighbour* v = g->nb[i];
        while (v) {
            matriz[i][v->currentNeighbour] = v->weight;
            v = v->next;
        }
    }

    _pretty_print_matrix(matriz, g->nv); 

    for (int i = 0; i < g->nv; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void free_graph(Graph* g) {
    if (!g) return;
    for (int i = 0; i < g->nv; i++) {
        Neighbour* v = g->nb[i];
        while (v) {
            Neighbour* temp = v;
            v = v->next;
            free(temp);
        }
    }
    free(g->nb);
    free(g);
}

/*
Performs breadth-first search (BFS)
*/
void bfs(Graph* g, int initialVertex) {
    if (!g) return;

    // Queue and visited vertices list creation
    Queue* verticesToVisit = create_queue();
    if (!verticesToVisit) {
        printf("Memory allocation failed during queue creation in BFS.\n");
        return;
    }
    int* visited = (int*) calloc(g->nv, sizeof(int));
    if (!visited) {
        printf("Memory allocation failed during visited vertex list creation in BFS.\n");
        free_queue(verticesToVisit);
        return;
    }

    printf("Starting BFS from vertex %d\n", initialVertex);

    // Enqueue initial vertex
    Neighbour* initial = (Neighbour*) malloc(sizeof(Neighbour));
    if (!initial) {
        printf("Memory allocation failed during initial vertex creation in BFS.\n");
        return;
    }
    initial->currentNeighbour = initialVertex;
    initial->weight = INT_MIN;
    initial->next = NULL;

    enqueue(verticesToVisit, initial);
    
    while (!is_queue_empty(verticesToVisit)) {
        QueueNode* currentVertex = dequeue(verticesToVisit);
        Neighbour* currentNeighbour = currentVertex->nb;
        
        printf("Visiting %d\n", currentNeighbour->currentNeighbour);
        visited[currentNeighbour->currentNeighbour] = 1;
        free(currentVertex);

        // Iterates over each neighbour of the current vertex 
        Neighbour* nb = g->nb[currentNeighbour->currentNeighbour];
        while (nb) {
            if (!visited[nb->currentNeighbour]) {
                visited[nb->currentNeighbour] = 1;

                // Enqueues a neighbour copy
                Neighbour* neighbourCopy = (Neighbour*) malloc(sizeof(Neighbour));
                if (!neighbourCopy) {
                    printf("Memory allocation failed during neighbour copy creation in BFS.\n");
                    return;
                }
                neighbourCopy->currentNeighbour = nb->currentNeighbour;
                neighbourCopy->weight = nb->weight;
                neighbourCopy->next =  nb->next;
                enqueue(verticesToVisit, neighbourCopy);
            }
            nb = nb->next;
        }
        free(currentNeighbour);
    }

    free_queue(verticesToVisit);
    free(visited);
}

/*
Returns the minimum spanning tree
(subgraph that contains all vertices and doesn't have cycles)
*/
Graph* kruskal(Graph* g) {
    if (!g) return NULL;

    // Firstly, store all edges
    Edge* edges = (Edge*) malloc(g->ne * sizeof(Edge));
    if (!edges) {
        printf("Memory allocation failed during creation of edge list in Kruskal.\n");
        return NULL;
    };

    int indiceDaAresta = 0;
    for (int i = 0; i < g->nv; i++) {
        for (Neighbour* nb = g->nb[i]; nb != NULL; nb = nb->next) {
            // Perform deduplication
            if (i < nb->currentNeighbour) {
                edges[indiceDaAresta++] = (Edge) {i, nb->currentNeighbour, nb->weight};
            }
        }
    }

    // Sort the edges by weight using quick sort
    qsort(edges, g->ne, sizeof(Edge), _compare_edges);

    // Inicializamos a Union-Find (DSU)
    int* parents = (int*) malloc(g->nv * sizeof(int));
    int* rank = (int*) calloc(g->nv, sizeof(int));
    if (!parents || !rank) {
        printf("Memory allocation failed during creation of Union-Find DS in Kruskal.\n");
        free(edges);
        free(parents);
        free(rank);
        return NULL;
    }

    // Initially, each vertex is the parent of the set that contains itself
    for (int i = 0; i < g->nv; i++) {
        parents[i] = i;
    }

    // Initializes the minimum spanning tree (MST)
    Graph* mst = create_graph(g->nv);
    if (!mst) {
        printf("Memory allocation failed during creation of spanning tree in Kruskal.\n");
        free(edges);
        free(parents);
        free(rank);
        return NULL;
    }

    /*
    Kruskal's main loop:
    the loop containues until all edges are included in the MST
    OR the MST is complete, that is, it has g->nv - 1 edges
    The edges are processed in ascending order, due to previous sorting
    */
    for (int i = 0, arestasIncluidas = 0; i < g->ne && arestasIncluidas < g->nv - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        float weight = edges[i].weight;

        /*
        If this conditional statement is true, it's safe to add the edge to the MST.
        Since they have different parents, they are in disjoint sets (different subtrees).
        Thus, no cycle will be formed
        */
        if (_find(parents, u) != _find(parents, v)) {
            add_edge(mst, u, v, weight);
            // Joins the sets of both vertices to reflect the current nature of the MST post edge inclusion
            _unionSets(parents, rank, u, v);
            arestasIncluidas++;
        }
    }

    free(edges);
    free(parents);
    free(rank);

    return mst;
}


static void _pretty_print_matrix(float **matriz, int tam) {
    // Gets columns (j) with values != 0, creating a list of "active" columns
    int active_columns[tam];
    for (int j = 0; j < tam; j++) {
        // Initially, assume all columns are inactive
        active_columns[j] = 0;
        for (int i = 0; i < tam; i++) {
            if (matriz[i][j] != 0) {
                // Column has value != 0, activate it
                active_columns[j] = 1;
                break;
            }
        }
    }

    // Print active columns
    printf("   ");
    for (int j = 0; j < tam; j++) {
        if (active_columns[j]) {
            printf("%6d", j);
        }
    }
    printf("\n");

    // Same logic for rows: print the active ones i.e with values != 0
    for (int i = 0; i < tam; i++) {
        int row_has_value = 0;
        for (int j = 0; j < tam; j++) {
            if (matriz[i][j] != 0) {
                row_has_value = 1;
                break;
            }
        }

        // Print the matrix itself
        if (row_has_value) {
            printf("%3d", i);
            for (int j = 0; j < tam; j++) {
                if (active_columns[j]) {
                    if (matriz[i][j] != 0) {
                        printf("%6.2f", matriz[i][j]);
                    } else {
                        printf("%6.0f", 0.0);
                    }
                }
            }
            printf("\n");
        }
    }
}

// Compares two edges by their weight
static int _compare_edges(const void* a, const void* b) {
    Edge* arestaA = (Edge*) a;
    Edge* arestaB = (Edge*) b;
    if (arestaA->weight < arestaB->weight) return -1;
    if (arestaA->weight > arestaB->weight) return 1;
    return 0;
}

// Search with path compression, avoiding cycles in the MST
static int _find(int* parents, int vertice) {
    if (parents[vertice] != vertice) {
        // Compression ("flattens" the tree): as the call stack unwinds, previous vertices point to the parent of the set 
        parents[vertice] = _find(parents, parents[vertice]);
    }

    // Vertex already is the root/parent of current set
    return parents[vertice];
}

// Performs union (U) of two, initially disjoint, sets
static void _unionSets(int* parents, int* rank, int u, int v) {
    int raizU = _find(parents, u);
    int raizV = _find(parents, v);

    // Picks the set with greatest rank ("height") in order to avoid deep and splay trees
    if (raizU != raizV) {
        if (rank[raizU] > rank[raizV]) {
            parents[raizV] = raizU;
        } else if (rank[raizU] < rank[raizV]) {
            parents[raizU] = raizV;
        } else {
            parents[raizV] = raizU;
            rank[raizU]++;
        }
    }
}
