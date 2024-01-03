#ifndef RNG_H
#define RNG_H

#define RNG_SUCCESS 0

typedef struct {
    unsigned char   Key[32];
    unsigned char   V[16];
    int             reseed_counter;
} AES256_CTR_DRBG_struct;

void    AES256_ECB(unsigned char *key, unsigned char *ctr, unsigned char *buffer);

void
AES256_CTR_DRBG_Update(unsigned char *provided_data,
                       unsigned char *Key,
                       unsigned char *V);


int randombytes(unsigned char *x, unsigned long long xlen);

#endif