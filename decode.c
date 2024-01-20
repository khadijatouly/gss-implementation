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

gf_t *syndrome(matrix_t y, gf_t *S, gf_t *L, int d) {
    gf_t *synd = (gf_t *)calloc(d-1, sizeof(gf_t));
    gf_t s=0;
    for (int i = 0; i < d-1; i++)
    {
        for (int j = 0; j < code_length; j++)
        {
            s ^= gf_mul(y.coefficient[i][j], gf_mul(gf_pow(S[j], i), L[j]));
        }
        synd[i] = s;
        s=0;
    }
    
}

gf_t syndrome_polynomial(gf_t * syndrome) {
    gf_t S = 0;
    for (int i = 0; i < sizeof(syndrome)/sizeof(syndrome[0]); i++)
    {
         S = gf_add(S, gf_mul(syndrome[i], gf_pow(2,i)));
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

void error_value(gf_t *S, gf_t *L, gf_t lambda, gf_t gamma, int *position, gf_t *error_values) {
    gf_t *inv_S = inverse_support(S);
    gf_t e;
    gf_t deriv_lambda = derive_lambda(lambda);
    for (int i = 0; i < sizeof(position)/sizeof(position[0]); i++)
    {
        e = gf_mul(gf_div((gf_ord()-S[position[i]]), L[position[i]]), gf_div(gamma%inv_S[position[i]], deriv_lambda%inv_S[position[i]]));
        error_values[i] = e;
    }
}

void decode(matrix_t y, gf_t *S, gf_t *L, int d) {
    gf_t * synd = syndrome(y, S, L, d);
    gf_t synd_pol = syndrome_polynomial(synd);
    gf_t lambda;
    gf_t gamma;
    compute_polynomials(gf_pow(2, d-1), synd_pol, lambda, gamma);
    int * error_position = calloc(t, sizeof(int));
    error_location(S, lambda, error_position);
    gf_t *error_values = calloc(t, sizeof(gf_t));
    error_value(S, L, lambda, gamma,error_position, error_values);
}