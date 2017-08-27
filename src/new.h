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
    uint32_t current;
    size_t length;
    size_t head;
    size_t buffer_head;
    int end;
} t_breader;
typedef struct {

} t_C_stats;
typedef struct {
    t_C_stats * stats;
} t_C_model;
typedef struct {

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
int run_decompression_model(t_D_model * model, t_breader * reader);
void start_breader(t_breader * reader);
uint get_bit(t_breader * reader, uint * value);
void start_bwriter(t_bwriter * writer);
void write_bit(uint b, t_bwriter * writer);
void flush_bits(t_bwriter * writer);
void binary_encode(uint value, uint length, t_bwriter * writer);
uint binary_decode(uint * V, uint length, t_breader * reader);
void unary_encode(uint value, t_bwriter * writer);
uint unary_decode(uint * V, t_breader * reader);
void elias_gamma_encode(uint value, t_bwriter * writer);
uint elias_gamma_decode(uint * V, t_breader * reader);
void elias_delta_encode(uint value, t_bwriter * writer);
uint elias_delta_decode(uint * V, t_breader * reader);
size_t get_file_size(FILE * stream);
int atend(t_breader * reader);
#endif