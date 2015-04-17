#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>


#include "double_random.h"
#include "graf_ER.h"

#define NUM_POINTS 2

static const uint N_global = 320000;
static uint8_t points_counter=0; // index of point to perform computations
static const uint8_t repeats = 7; // times to repeat the computation for single point
static const uint8_t num_points = NUM_POINTS;
static double points[NUM_POINTS] = {0.9,0.95};//, 1.1Q, 1.5Q};

pthread_mutex_t lock; // lock

void* compute(void* arg){
  double k_av = 0.;
  
  while (points_counter < num_points) {
    pthread_mutex_lock(&lock);
    /* ---- part of code that must be synchronized ---- */
    
    // checks if there are still points to perform computations
    
    
    k_av = points[points_counter];
    
    points_counter++;
    
    /* ---- end of code that must be synchronized  ---- */
    pthread_mutex_unlock(&lock);
    
    printf("%d punkt <k> %f\n", points_counter,(double) k_av);
    
    init_random();
    
    // perform main computation asychronously to 
    count_graph(N_global, k_av, repeats);
    
  }
  
  
  return NULL;
}



int main(){ // int argc, char *argv[]
    
    pthread_setconcurrency(2); // program uses 2 threads 
    
    cpu_set_t cpu_id;
    
    printf("\n");
    
    pthread_t thread1;
    pthread_t thread2;
    
    pthread_create(&thread1,NULL,compute,"thread 1 processing ...");
    pthread_create(&thread2,NULL,compute,"thread 2 processing ...");
    printf("%d\n",points_counter);
    
    // waiting for threads to end the computations
    pthread_join(thread1, NULL);
    //printf("*\n");
    pthread_join(thread2, NULL);
    printf("%d\n",points_counter);
    
    pthread_mutex_destroy(&lock);
    return 0;
}
