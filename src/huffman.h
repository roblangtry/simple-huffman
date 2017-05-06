#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "symbols.h"
#ifndef HUFFMAN_CODE
#define HUFFMAN_CODE
struct huffman_root_holder {
    struct huffman_tree_node * root;
    struct huffman_tree_node ** package_root;
    uint64_t no_package;
    struct huffman_tree_node * leaf_root;
};
struct huffman_tree_node {
    struct huffman_tree_node * left;
    struct huffman_tree_node * right;
    uint64_t frequency;
    uint64_t value;
};
struct huffman_tree_node * initialise_huffman_tree(struct probability_list list);
struct huffman_root_holder create_huffman_tree(struct probability_list list);
struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left);
struct huffman_tree_node * get_smallest_node(struct huffman_tree_node * leaf_nodes, uint64_t * length_leaf_nodes_list, struct huffman_tree_node ** ordered_packaged_node_pointers, uint64_t * length_packaged_list, uint64_t * length_ordered_packaged_list);
void print_huffman_tree(struct huffman_tree_node * node, uint64_t level);
void move_forward_list(struct huffman_tree_node **package_pointers, uint64_t package_pointers_length);
void free_huffman_root(struct huffman_root_holder root);
#endif