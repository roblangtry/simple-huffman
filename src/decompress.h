#include <stdio.h>
#include <stdint.h>
#include "model.h"
#include "bitlevel_io.h"
#ifndef DECOMPRESSION_CODE
#define DECOMPRESSION_CODE
int run_decompression(char *input_filename, char *output_filename, uint64_t verbose_flag, uint64_t general);
void write_decompressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model, uint64_t general);
#endif