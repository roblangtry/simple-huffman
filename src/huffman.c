#include "huffman.h"




struct huffman_tree_node * initialise_huffman_tree(struct probability_list list){
    int i;
    struct probability_point point;
    struct huffman_tree_node node;
    struct huffman_tree_node * nodes;
    i = 0;
    nodes = (struct huffman_tree_node *) malloc(sizeof(struct huffman_tree_node) * list.list_length);
    while(i < list.list_length){
        point = list.list[i];
        node.left = NULL;
        node.right = NULL;
        node.frequency = point.occurrences;
        node.value = point.value;
        nodes[i] = node;
        i++;
    }
    return nodes;
}
int compare_huffman_tree_node_pointers (const void * a, const void * b)
{
    int value_a, value_b;
    struct huffman_tree_node ** a_ptr_ptr;
    a_ptr_ptr = (struct huffman_tree_node**) a;
    struct huffman_tree_node * a_ptr;
    a_ptr = *a_ptr_ptr;
    struct huffman_tree_node ** b_ptr_ptr;
    b_ptr_ptr = (struct huffman_tree_node**) b;
    struct huffman_tree_node * b_ptr;
    b_ptr = *b_ptr_ptr;
    value_a = (*a_ptr).frequency;
    value_b = (*b_ptr).frequency;
    return (value_b - value_a);
}


struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left){
    struct huffman_tree_node * new_node;
    new_node = (struct huffman_tree_node *) malloc(sizeof(struct huffman_tree_node));
    (*new_node).frequency = right->frequency + left->frequency;
    (*new_node).value = 0;
    (*new_node).right = right;
    (*new_node).left = left;
    return new_node;
}

struct huffman_tree_node create_huffman_tree(struct probability_list list){
    int i, last, second_last, len;
    struct huffman_tree_node * node;
    struct huffman_tree_node * nodes;
    struct huffman_tree_node ** pointers;
    nodes = initialise_huffman_tree(list);
    i = 0;
    len = list.list_length;
    pointers = (struct huffman_tree_node **) malloc(sizeof(void *) * len);
    while(i < len){
        pointers[i] = &(nodes[i]);
        i++;
    }
    i = 0;
    while(i < (len - 1)){
        qsort(pointers, len - i, sizeof(void *), compare_huffman_tree_node_pointers);
        last = len - i - 1;
        second_last = len - i - 2;
        node = package_huffman_nodes(pointers[second_last], pointers[last]);
        pointers[second_last] = node;
        i++;
    }
    node = pointers[0];
    return *node;
}

void print_huffman_tree(struct huffman_tree_node node, int level){
    int i;
    if(level == 0){
        printf("=======================\n");
        printf("Huffman Tree\n");
        printf("=======================\n");
    }
    i = 0;
    while(i < level){
        printf(" ");
        i++;
    }
    printf("%d-%d\n", node.frequency, node.value);
    if(node.right != NULL){
        print_huffman_tree(*(node.right), level + 1);
    }
    if(node.left != NULL){
        print_huffman_tree(*(node.left), level + 1);
    }
    if(level == 0){
        printf("=======================\n");
    }
}