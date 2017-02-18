#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "huffman.h"
#include "symbols.h"
#include "bitlevel_io.h"
#include "elias.h"
#ifndef MODEL_CODE
#define MODEL_CODE
struct model_input {
    int message_length;
    int no_symbols;
    struct symbol_length_pair * list;
};
struct symbol_offset
{
    int offset;
    int symbol;
};
struct model {
    int message_length;
    int no_symbols;
    int * symbols;
    struct symbol_offset * binary_search_table;
    int length_max;
    int * no_words;
    int * base_l;
    int * offset_l;
    int * lj_limit;
};
int compare_symbol_offset (const void * a, const void * b);
void handle_symbol_data_for_model(struct model_input model_input, struct model * model_ptr);
int find_symbol_offset(int symbol, struct model model);
struct model_input recursive_codeword_length(struct huffman_tree_node node, int level);
struct model_input create_model_input(struct huffman_tree_node root);
struct model create_model(struct model_input model_input);
void print_model(struct model model);
void print_model_input(struct model_input model_input);
void write_model_input_to_file(struct model_input model_input, FILE * output_file_pointer);
struct model_input read_model_input_from_file(FILE * input_filename);
void free_model(struct model model);
void free_model_input(struct model_input model_input);
#endif