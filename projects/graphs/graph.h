#ifndef GRAPH_H
#define GRAPH_H

typedef struct _neighbour Neighbour;
typedef struct _graph Graph;

struct _neighbour { 
    int vertexValue;
    float weight; 
    Neighbour*  next; 
};

struct _graph { 
    int nv;    /* amount of vertices */
    int ne;    /* amount of edges */
    Neighbour** nb; /* nb[i] points to the list of edges that affects i */ 
};

// Used for Kruskal's algorithm
typedef struct _edge Edge;
struct _edge {
    int u, v;  
    float weight;
};

Graph* create_graph(int nv);
void add_edge(Graph* g, int vertex1, int vertex2, float weight);
void print_as_adjacency_list(Graph* g);
void print_as_adjacency_matrix(Graph* g);
void bfs(Graph* g, int initialVertexValue);
Graph* kruskal(Graph* g);
void free_graph(Graph* g);
#endif