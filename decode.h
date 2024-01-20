#include "gf_operation.h"
#include "matrix.h"

#ifndef DECODE_H
#define DECODE_H

gf_t *syndrome(matrix_t y, gf_t *S, gf_t *L, int d);
gf_t syndrome_polynomial(gf_t * syndrome);
gf_t euclide_etendu(gf_t a, gf_t b, gf_t *u, gf_t *v);
void compute_polynomials(gf_t a, gf_t b, gf_t lambda, gf_t gamma);
gf_t derive_lambda(gf_t lambda);
gf_t * inverse_support(gf_t *S);
void error_location(gf_t *S, gf_t lambda, int *position_error);
void error_value(gf_t *S, gf_t *L, gf_t lambda, gf_t gamma, int *position, gf_t *error_values);
void decode(matrix_t y, gf_t *S, gf_t *L);

#endif