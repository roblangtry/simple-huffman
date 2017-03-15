#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef SYMBOLS_CODE
#define SYMBOLS_CODE
struct symbol_length_pair {
    int symbol;
    int length;
};
struct probability_point {
    int value;
    int occurrences;
};
struct probability_list {
    int list_length;
    struct probability_point * list;
};
int compare_symbol_length (const void * a, const void * b);
struct probability_list evaluate_symbol_probabilities(FILE * input_file, int general);
struct probability_list initialise_probabilities_list(FILE * input_file, int general);
void add_to_probability_list(struct probability_list * list, struct probability_point point);
int compare_probability (const void * a, const void * b);
void sort_symbol_probabilities(struct probability_list * list);
void print_symbol_frequencies(struct probability_list list);
void free_probability_list(struct probability_list list);
int * add_to_val_list(int * list, int * size, int value);
#endif