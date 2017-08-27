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
} t_vector;
typedef struct {
    uint32_t no_distinct;
    uint32_t total;
    t_vector * freqs;
} t_C_stats;
typedef struct {
    t_C_stats * stats;
} t_C_model;
typedef struct {
    int placeholder;
} t_D_stats;
typedef struct {
    t_D_stats * stats;
} t_D_model;
typedef struct {
    uint32_t content[BLOCK_SIZE];
    size_t len;
} t_C_block;
typedef struct {
    uint32_t content[BLOCK_SIZE];
    size_t len;
} t_D_block;
int run_new_compression();
int get_compression_statistics(t_C_block * block, t_C_stats * stats, t_bwriter * writer);
int build_compression_model(t_C_block * block, t_C_stats * stats, t_C_model * model, t_bwriter * writer);
int run_compression_model(t_C_block * block, t_C_model * model, t_bwriter * writer);
int run_new_decompression();
int get_decompression_statistics(t_D_stats * stats, t_breader * reader);
int build_decompression_model(t_D_stats * stats, t_D_model * model, t_breader * reader);
int run_decompression_model(t_D_model * model, t_breader * reader, t_iwriter * writer);

#endif