#ifndef GF_OPERATION_H
#define GF_OPERATION_H
#define MAX_EXTENSION_DEGREE 16
typedef unsigned short gf_t;

extern int gf_extension_degree, gf_cardinality, gf_order;
extern gf_t *gf_antilog;
extern gf_t *gf_log;
extern unsigned gf_prim_poly[MAX_EXTENSION_DEGREE + 1];

#define gf_extdeg() gf_extension_degree
#define gf_card() gf_cardinality
#define gf_ord() gf_order

#define gf_unit() 1
#define gf_zero() 0
#define gf_add(x, y) ((x) ^ (y))
#define gf_antilog(i) gf_antilog[i]
#define gf_log(x) gf_log[x]

#define gf_modq_1(d) (((d) & gf_ord()) + ((d) >> gf_extdeg()))

#define gf_mul_fast(x, y) ((y) ? gf_antilog(gf_modq_1(gf_log(x) + gf_log(y))) : 0)
#define gf_mul(x, y) ((x) ? gf_mul_fast(x, y) : 0)
#define gf_square(x) ((x) ? gf_antilog(gf_modq_1(gf_log(x) << 1)) : 0)
#define gf_sqrt(x) ((x) ? gf_antilog(gf_modq_1(gf_log(x) << (gf_extdeg() - 1))) : 0)
#define gf_div(x, y) ((x) ? gf_antilog(gf_modq_1(gf_log(x) - gf_log(y))) : 0)
// #define gf_inv(x) gf_antilog(gf_ord() - gf_log(x))

int gf_inv(int x);
int init_gf(int extension_deg);
gf_t gf_rand(int (*u8rnd)());
gf_t gf_pow(gf_t x, int i);

#endif