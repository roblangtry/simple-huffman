#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "symbols.h"
#ifndef HUFFMAN_CODE
#define HUFFMAN_CODE
struct huffman_tree_node {
    struct huffman_tree_node * left;
    struct huffman_tree_node * right;
    int frequency;
    int value;
};

struct huffman_tree_node create_huffman_tree(struct probability_list list);
struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left);
int compare_huffman_tree_node_pointers (const void * a, const void * b);
void print_huffman_tree(struct huffman_tree_node node, int level);
#endif