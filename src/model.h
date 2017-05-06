#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "huffman.h"
#include "symbols.h"
#include "bitlevel_io.h"
#include "elias.h"
#ifndef MODEL_CODE
#define MODEL_CODE
#define MAX_SYMBOL 524288
struct model_input {
    uint64_t message_length;
    uint64_t no_symbols;
    struct symbol_length_pair * list;
};
struct symbol_offset
{
    uint64_t offset;
    uint64_t symbol;
};
struct model {
    uint64_t message_length;
    uint64_t no_symbols;
    uint64_t * symbols;
    size_t * binary_search_table;
    uint64_t length_max;
    uint64_t * no_words;
    uint64_t * base_l;
    uint64_t * offset_l;
    uint64_t * lj_limit;
};
uint64_t compare_symbol_offset (const void * a, const void * b);
void handle_symbol_data_for_model(struct model_input model_input, struct model * model_ptr);
uint64_t find_symbol_offset(uint64_t symbol, struct model model);
struct model_input recursive_codeword_length(struct huffman_tree_node node, uint64_t level);
struct model_input create_model_input(struct huffman_tree_node root);
struct model create_model(struct model_input model_input);
void print64_t_model(struct model model);
void print64_t_model_input(struct model_input model_input);
void write_model_input_to_file(struct model_input model_input, FILE * output_file_pointer);
struct model_input read_model_input_from_file(FILE * input_filename);
void free_model(struct model model);
void free_model_input(struct model_input model_input);
#endif