#include <stdio.h>
#include "model.h"
#include "bitlevel_io.h"
#ifndef DECOMPRESSION_CODE
#define DECOMPRESSION_CODE
int run_decompression(char *input_filename, char *output_filename, int verbose_flag);
void write_decompressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model);
#endif