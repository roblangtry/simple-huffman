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
    // Work out the probabilities of the symbol within the file
    list = evaluate_symbol_probabilities(input_file_pointer);
    // Sort the list so that most frequent is at 0
    sort_symbol_probabilities(&list);
    if (verbose_flag == 1)
        print_symbol_frequencies(list);
    // Create a huffman binary tree from the list
    huffman_root = create_huffman_tree(list);
    if (verbose_flag == 1)
        print_huffman_tree(huffman_root, 0);
    // Create the model_input struct to be transmitted between encoder and decoder
    // is used to create the model
    model_input = create_model_input(huffman_root);
    if (verbose_flag == 1)
        print_model_input(model_input);
    // Create the model from the model_input
    model = create_model(model_input);
    if (verbose_flag == 1)
        print_model(model);
    // Write out the model_input to file so the decoder can construct a model
    write_model_input_to_file(model_input, output_file_pointer);
    // Rewind the input file pointer so that we can read it again to write the compressed file
    rewind(input_file_pointer);
    // Read the input file and write the compressed file
    write_compressed_file(input_file_pointer, output_file_pointer, model);
    // Close both file pointers
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}

void write_compressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model){
    int value;
    struct bitlevel_object write_object;
    struct bitlevel_file_pointer * bitlevel_file_pointer;
    // Create the bitlevel file pointer from the output file pointer
    bitlevel_file_pointer = get_bitlevel_file_pointer(output_file_pointer);
    // While their is input to read
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        // Use the model to create a bitlevel object representing value to be written
        write_object = calculate_write_object(value, model);
        // Write the object
        bitlevel_write(bitlevel_file_pointer, write_object);
    }
    // Flush the file pointer to ensure all info written
    bitlevel_flush(bitlevel_file_pointer);
}
struct bitlevel_object calculate_write_object(int value, struct model model){
    int offset;
    int l;
    int c;
    struct bitlevel_object object;
    // Binary search for symbol offset value in model
    offset = find_symbol_offset(value, model) + 1; // +1 because offsets start at 0 instead of 1
    l = 0;
    // Workout the l value
    while(offset >= model.offset_l[l + 1]){
        l++;
    }
    // Get the encoded int value
    c = offset - model.offset_l[l] + model.base_l[l];
    // Setup the bitlevel_object
    object.value = c;
    object.length = l + 1;
    return object;
}
