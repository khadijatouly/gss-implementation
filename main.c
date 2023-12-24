#include <stdio.h>
#include <stdlib.h>
#include "gf_operation.h"
#include "matrix.h"

int main()
{
    /* code */
    /*init_gf(4);
    gf_t inv = gf_inv(7);
    printf(" produit %d ", gf_mul(7, inv));*/
    // printf("inverse %d ", gf_inv(7));

    binarymatrix_t A = init_binary_matrix(3, 4);
    mat_change_coeff(A, 0, 0);
    mat_change_coeff(A, 0, 2);
    mat_change_coeff(A, 1, 0);
    mat_change_coeff(A, 1, 1);
    mat_change_coeff(A, 1, 3);
    mat_change_coeff(A, 2, 1);
    mat_change_coeff(A, 2, 2);
    // mat_change_coeff(A, 2, 3);
    display_binary_matrix(A);
    gauss_elim(A);
    // printf("%d ", gauss_elim(A));
    display_binary_matrix(A);
    binary_matrix_free(A);
    // mingw32-make.exe
}
