#include "compress.h"
int run_compression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    int value;
    struct probability_list list;
    struct huffman_tree_node huffman_root;
    struct model_input model_input;
    struct model model;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        fprintf(output_file_pointer, "%d\n", value);
    }

    list = evaluate_symbol_probabilities(input_file_pointer);
    sort_symbol_probabilities(&list);
    if (verbose_flag == 1)
        print_symbol_frequencies(list);
    huffman_root = create_huffman_tree(list);
    if (verbose_flag == 1)
        print_huffman_tree(huffman_root, 0);
    model_input = create_model_input(huffman_root);
    if (verbose_flag == 1)
        print_model_input(model_input);
    model = create_model(model_input);
    if (verbose_flag == 1)
        print_model(model);
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
    int i, last, second_last, len;
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

void print_model_input(struct model_input model_input){
    int i;
    printf("=======================\n");
    printf("%10s | %10s\n", "Symbol", "Length");
    printf("=======================\n");
    i = 0;
    while(i < model_input.no_symbols){
        printf("%10d | %10d\n", model_input.list[i].symbol, model_input.list[i].length);
        i++;
    }
    printf("=======================\n");
}

int compare_symbol_length (const void * a, const void * b)
{
    int value_a, value_b;
    value_a = (*(struct probability_point*)a).occurrences;
    value_b = (*(struct probability_point*)b).occurrences;
    return (value_a - value_b);
}

struct model_input recursive_codeword_length(struct huffman_tree_node node, int level)
{
    struct model_input this_mi, left_mi, right_mi;
    struct symbol_length_pair sl_pair;
    int i;
    sl_pair.symbol = node.value;
    sl_pair.length = level;
    if(node.right == NULL && node.left == NULL){
        this_mi.no_symbols = 1;
        this_mi.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair));
        this_mi.list[0] = sl_pair;
    } else {
        this_mi.no_symbols = 0;
        this_mi.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair));
    }
    if(node.right != NULL){
        right_mi = recursive_codeword_length(*(node.right), level + 1);
        this_mi.list = realloc(this_mi.list, sizeof(struct symbol_length_pair) * (this_mi.no_symbols + right_mi.no_symbols));
        i = 0;
        while (i < right_mi.no_symbols){
            this_mi.list[this_mi.no_symbols + i] = right_mi.list[i];
            i++;
        }
        this_mi.no_symbols = this_mi.no_symbols + right_mi.no_symbols;
    }
    if(node.left != NULL){
        left_mi = recursive_codeword_length(*(node.left), level + 1);
        this_mi.list = realloc(this_mi.list, sizeof(struct symbol_length_pair) * (this_mi.no_symbols + left_mi.no_symbols));
        i = 0;
        while (i < left_mi.no_symbols){
            this_mi.list[this_mi.no_symbols + i] = left_mi.list[i];
            i++;
        }
        this_mi.no_symbols = this_mi.no_symbols + left_mi.no_symbols;
    }
    return this_mi;
}

struct model_input create_model_input(struct huffman_tree_node root){
    struct model_input model_input;
    model_input = recursive_codeword_length(root, 0);
    qsort(model_input.list, model_input.no_symbols, sizeof(struct symbol_length_pair), compare_symbol_length);
    return model_input;
}
struct model create_model(struct model_input model_input){
    int i, j, k, next_base, L_minus_l_minus_1, lj_limit;
    int no_symbols;
    int length_max;
    struct model model;
    no_symbols = model_input.no_symbols;
    model.no_symbols = no_symbols;
    model.symbols = (int *) malloc(sizeof(int) * no_symbols);
    i = 0;
    while(i < no_symbols){
        model.symbols[i] = model_input.list[i].symbol;
        i++;
    }
    length_max = model_input.list[no_symbols - 1].length;
    model.length_max = length_max;
    model.no_words = (int *) malloc(sizeof(int) * (length_max + 1));
    model.base_l = (int *) malloc(sizeof(int) * (length_max + 1));
    model.offset_l = (int *) malloc(sizeof(int) * (length_max + 1));
    model.lj_limit = (int *) malloc(sizeof(int) * (length_max + 1));
    i = 0;
    j = 0;
    while(i < length_max){
        k = 0;
        model.offset_l[i] = j + 1;
        while(model_input.list[j].length == (i + 1)){
            k++;
            j++;
        }
        model.no_words[i] = k;
        if(i == 0){
            model.base_l[i] = 0;
        }else{
            model.base_l[i] = 2 * (model.base_l[i - 1] + model.no_words[i - 1]);
        }
        if ((i + 1) == length_max){
            model.lj_limit[i] = pow(2, length_max);
        }else{
            next_base = 2 * (model.base_l[i] + model.no_words[i]);
            L_minus_l_minus_1 = length_max - (i+1) - 1;
            lj_limit = (next_base * pow(2, L_minus_l_minus_1));
            model.lj_limit[i] = lj_limit;
        }
        i++;
    }
    model.no_words[length_max] = 0;
    model.base_l[length_max] = 2 * (model.base_l[length_max - 1] + model.no_words[length_max - 1]);
    model.offset_l[length_max] = j + 1;
    model.lj_limit[length_max] = 0;
    return model;
}

void print_model(struct model model){
    int i;
    i = 0;
    printf("=======================\n");
    printf("%10s | %10s\n", "Offset", "Symbol");
    printf("=======================\n");
    while(i < model.no_symbols){
        printf("%10d | %10d\n", i + 1, model.symbols[i]);
        i++;
    }
    printf("=======================\n");
    i = 0;
    printf("=======================\n");
    printf("%10s | %10s | %10s | %10s | %10s\n", "l", "wl", "base", "offset", "lj_limit");
    printf("=======================\n");
    while(i <= model.length_max){
        printf("%10d | %10d | %10d | %10d | %10d\n",i + 1, model.no_words[i], model.base_l[i], model.offset_l[i], model.lj_limit[i]);
        i++;
    }
    printf("=======================\n");
}
