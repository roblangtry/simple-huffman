#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#ifndef SYMBOLS_CODE
#define SYMBOLS_CODE
#define MAX_SYMBOL 524288
#define BUFFER_SIZE 4096

struct symbol_length_pair {
    uint64_t symbol;
    uint64_t length;
};
struct probability_point64_t {
    uint64_t value;
    uint64_t occurrences;
};
struct probability_list {
    uint64_t list_length;
    struct probability_point64_t * list;
};
uint64_t compare_symbol_length (const void * a, const void * b);
struct probability_list evaluate_symbol_probabilities(FILE * input_file, uint64_t general);
struct probability_list initialise_probabilities_list(FILE * input_file, uint64_t general);
void add_to_probability_list(struct probability_list * list, struct probability_point64_t point64_t);
uint64_t compare_probability (const void * a, const void * b);
void sort_symbol_probabilities(struct probability_list * list);
void print64_t_symbol_frequencies(struct probability_list list);
void free_probability_list(struct probability_list list);
uint64_t add_to_val_list(uint64_t * list, uint32_t value);
#endif