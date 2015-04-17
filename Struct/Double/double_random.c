#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "double_random.h"

static uint32_t x1 = 1;
static uint32_t x2 = 0xffffffa;

void init_random(){
  x1 = (uint32_t) clock();
  x2 = (uint32_t) clock();
}

uint32_t random1_uint32(){
    /*
     * This random number generator should
     */
    // http://en.wikipedia.org/wiki/Linear_congruential_generator
    //MMIX
    uint64_t a = 1103515245;
    uint64_t c = 12345;
    uint64_t m = pow(2,32);
    x1 = (uint32_t) (a*x1 + c)%m;
    //printf("%lu\n", x1);
    return x1;  /*64-bit result */
}

uint32_t random2_uint32(){
    /*
     * This random number generator should
     */
    // http://en.wikipedia.org/wiki/Linear_congruential_generator
    //Newlib
    uint64_t a = 1664525;
    uint64_t c = 1013904223;
    uint64_t m = pow(2,32);
    x2 = (uint32_t) (a*x2 + c)%m;
    //printf("%lu\n", x2);
    return x2;  /* 32-bit result */
}

/*
 * Tu by jeszcze mozna bylo wielo watkowosc dopisac, to wtedy powinien smigac
 */

/* generates a random number on [0,1) as a double */
double random_res53(){
    uint32_t a=random1_uint32()>>5, b=random2_uint32()>>6;
    return(a*67108864.+b)*(1./9007199254740992.); 
}
// 

/*
 * Do przemyslenia!!!
 * http://cas.ee.ic.ac.uk/people/dt10/research/rngs-gpu-mwc64x.html
uint MWC64X(uint2 *state)
{
    enum{ A=4294883355U };
    uint x=(*state).x, c=(*state).y;  // Unpack the state
    uint res=x^c;                     // Calculate the result
    uint hi=mul_hi(x,A);              // Step the RNG
    x=x*A+c;
    c=hi+(x<c);
    *state=(uint2)(x,c)               // Pack the state back up
    return res;                       // Return the next result
}
 */

/* ***** Various distributions ***** */

//Distribs for graphs E-R

uint binomial_distrib(const uint N, double p){
    /*
     * This function returns random number from binomial distribution,
     * which can represent a number of edges connected to a single vertex.
     * PARAMS
     * uint16_t N   - number of vertices in a graph
     * double p - quad precision probability of existance of an edge
     * RETURN
     * uint16_t k   - a number of edges connected to a single vertex (from
     *                binomial distribution)
     */
    uint k = 0; //random number of edges
    for (uint ii=0; ii < N; ii++){
     if (p > random_res53())
       k++;
    }
    // W sumie dziala zle
    return k;
}

uint poisson_distrib(const uint N, double p){
    /*
     * This function returns random number from Poisson distribution,
     * which can represent a number of edges connected to a single vertex.
     * PARAMS
     * uint16_t N   - number of vertices in a graph
     * double p - quad precision probability of existence of an edge
     * RETURN
     * uint16_t k   - a number of edges connected to a single vertex (from
     *                Poisson distribution)
     */
    uint k = 0; //random number of edges

    return k;
}

/* ***** arrays of numbers from given distribution ***** */

// Nothing yet