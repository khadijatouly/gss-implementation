#include <stdio.h>
#include <stdlib.h>
#include "gf_operation.h"
#include "matrix.h"
#include "key_gen.h"
#include "param.h"
#include "cipher.h"
#include "decipher.h"
#include "decode.h"

int main(void)
{
    cipher();
    /*init_gf(EXTENSION_DEGREE);
    gf_t * S = (gf_t *)calloc(code_length, sizeof(gf_t));
    gf_t *L = (gf_t *)calloc(code_length, sizeof(gf_t));
    rs_support(S, L);
    binarymatrix_t exp = init_binary_matrix(1, EXTENSION_DEGREE*code_length);
    expansion(L, code_length, exp, EXTENSION_DEGREE);
    display_binary_matrix(exp);
    matrix_t exp_inv = init_matrix(1, code_length);
    expansion_inverse(exp_inv, exp, EXTENSION_DEGREE);
    printf("compression \n");
    display_no_binary_matrix(exp_inv);*/

    return 0;
}
