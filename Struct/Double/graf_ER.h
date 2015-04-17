#ifndef __graf_E_R_h__
#define __graf_E_R_h__

#define uint uint32_t

/*
 * Declaration of structure Vertex
 */
struct Vertex{
    uint* array_edges; // array with indices of neighbors
    volatile uint k; // number of vertices which the vertex connects with
    //uint index; // index of vertex in vertices array (graph)
    volatile uint cluster; // index of cluster that vertex belongs to
    uint8_t FLAG_VISITED:1; // flag to check if the structure was visited during search
    uint8_t FLAG_MEM_ALLOC:1; // flag to check if the memmory for array_edges is allocated
} pack;
typedef struct Vertex Vertex;


Vertex* create_graph_ER(uint N,const double p, uint64_t* num_edges);
Vertex* create_graph_ER1(uint N,const double p); //This function creates an array of Vertex structures representing a E-R graph

//void cluster_enumerator(Vertex* graph, Vertex* vertex, uint cluster_num, uint* cluster_size);
void enumerate_clusters(Vertex* graph, uint N,uint* array_clusters_sizes, uint* num_of_clusters );
void search_clusters(Vertex* graph, uint N, uint* array_clusters_sizes, uint* num_of_clusters);

uint histogram_clusters(uint* array_clusters_sizes, uint* array_histogram, uint N, uint num_of_clusters);
void bin_logaritmic(uint* array_histogram, uint N, double basis, char* filename);
void save_point ( uint N, /* number of vertices */
		  uint num_of_clusters, /* number of clusters */
		  uint num_edges, /* number of edges */
		  uint S, /* size of cluster of percolation */
		  double k_theory, /* theoretical value of mean degree of vertices */
		  char* filename);
void count_graph(uint N_global, double k_av, uint8_t repeats);


void delete_graph(Vertex* graph, uint N);

void present(); //This function just presents memory allocation for structure Vertex
void print_graph(Vertex* graph, uint N);

#endif
