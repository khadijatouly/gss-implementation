#include "gf_operation.h"
#include "param.h"

#ifndef MATRIX_H
#define MATRIX_H

typedef struct matrix
{
    int row_numbers;
    int column_numbers;
    unsigned long **element;
    int rwdcnt; // le nombre de blocs par ligne
    int alloc_size;
} binarymatrix_t;

typedef struct Matrix
{
    int row_numbers;
    int column_numbers;
    gf_t **coefficient;
} matrix_t;

#define mat_coeff(A, i, j) (((A).element[(i)][(j) / BITS_PER_LONG] >> ((j) % BITS_PER_LONG)) & 1)               // retourne l'element de A qui est a la ligne i et a la colonne j
#define mat_set_coeff_to_one(A, i, j) ((A).element[(i)][(j) / BITS_PER_LONG] |= (1UL << ((j) % BITS_PER_LONG))) // remplace l'element A[i][j] par 1
#define mat_change_coeff(A, i, j) ((A).element[(i)][(j) / BITS_PER_LONG] ^= (1UL << ((j) % BITS_PER_LONG)))     // remplace le coeff A[i][j]
#define mat_swap_rows(A, i, k)            \
    {                                     \
        unsigned long *pt = A.element[i]; \
        A.element[i] = A.element[k];      \
        A.element[k] = pt;                \
    } // echange deux lignes

binarymatrix_t init_binary_matrix(int row_numbers, int column_numbers);
binarymatrix_t init_binary_matrix_id(int ordre);
matrix_t init_matrix(int row_numbers, int column_numbers);

void binary_matrix_free(binarymatrix_t A);
void no_binary_matrix_free(matrix_t A);

void matrix_row_xor(binarymatrix_t A, int row_1, int row_2);
binarymatrix_t matrix_swap_columns(binarymatrix_t A, int column_1, int column_2);

void display_binary_matrix(binarymatrix_t A);
void display_no_binary_matrix(matrix_t A);

void transpose_binary_matrix(binarymatrix_t A, binarymatrix_t transpose_A);
void transpose_no_binary_matrix(matrix_t A, matrix_t transpse_A);

void product_binary_matrix(binarymatrix_t A, binarymatrix_t B, binarymatrix_t product_AB);
int gauss_elim(binarymatrix_t M);

void no_binary_reed_solomon_secret_check_matrix(matrix_t H, gf_t *S, gf_t *L);
void binary_reed_solomon_secret_check_matrix(binarymatrix_t H, gf_t *S, gf_t *L);

void G_mat_pub(binarymatrix_t A, binarymatrix_t H_syst);
binarymatrix_t mat_from_pk(const unsigned char *pk);

void product_vector_matrix(unsigned long *result, unsigned char *u, binarymatrix_t A);

void expansion(gf_t *v, int len, binarymatrix_t A);
void expansion_gen_mat(matrix_t G, binarymatrix_t exp_G);
void expansion_check_mat(matrix_t H, binarymatrix_t exp_H);

void copy_column(binarymatrix_t from_exp_mat, int from_idex, binarymatrix_t dest_exp_mat, int dest_index);

binarymatrix_t punct_block_matrix(binarymatrix_t exp_H, binarymatrix_t *proj_mats);

gf_t *multiplier_dual(gf_t *S, gf_t *L);

binarymatrix_t random_invertible(int ordre);
binarymatrix_t random_max_rank_matrix(int mu);
binarymatrix_t *random_max_rank_matrix_list(int size, int mu);

binarymatrix_t *generate_subspaces(int dimension, int size);
void print_vect(gf_t *v, int len, char *text,float scale);
void display_no_binary_vect(gf_t *v, int len);

#endif