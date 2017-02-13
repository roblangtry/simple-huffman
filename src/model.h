#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "huffman.h"
#include "symbols.h"
#ifndef MODEL_CODE
#define MODEL_CODE
struct model_input {
    int no_symbols;
    struct symbol_length_pair * list;
};
struct model {
    int no_symbols;
    int * symbols;
    int length_max;
    int * no_words;
    int * base_l;
    int * offset_l;
    int * lj_limit;
};
struct model_input recursive_codeword_length(struct huffman_tree_node node, int level);
struct model_input create_model_input(struct huffman_tree_node root);
struct model create_model(struct model_input model_input);
void print_model(struct model model);
void print_model_input(struct model_input model_input);
#endif