
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <quadmath.h>
#include <unistd.h>
#include <time.h>
#include <string.h>


#include "graf_ER.h"
#include "double_random.h"

/*
void clear_screen() {
  const char* CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO,CLEAR_SCREE_ANSI,12);
}*/



/* ************************************************************************************************************ *
 * 						UTIL FUNCTIONS							*
 * ************************************************************************************************************ */



/* ********************************************* Creating a graph ********************************************* */

// Tutaj moze zrobic private functions, ktorych nie bd uzywac poza modulem
void set_clusters(Vertex* graph, uint* indices, uint k, uint cluster_num){
    while(k){
	k--;
	graph[indices[k]].cluster = cluster_num;
    }
}

/* ********************************************* Creating a graph ********************************************* */

Vertex* allocate_graph();

Vertex* create_graph_ER(uint N,const __float128 p, uint64_t* num_edges){
    /*
     * This function creates an array of Vertex structures representing a E-R graph.
     * 
     * Every vertex contains all edges.
     * 
     * Could be used for sparse graph
     * 
     * PARAMS
     * uint16_t N - number of vertices in a graph
     * __float128 p - probability of existence of an edge between any pair of vertices
     * RETURN
     */
    
    // allocating memory for vertices
    Vertex* graph = (Vertex *) calloc((size_t) N , sizeof(Vertex)); // Every element in structures should be zero
    
    volatile uint iter = N;
    
    uint alloc_k = (uint) ceilq( p*N*(2 + 3*sqrtq(p*(1-p)*N)) );
    alloc_k = (alloc_k <= N) ? alloc_k : N;
    
    // allocate memory for arrays with edges
    while (iter){
      iter--;
      
      // allocates some more memory than average usage in case not to realloc all the time
      graph[iter].array_edges = (uint *) calloc((size_t) alloc_k  , sizeof(uint));
      
    }
    
    // this loop iterates through vertices and allocates elements of structure Vertex
    while (iter < N){
	
	volatile uint iter_vert = iter+1;
	uint iter_edge = graph[iter].k;
	while(iter_vert < N) {
	    // drawing vertices
	    if ( p > random_float128() ) { // if connection exists
	      
	      // resize array if needed
	      if ( graph[iter].k >= alloc_k ){
		graph[iter].array_edges = (uint *) realloc(graph[iter].array_edges, (size_t) N * sizeof(uint));
	      }
	      
	      // saves connected vertex index
	      graph[iter].array_edges[iter_edge] = iter_vert;
	      graph[iter].k++;
	      
	      // if array of connected vertex is not allocated or too small
	      /*if ( !(graph[iter_vert].array_edges) ){
		graph[iter_vert].array_edges = (uint *) calloc((size_t) alloc_k , sizeof(uint));
	      }
	      else*/ 
	      if (graph[iter_vert].k >= alloc_k){
		graph[iter_vert].array_edges = (uint *) realloc(graph[iter_vert].array_edges, (size_t) N * sizeof(uint));
	      }
	      
	      // saves edge in connected vertex
	      graph[iter_vert].array_edges[graph[iter_vert].k] = iter; // iter - current vertex's index
	      graph[iter_vert].k++;
	      
	      //graph[iter].FLAG_VISITED=0b0;
	      
	      // increment iterators
	      *num_edges+=1;
	      iter_edge++;
	  }
	  
	  iter_vert++;
	  // end of loop drawing vertices
	}
	
	// free unused memory - it could be done in another thread !!!
	graph[iter].array_edges = realloc(graph[iter].array_edges, (graph[iter].k+1) * sizeof(uint));
	//graph[iter].FLAG_MEM_ALLOC=0b1;
	
	//for (uint kk = 0; kk < graph[iter].k; kk++)
	//  printf("\t%d",graph[iter].array_edges[kk]);
	//printf("\n");
	
	//printf("Vertex %d\n",iter);
	
	iter++; // iterates over vertices
    }
    printf("\n%d vertices allocated\n\n **************************************************************************************** \n\n",
	   iter);
    
      
    return graph;
}

/* ***************************************** Searching through a graph **************************************** */


void cluster_enumerator(Vertex* graph, Vertex* vertex, uint cluster_num, uint* cluster_size){
  /*
   * This function is recursively searching through a cluster to count number of vertices that belong to it
   * 
   * PARAMS
   * Vertex* graph - array of vertices
   * Vertex* vertex - pointer to current vertex
   * uint cluter_num - current_cluster nummer
   * uint* cluster_size - poiter to cluster size value;
   */
    
  vertex->cluster = cluster_num;
  vertex->FLAG_VISITED = 0b1;
  
  // increments size of array_clusters_sizes[iter_size] through a pointer
  *cluster_size += 1;
  
  //uint* ptr = vertex->array_edges;
  
  
  // iterate through array of connected vertices
  uint ii = 0;
  while (ii < vertex->k){
    //printf("%d ",vertex->array_edges[ii]);
    
    // if the vertex wasn't visited yet
    if (!(graph[ vertex->array_edges[ii] ].FLAG_VISITED))
      cluster_enumerator(graph, &graph[ vertex->array_edges[ii] ], cluster_num, cluster_size);
    ii++;
  }
  //printf("\n");
}

void enumerate_clusters(Vertex* graph, uint N,uint* array_clusters_sizes, uint* num_of_clusters ){
    /*
     * This function search for clusters and finds number of vertices in a clusters
     * 
     * array_clusters_sizes contains sizes of clusters with index corresponding to index in array.
     * num_of_clusters contains total number of clusters in a graph
     * 
     * PARAMS
     * Vertex* graph 			- array of Vertex structures representing a graph
     * 			 		  each structure have to have both-way connections
     * uint N 				- number of vertices
     * uint* array_clusters_sizes 	- array of size of clusters' number with sizes of cluster
     * uint* num_of_clusters		- pointer to total number of cluster
     * 
     * RETURN
     * 
     */
    //uint64_t num_edges = 0;
    uint cluster_num = 1;
    
    uint iter = 0;
    uint iter_sizes = 0;
    while (iter < N){
      
      if (graph[iter].cluster == 0){
	//printf("Klaster %d. :\n", cluster_num);
	cluster_enumerator(graph, &graph[iter], cluster_num, &array_clusters_sizes[iter_sizes]);
	//printf("\n");
	iter_sizes++;
	cluster_num++;
      }
      
      iter++;
    }
    
    *num_of_clusters = cluster_num-1;
    array_clusters_sizes = realloc(array_clusters_sizes, (size_t) (*num_of_clusters) * sizeof(uint) );
}

uint histogram_clusters(uint* array_clusters_sizes, uint* array_histogram, uint N, uint num_of_clusters){
  /*
   * This function makes histogram mapping size of cluster (from 1 to N) to number of 
   * 
   * USAGE
     uint* array_histogram = calloc((size_t) N, sizeof(uint));
     histogram_clusters(array_clusters_sizes, array_histogram, N, num_of_clusters);
   * 
   * PARAMS
   * uint* array_clusters_sizes 	- 
   * uint* array_histogram		- 
   * uint N 				-
   * uint num_of_clusters		-
   */
  
  uint S = 0;
  uint current_size = 0;
  
  while (num_of_clusters){
    num_of_clusters--;
    
    current_size = array_clusters_sizes[num_of_clusters];
    
    // finds size of the largest cluster
    S = (S > current_size) ? S : current_size;
    
    // increments histogram cell representing matiching size of cluster
    array_histogram[ current_size ]++;
  }
  
  return S;
}

void bin_histogram(uint* array_histogram, uint N, uint bin_width, char* filename){
 /*
  * This function saves results of histogram to file <filename> in fomat:
  * uint end_of_bin, 
  *
  */
  FILE* file;
  if ((file=fopen(filename, "w"))==NULL) {
     fprintf (stderr,"Nie mogę otworzyc pliku %s do zapisu!\n", filename);
     //exit(1);
  }
  
  uint iter_histogram = 0;
  uint iter = 0;
  uint cluster_in_bin = 0;
  
  while (iter_histogram < N) {
    
    iter = bin_width;
    cluster_in_bin = 0;
    
    while (iter) {
     cluster_in_bin += array_histogram[iter_histogram];
     iter--;
     iter_histogram++;
    }
    
  }
}

void bin_logaritmic(uint* array_histogram, uint N, const __float128 basis, char* filename){
  uint iter_histogram = 0;  
  uint cells_in_bin = 1;
  uint iter = cells_in_bin;
  long long last = -1;
  uint clusters_in_bin = 0;
  
  FILE* file;
  if ((file=fopen(filename, "a"))==NULL) {
     fprintf (stderr,"Nie mogę otworzyc pliku %s do zapisu!\n", filename);
     return;
     //exit(1);
  }
  
  while (iter_histogram < N){
    
    iter = cells_in_bin;
    last += cells_in_bin;
    clusters_in_bin = 0;
    
    while (iter){
      iter--;
      iter_histogram++;
      
      clusters_in_bin += array_histogram[iter_histogram];
      //printf("bin: %d\n",iter_histogram);
      if (iter_histogram >= N)
        break;
    }
    
    //printf("last: %lld, index %d\n",last,iter_histogram);
    fprintf(file,"%.5f\t%d\n", (last+1 < N) ? ((double) last+1.) : ((double) N), clusters_in_bin);
    cells_in_bin = ceilq( basis * cells_in_bin);
    cells_in_bin = ( (N-last) > 1 ) ? cells_in_bin : (N-last);
  }

}

void save_point ( uint N, /* number of vertices */
		  uint num_of_clusters, /* number of clusters */
		  uint num_edges, /* number of edges */
		  uint S, /* size of cluster of percolation */
		  __float128 k_theory, /* theoretical value of mean degree of vertices */
		  char* filename) {
  FILE* file;
  if ((file=fopen(filename, "a"))==NULL) {
     fprintf (stderr,"Nie mogę otworzyc pliku %s do zapisu!\n", filename);
     return;
     //exit(1);
  }
  
  fprintf(file,"%.3f\t%d\t%f\t%d\t%d\n", (double) k_theory, S, (double) (k_theory-1)/(N - 1), num_of_clusters, num_edges);
}


void count_graph(uint N, __float128 k_av, uint8_t repeats){
  
  __float128 p = k_av / (N - 1);
  
  // preparing file name
    int str_lenght = 21 + ceilq(log10q(N));
    char filename1[str_lenght];
    sprintf(filename1,"point_N%dk%.9f.txt", N, (double) k_av);
    
    str_lenght-=1;
    char filename2[str_lenght];
    sprintf(filename2,"hist_N%dk%.9f.txt", N, (double) k_av);
  
  while (repeats) {
    repeats--;
    
    clock_t start = clock();
    clock_t total = 0;
      
    uint64_t num_edges = 0;
    Vertex* graph = create_graph_ER(N, p, &num_edges);
    total = clock();
    printf("Stworzono graf %.4f s\n", (total - start) / (double)CLOCKS_PER_SEC);
    start=total;
    
    uint num_of_clusters = 0;
    uint* array_clusters_sizes = calloc((size_t) N , sizeof(uint));
    enumerate_clusters(graph,N,array_clusters_sizes, &num_of_clusters);
    
    total = clock();
    printf("Zliczono klastry %.4f s\n", (total - start) / (double) CLOCKS_PER_SEC );
    start=total;
    
    
    
    uint* array_histogram = calloc((size_t) N, sizeof(uint));
    uint S = histogram_clusters(array_clusters_sizes, array_histogram, N, num_of_clusters);
    
    total = clock();
    printf("Utworzono histogram %.4f s\n", (total - start) / (double) CLOCKS_PER_SEC );
    start=total;
    
    
    
    bin_logaritmic(array_histogram, N, 2.Q, filename2);
    save_point(N, num_of_clusters, num_edges, S, (double) p*(N-1), filename1);
    
    total = clock();
    printf("Zapisano dane %.4f s\n", (total - start) / (double) CLOCKS_PER_SEC );
    start=total;
    
    printf("\nZapisywanie histogramu %s\n", filename2);
    
    
    //print_graph(graph, N);
    //for (int ii = 0; ii < num_of_clusters; ii++) {
    //	printf("%d. cluster \t%d vertices\n",ii,array_clusters_sizes[ii]);
    //}
    
    printf("\n%d klastrów\n",num_of_clusters);
    printf("klaster perkolacyjny %d\n\n",S);
    printf("\t\t wartosci wylosowane / teoretyczne\n");
    printf("calkowita ilosc krawedzi E:\t%lu / %.1f \n", num_edges,(double) p*(N-1)*N/2.);
    printf("prawodopodobienstwo p:\t\t%f/%f\n", (double) 2.*num_edges/(N*(N-1)), (double) p );
    printf("sredni stopien grafu k:\t\t%f/%f\n\n",(double) 2.*num_edges/N, (double) p*(N-1));
    
      
    delete_graph(graph,N);
    free(array_clusters_sizes);
    free(array_histogram);
  }
}


/* ********************************************* Managing a graph ********************************************* */


void delete_graph(Vertex* graph, uint N){
  /*
   * PARAMS
   * Vertex* graph - array of structs Vertex representing graph
   * uint16_t N - number of vertices in a graph (size of array graph)
   */
  do {
	N--;
	//printf("%d.",N); printf(" %d\n",graph[N].index);
	free(graph[N].array_edges);
    } while (N);
  printf("Memory for graph freed\n");
  free(graph);
}


/* ************************************************************************************************************ *
 * 							TESTS							*
 * ************************************************************************************************************ */

void present(){
    /*
     * This function just presents memory allocation for structure Vertex and tests its functionality
     */
    
    Vertex* x = (Vertex*) malloc(sizeof(Vertex));
    uint N = 10000;
    
    uint* pointer=NULL;
    printf("Pionter on Vertex %p\n", x);
    printf("Pionter on uint %p\n", pointer);
    printf("Size of pointer uint: %lu\n",sizeof(pointer));
    uint number;
    printf("Size of number uint: %lu\n",sizeof(number));
    
    x->k = binomial_distrib(N,1.Q/N);
    
    //x->index = 1;
    x->array_edges=(uint *) calloc((size_t) N,sizeof(uint));
    for (uint ii=0; ii < x->k; ii++){
	x->array_edges[ii] = ii;
    }
    x->FLAG_MEM_ALLOC = 0b1;
    
    printf("Size of structure: %lu\n",sizeof(x));
    
    //printf("Vertex index: %i\n",x->index);
    printf("Number of edges: %i\n",x->k);
    for (uint ii = 0; ii < x->k; ii++)
      printf("edge %i\n",x->array_edges[ii]);
    printf("Flag visited %i\n",x->FLAG_VISITED);
    printf("Flag memory  %i\n",x->FLAG_MEM_ALLOC);
    //printf("Test indices in array\n index %i\n",x[x->array_edges[0]].index);
    
    free(x->array_edges);
    free(x);
}


void print_graph(Vertex* graph, uint N){
  /*
   * This function lists all contents of vertices in a graph.
   * 
   * PARAMS
   * Vertex* graph - array of structs Vertex representing graph
   * uint16_t N - number of vertices in a graph (size of array graph)
   * */
  if (N > 100) {
      fprintf(stderr,"\nToo large to print, master!\n\n");
      return;
  }
  
  for (uint ii = 0; ii < N; ii++){
      printf("Vertex %d. \n",ii);
      printf("\tcluster: %d\n",graph[ii].cluster);
      printf("\tedges: %d.\n",graph[ii].k);
      for (uint jj = 0; jj < graph[ii].k; jj++)
	  printf("\t\t%d. connection with vertex: %d\n",jj,graph[ii].array_edges[jj]);
  }
}