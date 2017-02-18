#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "symbols.h"
#ifndef HUFFMAN_CODE
#define HUFFMAN_CODE
struct huffman_root_holder {
    struct huffman_tree_node * root;
    struct huffman_tree_node ** package_root;
    int no_package;
    struct huffman_tree_node * leaf_root;
};
struct huffman_tree_node {
    struct huffman_tree_node * left;
    struct huffman_tree_node * right;
    int frequency;
    int value;
};
struct huffman_tree_node * initialise_huffman_tree(struct probability_list list);
struct huffman_root_holder create_huffman_tree(struct probability_list list);
struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left);
struct huffman_tree_node * get_smallest_node(struct huffman_tree_node * leaf_nodes, int * length_leaf_nodes_list, struct huffman_tree_node ** packaged_node_pointers, int * length_packaged_list);
void print_huffman_tree(struct huffman_tree_node node, int level);
void move_forward_list(struct huffman_tree_node **package_pointers, int package_pointers_length);
void free_huffman_root(struct huffman_root_holder root);
#endif