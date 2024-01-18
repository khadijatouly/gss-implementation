#include <stdio.h>
#include <stdlib.h>

#include "decode.h"

gf_t euclide_etendu(gf_t a, gf_t b, gf_t *u, gf_t *v) {
    if (a==0)
    {
        *u = 0;
        *v = 1;
        return b;
    }
    gf_t u_1, v_1;
    gf_t r =  euclide_etendu(b % a, a, &u_1, &v_1);
    *u = v_1 - (b/a)*u_1;
    *v = u_1;
    return r;
    
}

gf_t syndrome_polynomial(matrix_t syndrome) {
    gf_t S = 0;
    for (int i = 0; i < syndrome.row_numbers; i++)
    {
         S = gf_add(S, gf_mul(syndrome.coefficient[i][0], gf_pow(2,i)));
    }
    return S;
    
}

void compute_polynomials(gf_t a, gf_t b, gf_t lambda, gf_t gamma) {
    gf_t u,v;
    gf_t r = euclide_etendu(a, b, &u, &v);
    gf_t lambda_0 = v%2;
    if (lambda_0==0)
    {
        fprintf(stderr, "erreur \n");
        exit(0);
    }
    gf_t c = gf_inv(lambda_0);
    lambda = gf_mul(c, v);
    gamma = gf_mul(c, r);
}

gf_t derive_lambda(gf_t lambda) {
    gf_t *lambda_binaire = (gf_t *)calloc(EXTENSION_DEGREE+1, sizeof(gf_t));
    for (int i = 0; lambda > 0; i++)
    {
        lambda_binaire[i] = lambda % 2;
        lambda = lambda / 2;
    }
    for (int i = 0; i < EXTENSION_DEGREE+1; i++)
    {
        lambda_binaire[i] = (lambda_binaire[i + 1] * (i + 1)) % 2;
    }
    gf_t deriv_lamda = 0;
    for (int j = 0; j < EXTENSION_DEGREE+1; j++)
    {
        deriv_lamda = gf_add(deriv_lamda, gf_mul(lambda_binaire[j], gf_pow(2, j)));
    }
    return deriv_lamda;
}

gf_t * inverse_support(gf_t *S) {
    gf_t *inv_S = (gf_t *)calloc(code_length, sizeof(gf_t));
    for (int i = 0; i < code_length; i++)
    {
        inv_S[i] = gf_inv(S[i]);
    }
    return inv_S;
}

void error_location(gf_t *S, gf_t lambda, int *position_error) {
    gf_t * inv_S = inverse_support(S);
    int cpt = 0;
    for (int i = 0; i < code_length; i++)
    {
        if (lambda%inv_S[i]==0)
        {
            position_error[cpt]=i;
            cpt++;
        }
        
    } 
}