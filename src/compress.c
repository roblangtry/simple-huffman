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

    rewind(input_file_pointer);
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        fprintf(output_file_pointer, "%d\n", value);
    }
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}