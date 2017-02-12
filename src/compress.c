#include "compress.h"
int run_compression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    int value;
    struct probability_list list;
    struct huffman_tree_node huffman_root;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        fprintf(output_file_pointer, "%d\n", value);
    }

    list = evaluate_symbol_probabilities(input_file_pointer);
    sort_symbol_probabilities(&list);
    if (verbose_flag == 1){
        print_symbol_frequencies(list);
    }
    huffman_root = create_huffman_tree(list);
    if (verbose_flag == 1){
        print_huffman_tree(huffman_root, 0);
    }
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}
struct probability_list evaluate_symbol_probabilities(FILE * input_file_pointer){
    int temp_value;
    struct probability_list list;
    rewind(input_file_pointer);
    list = initialise_probabilities_list(input_file_pointer);
    while (fscanf(input_file_pointer, "%d\n", &temp_value) != EOF) {
        add_to_probability_list(&list, temp_value);
    }
    return list;
}
struct probability_list initialise_probabilities_list(FILE * input_file_pointer){
    int value;
    fscanf(input_file_pointer, "%d\n", &value);
    struct probability_list list;
    struct probability_point point;
    point.value = value;
    point.occurrences = 1;
    list.list_length = 1;
    list.list = malloc(sizeof(struct probability_point));
    list.list[0] = point;
    return list;
}
void add_to_probability_list(struct probability_list * list, int value){
    int i;
    int list_len;
    int added;
    struct probability_point point;
    list_len = (*list).list_length;
    i = 0;
    added = 0;
    while (i < list_len && added == 0){
        point = (*list).list[i];
        if(point.value == value){
            point.occurrences = point.occurrences + 1;
            added = 1;
            (*list).list[i] = point;
        }
        i++;
    }
    if(added == 0){
        (*list).list_length = (*list).list_length + 1;
        (*list).list = realloc((*list).list, sizeof(struct probability_point) * (*list).list_length);
        point.value = value;
        point.occurrences = 1;
        (*list).list[(*list).list_length - 1] = point;
    }
}

int compare_probability (const void * a, const void * b)
{
    int value_a, value_b;
    value_a = (*(struct probability_point*)a).occurrences;
    value_b = (*(struct probability_point*)b).occurrences;
    return (value_b - value_a);
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

void sort_symbol_probabilities(struct probability_list * list){
   qsort((*list).list, (*list).list_length, sizeof(struct probability_point), compare_probability);
}


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
    (*new_node).frequency = right->frequency + left->frequency;
    (*new_node).value = 0;
    (*new_node).right = right;
    (*new_node).left = left;
    return new_node;
}

struct huffman_tree_node create_huffman_tree(struct probability_list list){
    int i, last, second_last, len, j;
    struct huffman_tree_node * node;
    struct huffman_tree_node * nodes;
    struct huffman_tree_node * last_node_ptr;
    struct huffman_tree_node * second_last_node_ptr;
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

void print_symbol_frequencies(struct probability_list list){
    int symbol, freq, value;
    value = 0;
    printf("=======================\n");
    printf("%10s | %10s\n", "Symbol", "Frequency");
    printf("=======================\n");
    while(list.list_length > value){
        symbol = list.list[value].value;
        freq = list.list[value].occurrences;
        printf("%10d | %10d\n", symbol, freq);
        value++;
    }
    printf("=======================\n");
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
