#include "matrix.h"
#include "param.h"
#include "util.h"
#include "rng.h"

#ifndef KEY_GEN
#define KEY_GEN

void generate_random_vector(int m, gf_t *vect);
void init_random_element(gf_t *U);
void rs_support(gf_t *S, gf_t *L);
int key_pair_gen();
void cauchy_support_centro(gf_t *z, gf_t *L, gf_t alpha);

#endif