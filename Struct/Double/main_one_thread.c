#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "graf_ER.h"
#include "double_random.h"

/*
 * gcc -o program main_one_thread.c graf_ER.c double_random.c -I. -std=c99 -Wall -lm -Ofast -O3 -m64 -march=native -fipa-pure-const -fipa-reference -fmerge-constants -fshrink-wrap -fsplit-wide-types -ftree-builtin-call-dce -ftree-copyrename -ftree-dce -ftree-dominator-opts -ftree-dse -ftree-forwprop -ftree-fre -ftree-phiprop -ftree-sra -ftree-pta -ftree-ter -funit-at-a-time -fkeep-inline-functions -fkeep-static-consts -fmerge-constants -fmodulo-sched -fgcse -fdce -fdse -fexpensive-optimizations -fipa-cp-clone -fipa-matrix-reorg -ftree-loop-linear -floop-interchange -floop-strip-mine -floop-block -ftree-loop-distribution -ffast-math -fassociative-math -freciprocal-math -funroll-all-loops -fpeel-loops
 */


int main(int argc, char *argv[]){
  
  
  uint N = 320000;
  double p = 1./(N-1);
  
  if (argc == 3){
    N = atoi(argv[1]);
    p = (double) atof(argv[2])/(N-1);
   printf("%s %f\n", argv[2], (double) atof(argv[2])/(N-1)); 
  }
  
  printf("N %d, k %.4f, p %.10f\n",N, (double) p*(N-1), (double) p);
  
  init_random();
  
  //present();
  
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
  
  // preparing file name
  int str_lenght = 21 + ceil(log10(N));
  char filename1[str_lenght];
  sprintf(filename1,"point_N%dk%.9f.txt", N, (double) p*(N-1));
  
  str_lenght-=1;
  char filename2[str_lenght];
  sprintf(filename2,"hist_N%dk%.9f.txt", N, (double) p*(N-1));
  
  bin_logaritmic(array_histogram, N, 2., filename2);
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
  return 0;
}
