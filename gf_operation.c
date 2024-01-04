#include <stdio.h>
#include <stdlib.h>

#include "gf_operation.h"

int gf_extension_degree, gf_cardinality, gf_order;
gf_t *gf_antilog;
gf_t *gf_log;

unsigned gf_prim_poly[MAX_EXTENSION_DEGREE + 1] = {
    01,      /* extension degree 0 (!) never used */
    03,      /* extension degree 1 (!) never used */
    07,      /* extension degree 2 */
    013,     /* extension degree 3 */
    023,     /* extension degree 4 */
    045,     /* extension degree 5 */
    0103,    /* extension degree 6 */
    0203,    /* extension degree 7 */
    0435,    /* extension degree 8 */
    01041,   /* extension degree 9 */
    02011,   /* extension degree 10 */
    04005,   /* extension degree 11 */
    010123,  /* extension degree 12 */
    020033,  /* extension degree 13 */
    042103,  /* extension degree 14   */
    0100003, /* extension degree 15 */
    0210013  /* extension degree 16 */

};

void view_log(gf_t *tab)
{   int i = 0;
    for ( i = 0; i < gf_card(); ++i)
    {
        printf("i : %d =  %d \n", i, tab[i]);
    }
}

// fonction qui caclule la table exponentielle de Fp
void init_gf_antilog()
{
    printf("je suis dans la fonction antilog \n");
    gf_antilog = (gf_t *)malloc(gf_card() * sizeof(gf_t));
    gf_antilog[0] = gf_antilog[gf_ord()] = 1;
    int i = 1;
    for (i = 1; i < gf_ord(); ++i)
    {
        gf_antilog[i] = gf_antilog[i - 1] << 1;
        if (gf_antilog[i] & (1 << gf_extdeg()))
        {
            gf_antilog[i] ^= gf_prim_poly[gf_extdeg()];
        }
    }
    //view_log(gf_antilog);
}

// fonction qui calcule la table log de Fp
void init_gf_log()
{
    printf("je suis dans la fonction log \n");
    gf_log = (gf_t *)malloc(gf_card() * sizeof(gf_t));
    gf_log[0] = gf_ord(); // log of 0 par convention
    int i = 0;
    for (i = 0; i < gf_ord(); ++i)
    {
        gf_log[gf_antilog[i]] = i;
    }
    //view_log(gf_log);
}

// fonction qui permet d'initialiser les tales log et antilog a partir du degre du polynome
int init_done = 0;

int init_gf(int extension_deg)
{
    if (extension_deg > MAX_EXTENSION_DEGREE)
    {
        fprintf(stderr, "Le degré d'extension %d n'est pas implémenté \n", extension_deg);
        exit(0);
    }

    if (init_done != extension_deg)
    {
        if (init_done)
        {
            free(gf_antilog);
            free(gf_log);
        }

        init_done = gf_extension_degree = extension_deg;
        gf_cardinality = 1 << extension_deg;
        gf_order = gf_cardinality - 1;
        init_gf_antilog();
        init_gf_log();
    }

    return 1;
}

gf_t gf_pow(gf_t x, int i)
{
    if (i == 0)
        return 1;
    else if (x == 0)
        return 0;
    else
    {
        while (i >> gf_extdeg())
            i = (i & (gf_ord())) + (i >> gf_extdeg());
        i *= gf_log(x);
        while (i >> gf_extdeg())
            i = (i & (gf_ord())) + (i >> gf_extdeg());
        return gf_antilog(i);
    }
}

gf_t gf_rand(int (*u8rnd)())
{
    return (u8rnd() ^ (u8rnd() << 8)) & gf_ord();
}