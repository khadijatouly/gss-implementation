#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cipher.h"
#include "param.h"
#include "key_gen.h"

void gen_error_vector(gf_t *v, int len) {
    srand((unsigned int)time(NULL));
    int weight = rand()%t +1;
    printf("Le poids = %d \n", weight);
    for (int i = 0; i < weight; i++)
    {
        int position = rand()%len;
        if (v[position])
        {
            position = rand()%len;
        }
        
        printf("La position = %d \n", position);
        v[position] ^=1;
    }  
    display_no_binary_vect(v, len);

}

void gen_message(gf_t *binary_message, int len) {
    gf_t* message = (gf_t*)calloc(len, sizeof(gf_t));
    srand(time(NULL));
    generate_random_vector(len, message);
    for (int i = 0; i < len; i++)
    {
        binary_message[i] = message[i]%2;
    }
    display_no_binary_vect(message, len);
    display_no_binary_vect(binary_message, len);
    
}

void concat(gf_t *concate, gf_t *u,gf_t *v) {
    for (int i = 0; i < R.row_numbers; i++)
    {
        concate[i] = u[i];
    }
    for (int j = R.row_numbers;  j< R.row_numbers+R.column_numbers; j++)
    {
        concate[j] = v[j-R.row_numbers];
    } 
}

void add_error_vector(gf_t *cipher, gf_t *concate, binarymatrix_t exp_e) {
    for (int i = 0; i < R.column_numbers; i++)
    {
        cipher[i] = concate[i]^exp_e.element[0][i];
    }
    
}

void cipher() {
    init_gf(EXTENSION_DEGREE);
    key_pair_gen();

    gf_t *message = (gf_t *)calloc(R.row_numbers, sizeof(gf_t));
    gen_message(message, R.row_numbers);
    gf_t *result = (gf_t *)calloc(R.row_numbers, sizeof(gf_t));
    product_vector_matrix(result, message, R);
    //display_binary_matrix(R);
    printf("Le resultat \n");
    display_no_binary_vect(result, R.column_numbers);
    gf_t * concate = (gf_t *)calloc(R.row_numbers+R.column_numbers, sizeof(gf_t));
    concat(concate, message, result);
    printf("La concatenation \n");
    display_no_binary_vect(concate, R.row_numbers+R.column_numbers);
    gf_t *e = (gf_t*)calloc(code_length, sizeof(gf_t));
    gen_error_vector(e, code_length);
    binarymatrix_t exp_e = init_binary_matrix(1, R.row_numbers+R.column_numbers);
    expansion(e, code_length, exp_e, EXT_MU);
    gf_t * cipher = (gf_t *)calloc(R.row_numbers+R.column_numbers, sizeof(gf_t));
    printf("Le vecteur d'erreur \n");
    display_binary_matrix(exp_e);
    add_error_vector(cipher, concate, exp_e);
    printf("Le chiffre \n");
    display_no_binary_vect(cipher, R.row_numbers+R.column_numbers);
}