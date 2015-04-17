#ifndef __double_random_h__
#define __double_random_h__

#ifndef uint
#define uint uint32_t
#endif

void init_random();

uint32_t random1_uint32();
uint32_t random2_uint32();

double random_res53();


uint binomial_distrib(const uint N, double p);

#endif