#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "key_gen.h"
#include "gf_operation.h"

void generate_random_vector(int m, gf_t *vect)
{
    int v;
    gf_t tmp;
    gf_t *U;
    U = (gf_t *)calloc(gf_card(), sizeof(gf_t));
    unsigned char *random_bytes = malloc(gf_card() * sizeof(gf_t));
    randombytes(random_bytes, gf_card() * sizeof(gf_t));
    U[0] = 1;
    int i = 0;
    for (i = 0; i < gf_card(); i++)
    {
        U[i] = i;
    }
    int j = 0;
    for (j = 0; j < gf_card(); j++)
    {
        v = ((gf_t *)random_bytes)[j] % (j + 1);
        tmp = U[j];
        U[j] = U[v + 1];
        U[v + 1] = tmp;
    }
    memcpy(vect, U + 1, (m) * sizeof(gf_t));
    free(random_bytes);
}

void rs_support(gf_t *S, gf_t *L)
{
    unsigned char *random_bytes = malloc(ss_length/8);
    randombytes(random_bytes, ss_length/8);
    unsigned long int seed = 0;
    int i;
    for (i = 0; i < sizeof(random_bytes); i++)
    {
        seed = (seed << 8) | random_bytes[i];
    }
    // srand(seed);
    time_t t;

    srand(((unsigned)time(&t)) + seed);
    int k, j;
    gf_t beta = gf_pow(2, seed);
    // make random i in  [1, l − 1] and i^l=1
    i = 1; // pour l'instant
    do
    {
        i = rand() % gf_ord(); // generate a random number between 1 and gf_ord()-1
    } while (gcd(gf_ord(), i) != 1);
    gf_t gamma = gf_pow(beta, i);
    gf_t *Sgamma = (gf_t *)calloc(gf_ord(), sizeof(gf_t));

    Sgamma[0] = gf_unit();
    Sgamma[1] = gamma;
    for (k = 2; k < gf_ord(); k++)
    {
        Sgamma[k] = gf_mul(gamma, Sgamma[k - 1]);
        // printf("%d ", Sgamma[k]);
    }

    for (j = 0; j < gf_ord(); j++)
    {
        // make random i_j and i_jbetta
        int i_j = rand() % gf_ord();
        int i_jbetta = rand() % gf_ord();
        L[j] = gf_pow(2, i_jbetta);
        gf_t alphaj = gf_pow(2, i_j);
        S[j] = gf_mul(alphaj, Sgamma[j]);
    }
}

int key_pair_gen()
{
    //int return_value = 1;
    gf_t *S, *L;
    init_gf(EXTENSION_DEGREE);
    int n = gf_ord();
    int k = n/2;
    matrix_t H = init_matrix(k, n);
    binarymatrix_t punct_mat;
    //while (return_value != punct_mat.row_numbers)
    //{
    S = (gf_t *)calloc(n, sizeof(gf_t));
    L = (gf_t *)calloc(n, sizeof(gf_t));
    rs_support(S, L);
    no_binary_reed_solomon_secret_check_matrix(H, S, L);
    display_no_binary_matrix(H);
    binarymatrix_t exp_H = init_binary_matrix(H.row_numbers * EXTENSION_DEGREE, H.column_numbers * EXTENSION_DEGREE);
    expansion_check_mat(H, exp_H);
    display_binary_matrix(exp_H);
    binarymatrix_t *proj_mats = random_max_rank_matrix_list(n, EXT_MU);
    punct_mat = init_binary_matrix(H.row_numbers * EXTENSION_DEGREE, H.column_numbers * EXT_MU);
    punct_mat = punct_block_matrix(exp_H, proj_mats);
    printf("Colonne=%d\n",punct_mat.column_numbers);
    printf("Ligne=%d\n",punct_mat.row_numbers);
    //display_binary_matrix(punct_mat);
    //gauss_elim(punct_mat);
    //display_binary_matrix(punct_mat);
    //}
    //binary_matrix_free(punct_mat);
    //binary_matrix_free(exp_H);
    //no_binary_matrix_free(H);
    return 1;
}