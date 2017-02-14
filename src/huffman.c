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


struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left){
    struct huffman_tree_node * new_node;
    new_node = (struct huffman_tree_node *) malloc(sizeof(struct huffman_tree_node));
    new_node->frequency = right->frequency + left->frequency;
    new_node->value = 0;
    new_node->right = right;
    new_node->left = left;
    return new_node;
}
struct huffman_tree_node * yield_from_package(struct huffman_tree_node ** packaged_node_pointers, int * length_packaged_list){
    struct huffman_tree_node * ptr;
    ptr = packaged_node_pointers[0];
    move_forward_list(packaged_node_pointers, (*length_packaged_list));
    (*length_packaged_list) = (*length_packaged_list) - 1;
    return ptr;
}
struct huffman_tree_node * yield_from_leaf(struct huffman_tree_node * leaf_nodes, int * length_leaf_nodes_list){
    (*length_leaf_nodes_list) = (*length_leaf_nodes_list) - 1;
    return &(leaf_nodes[(*length_leaf_nodes_list)]);
}
struct huffman_tree_node * get_smallest_node(struct huffman_tree_node * leaf_nodes, int * length_leaf_nodes_list, struct huffman_tree_node ** packaged_node_pointers, int * length_packaged_list){
    int f1, f2;
    if((*length_leaf_nodes_list) == 0){
        // if there are no leaves take the smallest packaged node
        // then shuffle the rest forward
        yield_from_package(packaged_node_pointers, length_packaged_list);
    } else if((*length_packaged_list) == 0){
        // if there are no packaged nodes take the smallest leaf node
        yield_from_leaf(leaf_nodes, length_leaf_nodes_list);
    } else {
        f1 = packaged_node_pointers[0]->frequency;
        f2 = leaf_nodes[(*length_leaf_nodes_list) - 1].frequency;
        if(f1 > f2){
            yield_from_leaf(leaf_nodes, length_leaf_nodes_list);
        } else {
            yield_from_package(packaged_node_pointers, length_packaged_list);
        }
    }
}
struct huffman_tree_node create_huffman_tree(struct probability_list list){
    int length_leaf_nodes_list;
    int length_packaged_list;
    int memory_allocated_packaged_list;
    struct huffman_tree_node * leaf_nodes;
    struct huffman_tree_node * last;
    struct huffman_tree_node * second_last;
    struct huffman_tree_node * new_node;
    struct huffman_tree_node ** packaged_node_pointers;
    leaf_nodes = initialise_huffman_tree(list);
    length_leaf_nodes_list = list.list_length;
    //setup packaged
    packaged_node_pointers = malloc(sizeof(struct huffman_tree_node *));
    memory_allocated_packaged_list = 1;
    length_packaged_list = 0;
    //start fancy while
    while((length_leaf_nodes_list + length_packaged_list) > 1){
        last = get_smallest_node(leaf_nodes, &length_leaf_nodes_list, packaged_node_pointers, &length_packaged_list);
        second_last = get_smallest_node(leaf_nodes, &length_leaf_nodes_list, packaged_node_pointers, &length_packaged_list);
        new_node = package_huffman_nodes(second_last, last);
        packaged_node_pointers[length_packaged_list] = new_node;
        length_packaged_list++;
        if(length_packaged_list == memory_allocated_packaged_list){
            packaged_node_pointers = (struct huffman_tree_node **) realloc(packaged_node_pointers, sizeof(struct huffman_tree_node *) * 2 * memory_allocated_packaged_list);
            memory_allocated_packaged_list = memory_allocated_packaged_list * 2;
        }
    }
    return (*packaged_node_pointers[0]);
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
void move_forward_list(struct huffman_tree_node **package_pointers, int package_pointers_length){
    int i = 0;
    while (i < (package_pointers_length - 1)){
        package_pointers[i] = package_pointers[i + 1];
        i++;
    }
}