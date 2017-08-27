#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#ifndef NEW_CODE
#define NEW_CODE
#define SYMBOL_MAP_SIZE 524288
#define BLOCK_SIZE 131072
#define BUFFER_SZ 1048576
typedef struct {
    uint32_t buffer[BUFFER_SZ];
    uint32_t current;
    size_t head;
    size_t buffer_head;
} t_bwriter;
typedef struct {
    uint32_t buffer[BUFFER_SZ];
    size_t head;
} t_iwriter;
typedef struct {
    uint32_t buffer[BUFFER_SZ];
    uint32_t current;
    size_t length;
    size_t head;
    size_t buffer_head;
    int end;
} t_breader;
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
void start_breader(t_breader * reader);
uint32_t get_bit(t_breader * reader, uint32_t * value);
void start_bwriter(t_bwriter * writer);
void write_bit(uint32_t b, t_bwriter * writer);
void flush_bits(t_bwriter * writer);
void binary_encode(uint32_t value, uint32_t length, t_bwriter * writer);
uint32_t binary_decode(uint32_t * V, uint32_t length, t_breader * reader);
void unary_encode(uint32_t value, t_bwriter * writer);
uint32_t unary_decode(uint32_t * V, t_breader * reader);
void elias_gamma_encode(uint32_t value, t_bwriter * writer);
uint32_t elias_gamma_decode(uint32_t * V, t_breader * reader);
void elias_delta_encode(uint32_t value, t_bwriter * writer);
uint32_t elias_delta_decode(uint32_t * V, t_breader * reader);
size_t get_file_size(FILE * stream);
int atend(t_breader * reader);
void start_iwriter(t_iwriter * writer);
void write_int(uint32_t i, t_iwriter * writer);
void flush_ints(t_iwriter * writer);
uint32_t log2(uint32_t value);
#endif