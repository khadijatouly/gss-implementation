#include <stdio.h>
#include <stdlib.h>

#include "decipher.h"
#include "param.h"
#include "key_gen.h"

/**
 * Une fonction qui prend en entree le mot chiffre et la cle secrete et 
 * calcule le produit entre c et le transpose des n matrices qui 
 * composent la cle secrete
*/
binarymatrix_t compute_syndrome(binarymatrix_t cipher, binarymatrix_t * proj_mats, int size){
    binarymatrix_t *proj_mats_transpose = random_max_rank_matrix_list_transpose(size, proj_mats);
    binarymatrix_t syndrome = init_binary_matrix(1, (cipher.column_numbers/EXT_MU)*EXTENSION_DEGREE);
    gf_t bit = 0;
    for (int i = 0; i < syndrome.column_numbers; i++)
    {
        int depart = (i/EXTENSION_DEGREE)*EXT_MU;
        for (int k = depart; k < depart+EXT_MU; k++)
        {
            bit ^= mat_coeff(cipher, 0, k) & mat_coeff(proj_mats_transpose[i/EXTENSION_DEGREE], k%EXT_MU, i%EXTENSION_DEGREE); 
        }
        if (bit)
        {
            mat_set_coeff_to_one(syndrome,0,i);
        }
        bit = 0;
    }
    return syndrome;
}

void expansion_inverse(matrix_t exp_inv, binarymatrix_t exp, int base) {
    gf_t bit = 0;
    for (int i = 0; i < exp.column_numbers; i+=base)
    {
        for (int k = 0; k < base; k++)
        {
            bit ^= (gf_mul(mat_coeff(exp, 0, i+k), gf_pow(2,k)));
        }
        exp_inv.coefficient[0][i/base] ^= bit;
        bit = 0;
        
    }
}