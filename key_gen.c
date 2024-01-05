#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>          
#include <assert.h>

#include "key_gen.h"
#include "gf_operation.h"

void generate_random_vector(int m, gf_t *vect)
{
    int v;
    gf_t tmp;
    gf_t *U;
    int i=0;
    U = (gf_t *)calloc(gf_card(), sizeof(gf_t));
    srand(time(NULL));
    unsigned char buffer[512];
    int urandom = open("/dev/urandom", O_RDONLY);

    if (urandom == -1) {
        perror("Erreur lors de l'ouverture de /dev/urandom");
        return 1;
    }
    ssize_t bytesRead = read(urandom, buffer, sizeof(buffer));


    unsigned char *random_bytes = malloc(gf_card() * sizeof(gf_t));
    unsigned char entropy_input[48];
    unsigned char personalization_string[48];
    
    for (i=0; i<48; i++){
        entropy_input[i] = buffer[i];
        personalization_string[i] = rand();
        }

    randombytes_init(entropy_input, personalization_string, 256);
    randombytes(random_bytes, gf_card() * sizeof(gf_t));
    U[0] = 1;
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
    close(urandom);
}

void init_random_element(gf_t *U) {
	int i, j, v;
	gf_t tmp;
	unsigned char *random_bytes = 0;
	random_bytes = malloc(gf_ord() * sizeof(gf_t));
	randombytes(random_bytes, gf_ord() * sizeof(gf_t));
   
	for (i = 1; i <= gf_ord(); i++) {
		U[i] = i;
	}

	for (j = 1; j < gf_ord(); j++) {

		v = ((gf_t *) random_bytes)[j] % (j + 1);
		tmp = U[j];
		U[j] = U[v + 1];
		U[v + 1] = tmp;
	}

	free(random_bytes);
}



void rs_support(gf_t *S, gf_t *L)
{   
    gf_t *U, *V;
    int i=0; 
    U = (gf_t *)calloc(gf_card(), sizeof(gf_t));
    V= (gf_t *)calloc(gf_card(), sizeof(gf_t));
    srand(time(NULL));
    generate_random_vector(code_length, U);
    generate_random_vector(code_length, V);
    for ( i = 0; i < code_length; i++)
    {
        S[i]=U[i];
        L[i]=gf_antilog[V[i]%gf_card()];
    }

    display_no_binary_vect(L,code_length);
    
}

void cauchy_support_centro(gf_t *z, gf_t *L, gf_t alpha)
{
	int i, b;
	gf_t *U;
	int test_u = 0, test_v = 0, test_u_inter_v = 0;
	U = (gf_t *)calloc(gf_card(), sizeof(gf_t));

	do
	{
		init_random_element(U);
		for (i = 0; i < order / 2; i++)
		{
			z[i] = U[i + 1];
			z[order - 1 - i] = z[i] ^ alpha;
			Remove_From_U(z[i], U);
			Remove_From_U(z[order - 1 - i], U);
		}
		for (b = 0; b < code_length / order; b++)
		{
			for (i = 0; i < order / 2; i++)
			{
				L[b * order + i] = U[order + b * order + i];
				L[b * order + order - 1 - i] = L[b * order + i] ^ alpha;
				Remove_From_U(L[b * order + i], U);
				Remove_From_U(L[b * order + order - 1 - i], U);
			}
		}
		test_u = Test_disjoint(L, code_length);
		test_v = Test_disjoint(z, order);
		test_u_inter_v = disjoint_test(z, L);

	} while ((test_u != 0) || (test_v != 0) || (test_u_inter_v != 0));
}

void Remove_From_U(gf_t elt, gf_t *U)
{
	int k;
	for (k = 0; k <= gf_ord(); k++)
	{
		if (U[k] == elt)
		{
			U[k] = 0;
			break;
		}
	}
}

int Test_disjoint(gf_t *L, int n)
{
	int i, j;
	for (i = 0; i < n; i++)
	{
		for (j = i + 1; j < n; j++)
		{
			if (L[i] == L[j])
			{
				return -1;
			}
		}
	}
	return 0;
}

int disjoint_test(gf_t *u, gf_t *v)
{
	int i, j;
	for (i = 0; i < (order); i++)
	{
		for (j = 0; j < code_length; j++)
		{
			if (u[i] == v[j])
			{
				return -1;
			}
		}
	}
	return 0;
}


int key_pair_gen()
{
    //int return_value = 1;
    
    gf_t *S, *L,*z;
	int return_value = 1;
    init_gf(EXTENSION_DEGREE);
    assert( code_length <= gf_card() );
    assert( mt <= code_length );
    //int n = code_length;
    //int k = n/2;
    matrix_t H = init_matrix(t, code_length);
    binarymatrix_t punct_mat;
    while (return_value != punct_mat.row_numbers)
    {

    S = (gf_t *)calloc(code_length, sizeof(gf_t));
    z = (gf_t *)calloc(order, sizeof(gf_t));
    L = (gf_t *)calloc(code_length, sizeof(gf_t));
    //rs_support(S, L);
    cauchy_support_centro(z, L, 1);
    //display_no_binary_vect(S,code_length);
	//display_no_binary_vect(L,code_length);
   // no_binary_reed_solomon_secret_check_matrix(H, S, L);
    Cauchy_check_matrix(H, z, L);
    display_no_binary_matrix(H);
    printf("Colonne=%d\n",H.column_numbers);
    printf("Ligne=%d\n",H.row_numbers);
    binarymatrix_t exp_H = init_binary_matrix(H.row_numbers * EXTENSION_DEGREE, H.column_numbers * EXTENSION_DEGREE);
    expansion_check_mat(H, exp_H);
    display_binary_matrix(exp_H);
    binarymatrix_t *proj_mats = random_max_rank_matrix_list(code_length, EXT_MU);
    punct_mat = init_binary_matrix(H.row_numbers * EXTENSION_DEGREE, H.column_numbers * EXT_MU);
    punct_mat = punct_block_matrix(exp_H, proj_mats);
    //printf("Colonne=%d\n",punct_mat.column_numbers);
    //printf("Ligne=%d\n",punct_mat.row_numbers);
    
    display_binary_matrix(proj_mats[0]);
    return_value=gauss_elim(punct_mat);
    //display_binary_matrix(punct_mat);
    

    if (return_value != punct_mat.row_numbers)
		{
			binary_matrix_free(punct_mat);
		}
    //binary_matrix_free(punct_mat);
    //binary_matrix_free(exp_H);
    //no_binary_matrix_free(H);
    
    }
    
    return 1;
}