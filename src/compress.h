#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "symbols.h"
#include "model.h"
#include "huffman.h"
#include "bitlevel_io.h"
#ifndef COMPRESSION_CODE
#define COMPRESSION_CODE
#define BUFFER_SIZE 4096
int run_compression(char *input_filename, char *output_filename, int verbose_flag, int general);
void write_compressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model, int general);
struct bitlevel_object calculate_write_object(unsigned int value, struct model model);
int calculate_offset(unsigned int value, struct model model);
#endif