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
int run_new_compression();
int get_compression_statistics(C_block_t * block, C_stats_t * stats, t_bwriter * writer);
int build_compression_model(C_block_t * block, C_stats_t * stats, C_model_t * model, t_bwriter * writer);
int run_compression_model(C_block_t * block, C_model_t * model, t_bwriter * writer);
int run_new_decompression();
int get_decompression_statistics(D_stats_t * stats, t_breader * reader);
int build_decompression_model(D_stats_t * stats, D_model_t * model, t_breader * reader);
int run_decompression_model(D_model_t * model, t_breader * reader, t_iwriter * writer);

#endif