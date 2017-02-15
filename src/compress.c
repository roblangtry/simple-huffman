#include "compress.h"
int run_compression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
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
    write_model_input_to_file(model_input, output_file_pointer);
    rewind(input_file_pointer);
    write_compressed_file(input_file_pointer, output_file_pointer, model);
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}

void write_compressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model){
    int value;
    struct bitlevel_object write_object;
    struct bitlevel_file_pointer * bitlevel_file_pointer;
    bitlevel_file_pointer = get_bitlevel_file_pointer(output_file_pointer);
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        write_object = calculate_write_object(value, model);
        bitlevel_write(bitlevel_file_pointer, write_object);
    }
    bitlevel_flush(bitlevel_file_pointer);
}
struct bitlevel_object calculate_write_object(int value, struct model model){
    int offset;
    int i;
    int c;
    struct bitlevel_object object;
    offset = find_symbol_offset(value, model) + 1; // +1 because offsets start at 0 instead of 1
    i = 0;
    while(offset >= model.offset_l[i + 1]){
        i++;
    }
    c = offset - model.offset_l[i] + model.base_l[i];
    object.value = c;
    object.length = i + 1;
    return object;
}
