#include "gf_operation.h"
#include "matrix.h"

#ifndef CIPHER_H
#define CIPHER_H

void gen_error_vector(gf_t *v, int len);
void gen_message(unsigned char *binary_message, int len);
void concat(binarymatrix_t concate, unsigned char *m, binarymatrix_t c);
void add_error_vector(binarymatrix_t cipher, binarymatrix_t concate, binarymatrix_t exp_e);
void cipher();

#endif