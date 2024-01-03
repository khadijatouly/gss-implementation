#include "matrix.h"
#include "param.h"
#include "util.h"
#include "rng.h"

#ifndef KEY_GEN
#define KEY_GEN

void generate_random_vector(int m, gf_t *vect);
void rs_support(gf_t *S, gf_t *L);
int key_pair_gen();

#endif