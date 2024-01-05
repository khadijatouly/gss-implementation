#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "matrix.h"
#include "rng.h"

binarymatrix_t init_binary_matrix(int row_numbers, int column_numbers)
{
    binarymatrix_t A;
    A.row_numbers = row_numbers;
    A.column_numbers = column_numbers;
    A.rwdcnt = (1 + (column_numbers - 1) / BITS_PER_LONG);
    A.alloc_size = row_numbers * A.rwdcnt * sizeof(unsigned long);
    A.element = (unsigned long **)malloc(row_numbers * sizeof(unsigned long *));
    int i = 0;
    for (i = 0; i < row_numbers; i++)
    {
        A.element[i] = (unsigned long *)calloc((1 + (column_numbers - 1) / BITS_PER_LONG), sizeof(unsigned long));
    }
    return A;
}

binarymatrix_t init_binary_matrix_id(int ordre)
{
    binarymatrix_t A = init_binary_matrix(ordre, ordre);
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        mat_change_coeff(A, i, i);
    }
    return A;
}

matrix_t init_matrix(int row_numbers, int column_numbers)
{
    matrix_t A;
    A.row_numbers = row_numbers;
    A.column_numbers = column_numbers;
    A.coefficient = (gf_t **)malloc(row_numbers * sizeof(gf_t *));
    int i = 0;
    for (i = 0; i < row_numbers; i++)
    {
        A.coefficient[i] = (gf_t *)calloc(column_numbers, sizeof(gf_t));
    }
    return A;
}

void binary_matrix_free(binarymatrix_t A)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        free(A.element[i]);
    }
    free(A.element);
}

void no_binary_matrix_free(matrix_t A)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        free(A.coefficient[i]);
    }
    free(A.coefficient);
}

void matrix_row_xor(binarymatrix_t A, int row_1, int row_2)
{
    int i = 0;
    for (i = 0; i < A.rwdcnt; i++)
    {
        A.element[row_1][i] ^= A.element[row_2][i];
    }
}

binarymatrix_t matrix_swap_columns(binarymatrix_t A, int column_1, int column_2)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        if (mat_coeff(A, i, column_1) != mat_coeff(A, i, column_2))
        {
            mat_change_coeff(A, i, column_1);
            mat_change_coeff(A, i, column_2);
        }
    }
    return A;
}

void display_binary_matrix(binarymatrix_t A)
{
    int i = 0; 
    int j = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
    for (j = 0; j < A.column_numbers; j++)
        {   if(j == A.column_numbers - A.row_numbers) printf("|");
            printf("%lu ", mat_coeff(A, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void display_no_binary_matrix(matrix_t A)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < A.column_numbers; j++)
        {
            printf("%d ", A.coefficient[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void transpose_binary_matrix(binarymatrix_t A, binarymatrix_t transpose_A)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < A.column_numbers; j++)
        {
            if (mat_coeff(A, i, j))
            {
                mat_set_coeff_to_one(transpose_A, j, i);
            }
        }
    }
}

void transpose_no_binary_matrix(matrix_t A, matrix_t transpose_A)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < A.column_numbers; j++)
        {
            transpose_A.coefficient[j][i] = A.coefficient[i][j];
        }
    }
}

void product_binary_matrix(binarymatrix_t A, binarymatrix_t B, binarymatrix_t product_AB)
{
    gf_t bit = 0;
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < B.column_numbers; j++)
        {
            int k = 0;
    for (k = 0; k < B.row_numbers; k++)
            {
                bit ^= mat_coeff(A, i, k) & mat_coeff(B, k, j);
            }
            if (bit)
            {
                mat_set_coeff_to_one(product_AB, i, j);
            }
            bit = 0;
        }
    }
}



int gauss_elim(binarymatrix_t M)
{
	int i, j, k, l;
	printf("entering gaussElim\n");
	//int rwdcnt = 1 + ( - 1) / __WORDSIZE;

	for (j = M.column_numbers - M.row_numbers, k = 0; (k < M.row_numbers) && (j < M.column_numbers); ++j)
	//for (j = 0, k = 0; (k < M.rown) && (j < M.rown); ++j)
	{
		// we search for a pivot
		for (i = k; i < M.row_numbers; ++i)
		{
			if (mat_coeff(M, i, j))
			{
				mat_swap_rows(M, i, k);

				break;
			}
		}

		if (i < M.row_numbers)
		{ // found
			for (l = 0; l < k; ++l)
				if (mat_coeff(M, l, j))
					matrix_row_xor(M, l, k);
			for (l = i + 1; l < M.row_numbers; ++l)
				if (mat_coeff(M, l, j))
					matrix_row_xor(M, l, k);
			++k;
		}
		else
		{ // not found
		}
	}

	return k;
}

void no_binary_reed_solomon_secret_check_matrix(matrix_t H, gf_t *S, gf_t *L)
{
    int i = 0;
    for (i = 0; i < H.column_numbers; i++)
    {
        H.coefficient[0][i] = L[i];
    }
    
    for (i = 1; i < H.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < H.column_numbers; j++)
        {
            H.coefficient[i][j] = gf_mul(H.coefficient[i - 1][j], S[j]);
        }
    }
}

/*void binary_reed_solomon_secret_check_matrix(binarymatrix_t H, gf_t *S, gf_t *L)
{
    gf_t hij = 1, h = 1;
    int i = 0;
    for (i = 0; i < H.column_numbers; i++)
    {
        H.coefficient[0][i] = L[i];
    }

    int j = 0;
    for (j = 0; j < code_length; ++j)
    {
        int i = 0;
    for (i = 0; i < H.row_numbers; ++i)
        {
            hij = gf_mul(hij, S[j]);
            h = hij;
            int k = 0;
    for (k = 0; k < EXTENSION_DEGREE; ++k)
            {
                if (h & (1 << k))
                {
                    mat_set_coeff_to_one(H, i + k * order, j);
                }
            }
        }
    }
}*/
/**
 * fonction qui permet de calculer une matrice generatrice G a partir d'une matrice de parite systematique
 */
void G_mat_pub(binarymatrix_t A, binarymatrix_t H_syst)
{
    int i = 0,k=0;
    for (i = 0; i < A.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < A.row_numbers; j++)
        {
            mat_set_coeff_to_one(A, j, j);
        }

        for (k = A.row_numbers; k < A.column_numbers; k++)
        {
            if (mat_coeff(H_syst, k - A.row_numbers, i))
            {
                mat_set_coeff_to_one(A, i, k);
            }
        }
    }
}

/*binarymatrix_t mat_from_pk(const unsigned char *pk)
{
    binarymatrix_t H;
    int r = EXTENSION_DEGREE * pol_deg * order;
    int n = code_length;
    H = init_binary_matrix(n - r, r);
    int ind;
    int i = 0;
    for (i = 0; i < H.row_numbers; i += order)
    {
        memcpy(H.element[i], pk, H.rwdcnt * sizeof(unsigned long));
        pk += H.rwdcnt * sizeof(unsigned long);
        for (int j = i + 1; j < i + order; j++)
        {
            int k = 0;
    for (k = 0; k < r; k++)
            {
                ind = j ^ k ^ i;
                if (mat_coeff(H, i, ind))
                {
                    mat_set_coeff_to_one(H, j, k);
                }
            }
        }
    }
    return H;
}*/

/**
 * fonction qui permet de calculer le produit entre un vecteur et un matrice
 * */

void product_vector_matrix(unsigned long *result, unsigned char *u, binarymatrix_t A)
{
    int i = 0;
    for (i = 0; i < A.row_numbers; i++)
    {
        if ((u[i / 8] >> (i % 8)) & 1)
        {
            int j = 0;
    for (j = 0; j < A.rwdcnt; j++)
            {
                result[j] ^= A.element[i][j];
            }
        }
    }
}

// fonction qui permet d'étendre un vecteur
void expansion(gf_t *v, int len, binarymatrix_t A)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
        int j = 0;
    for (j = 0; j < EXTENSION_DEGREE; j++)
        {
            if (v[i] & (1 << j))
            {
                mat_set_coeff_to_one(A, 0, (i * EXTENSION_DEGREE) + j);
            }
        }
    }
}

/**
 * fonction qui permet d'étendre une matrice génératrice G
 */
void expansion_gen_mat(matrix_t G, binarymatrix_t exp_G)
{
    int i = 0;
    for (i = 0; i < G.row_numbers; i++)
    {
        int j = 0;
    for (j = 0; j < EXTENSION_DEGREE; j++)
        {
            int k = 0;
    for (k = 0; k < G.column_numbers; k++)
            {
                int l = 0;
    for (l = 0; l < EXTENSION_DEGREE; l++)
                {
                    if (gf_mul(G.coefficient[i][k], gf_pow(2, j)) & (1 << l))
                    {
                        mat_set_coeff_to_one(exp_G, i * EXTENSION_DEGREE + j, k * EXTENSION_DEGREE + l);
                    }
                }
            }
        }
    }
}

/**
 * fonction qui permet d'etendre une matrice de parite
 */
void expansion_check_mat(matrix_t H, binarymatrix_t exp_H)
{
    int j = 0;
    for (j = 0; j < H.column_numbers; j++)
    {
        int k = 0;
    for (k = 0; k < EXTENSION_DEGREE; k++)
        {
            int i = 0;
    for (i = 0; i < H.row_numbers; i++)
            {
                int l = 0;
    for (l = 0; l < EXTENSION_DEGREE; l++)
                {
                    if (gf_mul(H.coefficient[i][j], gf_pow(2, k)) & (1 << l))
                    {
                        mat_set_coeff_to_one(exp_H, i * EXTENSION_DEGREE + l, j * EXTENSION_DEGREE + k);
                    }
                }
            }
        }
    }
}

void copy_column(binarymatrix_t from_exp_mat, int from_idex, binarymatrix_t dest_exp_mat, int dest_index)
{
    int i = 0;
    for (i = 0; i < from_exp_mat.row_numbers; i++)
    {
        if (mat_coeff(from_exp_mat, i, from_idex) != mat_coeff(dest_exp_mat, i, dest_index))
        {
            mat_change_coeff(dest_exp_mat, i, dest_index);
        }
    }
}

/**
 *
 */
binarymatrix_t punct_block_matrix(binarymatrix_t exp_H, binarymatrix_t *proj_mats)
{
    gf_t bit = 0;
    int j=0,r=0; int i = 0;
    binarymatrix_t punct_matrix;
    punct_matrix = init_binary_matrix(exp_H.row_numbers, (exp_H.column_numbers / EXTENSION_DEGREE) * EXT_MU);
    for (j = 0, r = 0; j < exp_H.column_numbers && (r < exp_H.column_numbers / EXTENSION_DEGREE); j += EXTENSION_DEGREE, r++)
    {
        
    for (i = 0; i < exp_H.row_numbers; i++)
        {
            int k = 0;
    for (k = 0; k < EXT_MU; k++)
            {
                int l = 0;
    for (l = 0; l < EXTENSION_DEGREE; l++)
                {
                    bit ^= mat_coeff(exp_H, i, l) & mat_coeff(proj_mats[r], l, k);
                }
                if (bit)
                {
                    mat_set_coeff_to_one(punct_matrix, i, r * EXT_MU + k);
                }
                bit = 0;
            }
        }
    }
    return punct_matrix;
}

/**
 * fonction qui permet de calculer le dual du multiplieur d'un code de reed solomon donne
 */
gf_t *multiplier_dual(gf_t *S, gf_t *L)
{
    int prod = 1;
    gf_t *L_dual;
    L_dual = (gf_t *)malloc(gf_ord() * sizeof(gf_t));
    int i = 0;
    for (i = 0; i < gf_ord(); i++)
    {
        int j = 0;
    for (j = 0; j < gf_ord(); j++)
        {
            if (i == j)
            {
                continue;
            }
            prod = gf_mul(prod, gf_add(L[i], gf_ord() - L[j]));
        }
        L_dual[i] = gf_inv(gf_mul(S[i], prod));
        prod = 1;
    }
    return L_dual;
}


binarymatrix_t random_invertible(int ordre)
{
    binarymatrix_t A, U, L;
    U = init_binary_matrix_id(ordre);
    L = init_binary_matrix_id(ordre);
    A = init_binary_matrix_id(ordre);
    unsigned char *random_bytes = malloc((ordre-1)/8 +1);
    randombytes(random_bytes, (ordre-1)/8 +1);
    int i = 0;
    for (i = 0; i < ordre; i++)
    {
        int j = 0;
    for (j = 0; j < ordre; j++)
        {
            if (i > j)
            {
                if ((random_bytes[j/8] >> (j%8)) & 1)
                {
                    mat_set_coeff_to_one(L, i, j);
                }
            }
            else if (i < j)
            {
                if ((random_bytes[j/8] >> (j%8)) & 1)
                {
                    mat_set_coeff_to_one(U, i, j);
                }
            }
        }
        randombytes(random_bytes, (ordre-1)/8 +1);
    }
    product_binary_matrix(L, U, A);
    free(random_bytes);
    return A;
}

binarymatrix_t random_max_rank_matrix(int mu)
{
    binarymatrix_t A, B;
    A = init_binary_matrix(EXTENSION_DEGREE, mu);
    B = random_invertible(EXTENSION_DEGREE);
    int i = 0;
    for (i = 0; i < EXTENSION_DEGREE; i++)
    {
        int j = 0;
    for (j = 0; j < mu; j++)
        {
            if (mat_coeff(B, i, j))
            {
                mat_set_coeff_to_one(A, i, j);
            }
        }
    }
    return A;
}

binarymatrix_t *random_max_rank_matrix_list(int size, int mu)
{
    binarymatrix_t *proj_mats;
    proj_mats = (binarymatrix_t *)calloc(size, sizeof(binarymatrix_t));
    int i = 0;
    for (i = 0; i < size; i++)
    {
        proj_mats[i] = random_max_rank_matrix(mu);
    }
    return proj_mats;
}
void print_vect(gf_t *v, int len, char *text,float scale)
{
	int i;
	printf("\\[\\Scale[%f]{\n",scale);
	printf("$$%s(", text);
	for (i = 0; i < len; i++)
	{
		printf("u^{%d}", gf_log[v[i]]);
		if((i+1)!=len)printf(",");
	}
	printf(")$$\n");
	printf("}\\]\n");
}
void display_no_binary_vect(gf_t *v, int len)
{
    int i = 0;
    for (i = 0; i < len; i++)
    {
       
            printf("%d ", v[i]);
        
       
    }
    printf("\n");
}

/*binarymatrix_t *generate_subspaces(int dimension, int size)
{
    unsigned char *random_bytes = malloc(dimension * sizeof(int));
    random(random_bytes, dimension);
    binarymatrix_t *proj_mats;
    proj_mats = (binarymatrix_t *)calloc(size, sizeof(binarymatrix_t));
    int i = 0;
    for (i = 0; i < size; i++)
    {
        proj_mats[i] = init_binary_matrix(EXTENSION_DEGREE, dimension);
        int j = 0;
    for (j = 0; j < dimension; j++)
        {
            int k = 0;
    for (k = 0; k < EXTENSION_DEGREE; k++)
            {

                if (gf_mul((random_bytes[j] % gf_ord() + 1), gf_pow(2, k)) & (1 << k))
                {
                    mat_set_coeff_to_one(proj_mats[i], k, j);
                }
            }
            printf("rand = %d ", random_bytes[j] % gf_ord() + 1);
        }
        printf("\n");
        random(random_bytes, dimension);
    }
    free(random_bytes);
    return proj_mats;
}*/