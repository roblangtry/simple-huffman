#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "symbols.h"
#include "model.h"
#include "huffman.h"
#include "bitlevel_io.h"
#ifndef COMPRESSION_CODE
#define COMPRESSION_CODE
int run_compression(char *input_filename, char *output_filename, uint64_t verbose_flag, uint64_t general);
void write_compressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model, uint64_t general);
struct bitlevel_object calculate_write_object(uint64_t value, struct model model);
uint64_t calculate_offset(uint64_t value, struct model model);
#endif