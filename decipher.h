#include "matrix.h"

#ifndef DECIPHER_H
#define DECIPHER_H

binarymatrix_t compute_syndrome(binarymatrix_t cipher, binarymatrix_t * proj_mats, int size);
void expansion_inverse(matrix_t exp_inv, binarymatrix_t exp, int base);

#endif