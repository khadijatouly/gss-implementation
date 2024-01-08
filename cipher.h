#include "gf_operation.h"
#include "matrix.h"

#ifndef CIPHER_H
#define CIPHER_H

void gen_error_vector(gf_t *v, int len);
void gen_message(gf_t *binary_message, int len);
void concat(gf_t *concate, gf_t *u,gf_t *v);
void add_error_vector(gf_t *cipher, gf_t * concate, binarymatrix_t exp_e);
void cipher();

#endif