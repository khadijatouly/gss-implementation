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
        //v[position] ^=1;
        v[position] = rand()%((1<<EXT_MU)-1)+1;
    }  
    display_no_binary_vect(v, len);

}

void gen_message(unsigned char *binary_message, int len) {
    gf_t* message = (gf_t*)calloc(len, sizeof(gf_t));
    srand(time(NULL));
    generate_random_vector(len, message);
    for (int i = 0; i < len; i++)
    {
        binary_message[i] = message[i]%2;
    }
    display_no_binary_vect(message, len);
    //display_no_binary_vect(binary_message, len);
    
}

void concat(binarymatrix_t concate, unsigned char *m, binarymatrix_t c) {
    for (int i = 0; i < R.row_numbers; i++)
    {
        if (m[i])
        {
            mat_set_coeff_to_one(concate,0,i);
        }
        
    }
    for (int j = R.row_numbers;  j< R.row_numbers+R.column_numbers; j++)
    {
        if (mat_coeff(c,0,j-R.row_numbers))
        {
            mat_set_coeff_to_one(concate, 0, j);
        }
    } 
}

void add_error_vector(binarymatrix_t cipher, binarymatrix_t concate, binarymatrix_t exp_e) {
    for (int i = 0; i < code_length*EXT_MU; i++)
    {
        cipher.element[0][i] ^= (concate.element[0][i]^exp_e.element[0][i]);
    }
    
}

void cipher() {
    init_gf(EXTENSION_DEGREE);
    unsigned char *message = (unsigned char *)calloc(R.row_numbers, sizeof(unsigned char));
    gen_message(message, R.row_numbers);
    binarymatrix_t result = init_binary_matrix(1, R.column_numbers);
    product_vector_matrix(result, message, R);
    //display_binary_matrix(R);
    printf("Le resultat \n");
    display_binary_matrix(result);

    binarymatrix_t concate = init_binary_matrix(1, code_length*EXT_MU);
    concat(concate, message, result);
    printf("La concatenation \n");
    display_binary_matrix(concate);

    gf_t *e = (gf_t*)calloc(code_length, sizeof(gf_t));
    gen_error_vector(e, code_length);
    binarymatrix_t exp_e = init_binary_matrix(1, code_length*EXT_MU);
    expansion(e, code_length, exp_e, EXT_MU);
    printf("Le vecteur d'erreur \n");
    display_binary_matrix(exp_e);

    binarymatrix_t cipher = init_binary_matrix(1, code_length*EXT_MU);
    add_error_vector(cipher, concate, exp_e);
    printf("Le chiffre \n");
    display_binary_matrix(cipher);
}