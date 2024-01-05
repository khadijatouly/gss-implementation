#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

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
{   gf_t *U, *V;
    int i=0; 
    U = (gf_t *)calloc(gf_card(), sizeof(gf_t));
    V= (gf_t *)calloc(gf_card(), sizeof(gf_t));
    srand(time(NULL));
    //generate_random_vector(code_length, U);
    //generate_random_vector(code_length, V);
    for ( i = 0; i < code_length; i++)
    {
        S[i]=U[i];
        L[i]=gf_antilog[V[i]%gf_card()];
    }

    
    
}

int key_pair_gen()
{
    //int return_value = 1;
    gf_t *S, *L;
	int return_value = 1;
    init_gf(EXTENSION_DEGREE);
    //int n = code_length;
    //int k = n/2;
    matrix_t H = init_matrix(t, code_length);
    binarymatrix_t punct_mat;
    while (return_value != punct_mat.row_numbers)
    {

    S = (gf_t *)calloc(code_length, sizeof(gf_t));
    L = (gf_t *)calloc(code_length, sizeof(gf_t));
    rs_support(S, L);
    display_no_binary_vect(S,code_length);
	display_no_binary_vect(L,code_length);
    no_binary_reed_solomon_secret_check_matrix(H, S, L);
    display_no_binary_matrix(H);
    printf("Colonne=%d\n",H.column_numbers);
    printf("Ligne=%d\n",H.row_numbers);
    binarymatrix_t exp_H = init_binary_matrix(H.row_numbers * EXTENSION_DEGREE, H.column_numbers * EXTENSION_DEGREE);
    expansion_check_mat(H, exp_H);
    //display_binary_matrix(exp_H);
    binarymatrix_t *proj_mats = random_max_rank_matrix_list(code_length, EXT_MU);
    punct_mat = init_binary_matrix(H.row_numbers * EXTENSION_DEGREE, H.column_numbers * EXT_MU);
    punct_mat = punct_block_matrix(exp_H, proj_mats);
    //printf("Colonne=%d\n",punct_mat.column_numbers);
    //printf("Ligne=%d\n",punct_mat.row_numbers);
    
    //display_binary_matrix(punct_mat);
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