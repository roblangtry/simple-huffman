#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "nio.h"
#include "constants.h"
#include "coders.h"
#ifndef NEW_CODE
#define NEW_CODE

typedef struct {
    uint32_t value;
    uint32_t freq;
} vector_t;
typedef struct {
    uint32_t no_distinct;
    uint32_t total;
    vector_t * freqs;
} C_stats_t;
typedef struct {
    C_stats_t * stats;
} C_model_t;
typedef struct {
    int placeholder;
} D_stats_t;
typedef struct {
    D_stats_t * stats;
} D_model_t;
typedef struct {
    uint32_t content[BLOCK_SIZE];
    size_t len;
} C_block_t;
typedef struct {
    uint32_t content[BLOCK_SIZE];
    size_t len;
} D_block_t;
typedef struct {
    uint32_t len;
    uint32_t val;
} io_back;
int run_new_compression();
int run_new_decompression();
int syms_used_cmpfunc (const void * a, const void * b);
int syms_used_cmpfunc2 (const void * a, const void * b);
void mr_encode_block(C_block_t * block, t_bwriter * writer);
void calculate_huffman_code(uint32_t * list, uint32_t length);
void canonical_encode(uint32_t symbol, uint32_t * base, uint32_t * offset, t_bwriter * writer);
uint32_t canonical_decode(uint32_t * value, uint32_t * buffer, uint32_t longest, uint32_t * base, uint32_t * offset, uint32_t * lj_limit, t_breader * reader);
uint32_t mr_decode_block(t_breader * reader, t_iwriter * writer, io_back * backfeed);
#endif