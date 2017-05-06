#include "compress.h"
int run_compression(char *input_filename, char *output_filename, uint64_t verbose_flag, uint64_t general){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    struct probability_list list;
    struct huffman_root_holder huffman_root;
    struct model_input model_input;
    struct model model;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    // Work out the probabilities of the symbol within the file
    list = evaluate_symbol_probabilities(input_file_pointer, general);
    // Sort the list so that most frequent is at 0
    sort_symbol_probabilities(&list);;
    if (verbose_flag == 1)
        print_symbol_frequencies(list);
    // Create a huffman binary tree from the list
    huffman_root = create_huffman_tree(list);
    if (verbose_flag == 1)
        print_huffman_tree(huffman_root.root, 0);
    // Create the model_input struct to be transmitted between encoder and decoder
    // is used to create the model
    model_input = create_model_input(*huffman_root.root);
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
    write_compressed_file(input_file_pointer, output_file_pointer, model, general);
    // Free the memory of structs
    free_huffman_root(huffman_root);
    free_model(model);
    free_model_input(model_input);
    free_probability_list(list);
    // Close both file pointers
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}

void write_compressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model, uint64_t general){
    uint64_t value, i, alloted;
    unsigned char c_value[BUFFER_SIZE];
    uint32_t i_value[BUFFER_SIZE];
    struct bitlevel_object write_object;
    struct bitlevel_file_pointer * bitlevel_file_pointer;
    // Create the bitlevel file pointer from the output file pointer
    bitlevel_file_pointer = get_bitlevel_file_pointer(output_file_pointer);
    if(general == 0){
        // While their is input to read
        while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
            // Use the model to create a bitlevel object representing value to be written
            write_object = calculate_write_object(value, model);
            // Write the object
            bitlevel_write(bitlevel_file_pointer, write_object);
        }
    } else if(general == 1){
        alloted = fread(c_value, sizeof(char), BUFFER_SIZE, input_file_pointer);
        while (alloted > 0) {
            i = 0;
            while(i < alloted){
                value = c_value[i];
                // Use the model to create a bitlevel object representing value to be written
                write_object = calculate_write_object(value, model);
                // Write the object
                bitlevel_write(bitlevel_file_pointer, write_object);
                i++;
            }
            alloted = fread(c_value, sizeof(char), BUFFER_SIZE, input_file_pointer);
        }
    } else{
        alloted = fread(i_value, sizeof(uint32_t), BUFFER_SIZE, input_file_pointer);
        while (alloted > 0) {
            i = 0;
            while(i < alloted){
                value = i_value[i];
                // Use the model to create a bitlevel object representing value to be written
                write_object = calculate_write_object(value, model);
                // Write the object
                bitlevel_write(bitlevel_file_pointer, write_object);
                i++;
            }
            alloted = fread(i_value, sizeof(uint32_t), BUFFER_SIZE, input_file_pointer);
        }
    }
    // Flush the file pointer to ensure all info written
    bitlevel_flush(bitlevel_file_pointer);
    // Free the bitlevel_file_pointer
    free(bitlevel_file_pointer);
}
struct bitlevel_object calculate_write_object(uint64_t value, struct model model){
    uint64_t offset;
    uint64_t l;
    uint64_t c;
    struct bitlevel_object object;
    // Binary search for symbol offset value in model
    offset = find_symbol_offset(value, model) + 1; // +1 because offsets start at 0 instead of 1
    l = 0;
    // Workout the l value
    while(offset >= model.offset_l[l + 1]){
        l++;
    }
    // Get the encoded uint64_t value
    c = offset - model.offset_l[l] + model.base_l[l];
    // Setup the bitlevel_object
    object.value = c;
    object.length = l + 1;
    return object;
}
