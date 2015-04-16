#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <quadmath.h>

#include "quad_random.h"

/*
 * The purpose is to create a library for quad precision random numbers
 */


static uint64_t x1 = 1;
static uint64_t x2 = 0xfffa;

uint64_t random1_uint64(){
    /*
     * This random number generator should
     */
    // http://en.wikipedia.org/wiki/Linear_congruential_generator
    //MMIX
    uint64_t a = 6364136223846793005;
    uint64_t c = 1442695040888963407;
    uint64_t m = 18446744073709551616;
    x2 = (a*x2 + c)%m;
    return x2;  /*64-bit result */
}

uint64_t random2_uint64(){
    /*
     * This random number generator should
     */
    // http://en.wikipedia.org/wiki/Linear_congruential_generator
    //Newlib
    uint64_t a = 6364136223846793005;
    uint64_t c = 1;
    uint64_t m = 18446744073709551616;
    x1 = (a*x1 + c)%m;
    return x1;  /* 32-bit result */
}

/*__float128 random_float128(){
    char str[128];
    snprintf(str,sizeof(str),"%lu",random_uint64());
    __float128 counter = strtoflt128(str,NULL);
    //__float128 reminder = powq(2,64);
    return counter/powq(2,64);
}*/


/*
 * 
 * ALE TU TRZEBA ZMIENIC LICZBY W FUNKCJI GEN FLOATA!!!!
 * 
 */


/* generates a random number on [0,1) as a __float128 */
__float128 random_float128(){
    uint64_t a=random1_int64()>>5, b=random2_int64()>>6;
    return(a*67108864.Q+b)*(1.Q/9007199254740992.Q); 
}