#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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
struct huffman_tree_node {
    struct huffman_tree_node * left;
    struct huffman_tree_node * right;
    int frequency;
    int value;
};
struct symbol_length_pair {
    int symbol;
    int length;
};
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
int run_compression(char *input_filename, char *output_filename, int verbose_flag);
struct probability_list evaluate_symbol_probabilities(FILE * input_file);
struct probability_list initialise_probabilities_list(FILE * input_file);
void add_to_probability_list(struct probability_list * list, int value);
int compare_probability (const void * a, const void * b);
int compare_huffman_tree_node_pointers (const void * a, const void * b);
void sort_symbol_probabilities(struct probability_list * list);
struct huffman_tree_node create_huffman_tree(struct probability_list list);
struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left);
void print_symbol_frequencies(struct probability_list list);
void print_huffman_tree(struct huffman_tree_node node, int level);
struct model_input create_model_input(struct huffman_tree_node root);
struct model create_model(struct model_input model_input);
void print_model(struct model model);
#endif