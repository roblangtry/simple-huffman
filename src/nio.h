#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "constants.h"
#ifndef NIO_CODE
#define NIO_CODE
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
void start_breader(t_breader * reader);
uint32_t get_bit(t_breader * reader, uint32_t * value);
void start_bwriter(t_bwriter * writer);
void write_bit(uint32_t b, t_bwriter * writer);
void flush_bits(t_bwriter * writer);
size_t get_file_size(FILE * stream);
int atend(t_breader * reader);
void start_iwriter(t_iwriter * writer);
void write_int(uint32_t i, t_iwriter * writer);
void flush_ints(t_iwriter * writer);
#endif