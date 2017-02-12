#include <stdio.h>
#include <stdlib.h>
#ifndef COMPRESSION_CODE
#define COMPRESSION_CODE
struct probability_point {
    int value;
    int occurrences;
};
struct probability_list {
    int list_length;
    struct probability_point * list;
};
int run_compression(char *input_filename, char *output_filename, int verbose_flag);
struct probability_list evaluate_symbol_probabilities(FILE * input_file);
struct probability_list initialise_probabilities_list(FILE * input_file);
void add_to_probability_list(struct probability_list * list, int value);
#endif