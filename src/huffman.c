#include "huffman.h"

struct huffman_tree_node * initialise_huffman_tree(struct probability_list list){
    /* Setup all the leaf nodes for the huffman tree creation */
    uint64_t index;
    struct probability_point64_t point64_t;
    struct huffman_tree_node node;
    struct huffman_tree_node * nodes;
    index = 0;
    /* Allocate the memory for the leaves */
    nodes = (struct huffman_tree_node *) malloc(sizeof(struct huffman_tree_node) * list.list_length);
    /* While there are still uninitialised leaves */
    while(index < list.list_length){
        point64_t = list.list[index];
        /* Set both the left and right pointers to NULL */
        node.left = NULL;
        node.right = NULL;
        /* Setup the node values */
        node.frequency = point64_t.occurrences;
        node.value = point64_t.value;
        /* Put the nodes uint64_to the allocated memory */
        nodes[index] = node;
        index++;
    }
    return nodes;
}


struct huffman_tree_node * package_huffman_nodes(struct huffman_tree_node * right, struct huffman_tree_node * left){
    /* Take two nodes and create a new node that point64_ts to the two given nodes */
    struct huffman_tree_node * new_node;
    /* Allocate the memory for the node */
    new_node = (struct huffman_tree_node *) malloc(sizeof(struct huffman_tree_node));
    /* Set the frequency to be the sum of the left and right node */
    new_node->frequency = right->frequency + left->frequency;
    /* Set the value to 0 as it doesnt matter in this context */
    new_node->value = 0;
    /* Set the pointers */
    new_node->right = right;
    new_node->left = left;
    return new_node;
}
struct huffman_tree_node * yield_from_package(struct huffman_tree_node ** ordered_packaged_node_pointers, uint64_t * length_packaged_list, uint64_t * length_ordered_packaged_list){
    /* Yield the lowest frequency element from the huffman tree node list */
    /* and move the rest of the list up */
    struct huffman_tree_node * ptr;
    /* Get the pointer of the first (and given the sorted nature the */
    /* least frequent node) */
    ptr = ordered_packaged_node_pointers[*length_ordered_packaged_list - *length_packaged_list];
    /* Lower the length of the list */
    (*length_packaged_list) = (*length_packaged_list) - 1;
    /* Return the pointer */
    return ptr;
}
struct huffman_tree_node * yield_from_leaf(struct huffman_tree_node * leaf_nodes, uint64_t * length_leaf_nodes_list){
    /* Return the node from the end of the list (and given the sorted nature of */
    /* therefore the least frequent element) */
    (*length_leaf_nodes_list) = (*length_leaf_nodes_list) - 1;
    return &(leaf_nodes[(*length_leaf_nodes_list)]);
}
struct huffman_tree_node * get_smallest_node(struct huffman_tree_node * leaf_nodes, uint64_t * length_leaf_nodes_list, struct huffman_tree_node ** ordered_packaged_node_pointers, uint64_t * length_packaged_list, uint64_t * length_ordered_packaged_list){
    /* Given the leaf nodes and the packaged(composite) nodes find the smallest */
    /* node and return it */
    uint64_t f1, f2;
    if((*length_leaf_nodes_list) == 0){
        /* If there are no leaves take the smallest packaged node */
        /* then shuffle the rest forward */
        return yield_from_package(ordered_packaged_node_pointers, length_packaged_list, length_ordered_packaged_list);
    } else if((*length_packaged_list) == 0){
        /* If there are no packaged nodes take the smallest leaf node */
        return yield_from_leaf(leaf_nodes, length_leaf_nodes_list);
    } else {
        /* Get the frequencies from the two lowest frequency elements */
        f1 = ordered_packaged_node_pointers[*length_ordered_packaged_list - *length_packaged_list]->frequency;
        f2 = leaf_nodes[(*length_leaf_nodes_list) - 1].frequency;
        if(f1 > f2){
            /* If the leaf node has a lower frequency return that */
            return yield_from_leaf(leaf_nodes, length_leaf_nodes_list);
        } else {
            /* Else return the packaged node pointer */
            return yield_from_package(ordered_packaged_node_pointers, length_packaged_list, length_ordered_packaged_list);
        }
    }
}
struct huffman_root_holder create_huffman_tree(struct probability_list list){
    /* Take the probability list and create a huffman tree */
    uint64_t length_leaf_nodes_list;
    uint64_t length_packaged_list;
    uint64_t memory_allocated_packaged_list;
    uint64_t allocated_packaged_ordered_list;
    struct huffman_tree_node * leaf_nodes;
    struct huffman_tree_node * last;
    struct huffman_tree_node * second_last;
    struct huffman_tree_node * new_node;
    struct huffman_tree_node * root;
    struct huffman_tree_node ** packaged_node_pointers;
    struct huffman_tree_node ** ordered_packaged_node_pointers;
    struct huffman_root_holder holder;
    /* Create the leaf nodes from the probability list */
    leaf_nodes = initialise_huffman_tree(list);
    length_leaf_nodes_list = list.list_length;
    /*setup packaged node list */
    packaged_node_pointers = malloc(sizeof(struct huffman_tree_node *));
    memory_allocated_packaged_list = 1;
    ordered_packaged_node_pointers = malloc(sizeof(struct huffman_tree_node *));
    allocated_packaged_ordered_list = 0;
    length_packaged_list = 0;
    /* While their are more than one nodes left combine nodes together */
    /* eventually coalesing uint64_to 1 node that contains pointers that can lead to */
    /* any of the leaf nodes */
    while((length_leaf_nodes_list + length_packaged_list) > 1){
        /* Get the two smallest nodes */
        last = get_smallest_node(leaf_nodes, &length_leaf_nodes_list, ordered_packaged_node_pointers, &length_packaged_list, &allocated_packaged_ordered_list);
        second_last = get_smallest_node(leaf_nodes, &length_leaf_nodes_list, ordered_packaged_node_pointers, &length_packaged_list, &allocated_packaged_ordered_list);
        /* Package them together uint64_to a single node */
        new_node = package_huffman_nodes(second_last, last);
        /* Add that node to the end of the packaged node list (thereby maint64_taining its ordering) */
        packaged_node_pointers[length_packaged_list] = new_node;
        ordered_packaged_node_pointers[allocated_packaged_ordered_list] = new_node;
        length_packaged_list++;
        allocated_packaged_ordered_list++;
        if(allocated_packaged_ordered_list == memory_allocated_packaged_list){
            /* If the packaged node list has run out of memory just double it to save on lots of partial memory allocations */
            packaged_node_pointers = (struct huffman_tree_node **) realloc(packaged_node_pointers, sizeof(struct huffman_tree_node *) * 2 * memory_allocated_packaged_list);
            ordered_packaged_node_pointers = (struct huffman_tree_node **) realloc(ordered_packaged_node_pointers, sizeof(struct huffman_tree_node *) * 2 * (memory_allocated_packaged_list + allocated_packaged_ordered_list));
            memory_allocated_packaged_list = memory_allocated_packaged_list * 2;
        }
    }
    /* Return the head of the packaged node list as this is currently the root */
    /* of a huffman tree */
    root = packaged_node_pointers[0];
    /* Free pointers list */
    free(packaged_node_pointers);
    holder.root = root;
    /*print_huffman_tree(root, 0); */
    holder.package_root = ordered_packaged_node_pointers;
    holder.no_package = allocated_packaged_ordered_list;
    holder.leaf_root = leaf_nodes;
    return holder;
}

void print_huffman_tree(struct huffman_tree_node * node, uint64_t level){
    /* print the huffman tree for debugging */
    if(level == 0){
        printf("=======================\n");
        printf("Huffman Tree\n");
        printf("=======================\n");
    }
    if(node->right != NULL){
        print_huffman_tree(node->right, level + 1);
    }
    if(node->left != NULL){
        print_huffman_tree(node->left, level + 1);
    }
    if(level == 0){
        printf("=======================\n");
    }
}
void move_forward_list(struct huffman_tree_node **package_pointers, uint64_t package_pointers_length){
    /* Take a list of pointers and move the list forward all the elements */
    uint64_t index = 0;
    while (index < (package_pointers_length - 1)){
        package_pointers[index] = package_pointers[index + 1];
        index++;
    }
    package_pointers[index] = NULL;
}

void free_huffman_root(struct huffman_root_holder root){
    uint64_t i;
    i=0;
    /* Free the uint64_ternal nodes of the huffman tree */
    while(i < root.no_package){
        free(root.package_root[i]);
        i++;
    }
    /* Free the leaf nodes */
    free(root.leaf_root);
    /* Free the pointer list in the huffman root */
    free(root.package_root);
}