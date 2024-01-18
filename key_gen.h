#include "matrix.h"
#include "util.h"
#include "rng.h"

#ifndef KEY_GEN
#define KEY_GEN

extern binarymatrix_t R;
extern binarymatrix_t punct_mat;
extern binarymatrix_t *proj_mats;

void generate_random_vector(int m, gf_t *vect);
void init_random_element(gf_t *U);
void rs_support(gf_t *S, gf_t *L);
int key_pair_gen();

#endif