#include <stdio.h>
#include <stdlib.h>
#include "gf_operation.h"
#include "matrix.h"

int main()
{
    /* code */
    init_gf(3);
    gf_t L[7] = {1, 1, 1, 1, 1, 1, 1};
    gf_t S[7] = {1, 2, 4, 3, 6, 7, 5};
    matrix_t G = init_matrix(3, 7);
    no_binary_reed_solomon_secret_check_matrix(G, S, L);
    G.coefficient[0][1] = 0;
    G.coefficient[0][2] = 0;
    G.coefficient[0][3] = 3;
    G.coefficient[0][4] = 2;
    G.coefficient[0][5] = 1;
    G.coefficient[0][6] = 3;
    G.coefficient[1][0] = 0;
    G.coefficient[1][1] = 1;
    G.coefficient[1][2] = 0;
    G.coefficient[1][3] = 5;
    G.coefficient[1][4] = 5;
    G.coefficient[1][5] = 1;
    G.coefficient[1][6] = 4;
    G.coefficient[2][0] = 0;
    G.coefficient[2][1] = 0;
    G.coefficient[2][2] = 1;
    G.coefficient[2][3] = 7;
    G.coefficient[2][4] = 6;
    G.coefficient[2][5] = 1;
    G.coefficient[2][6] = 6;

    display_no_binary_matrix(G);
    binarymatrix_t exp_G = init_binary_matrix(9, 21);
    expansion_gen_mat(G, exp_G);
    display_binary_matrix(exp_G);
    binarymatrix_t *proj_mats = random_max_rank_matrix_list(7, 2);
    binarymatrix_t punct = punct_block_matrix(exp_G, proj_mats);
    printf("punct \n");
    display_binary_matrix(punct);
    no_binary_matrix_free(G);
    binary_matrix_free(exp_G);
    // mingw32-make.exe
}
