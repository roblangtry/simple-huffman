#include "decompress.h"
int run_decompression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    struct model_input model_input;
    struct model model;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    // Read the model_input data from the file
    model_input = read_model_input_from_file(input_file_pointer);
    if (verbose_flag == 1)
        print_model_input(model_input);
    // Use the model_input data to create the model same as the encoder
    model = create_model(model_input);
    if (verbose_flag == 1)
        print_model(model);
    // Use the model to read the input file and create the decompressed output file
    write_decompressed_file(input_file_pointer, output_file_pointer, model);
    free_model(model);
    free_model_input(model_input);
    // Close the file pointers
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}

void write_decompressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model){
    int length, no_symbols_processed, c, symbol_offset;
    int value, max_length;
    struct bitlevel_file_pointer * bitlevel_file_pointer;
    // Create the bitlevel file pointer
    bitlevel_file_pointer = get_bitlevel_file_pointer(input_file_pointer);
    struct bitlevel_object buffer;
    no_symbols_processed = 0;
    max_length = model.length_max;
    // Read in L (maximum length codeword) bits from the file
    buffer = bitlevel_read(bitlevel_file_pointer, max_length);
    value = buffer.value;
    // While there are still compressed symbols to be read
    while (no_symbols_processed < model.message_length){
        // Set the length to zero then iteratively test it against the lj_limit
        // to see if that is the length of the given codeword (length is actually
        // one more than that given here but this is easier to use for memory offsets)
        length = 0;
        while(value >= model.lj_limit[length]){
            length++;
        }
        // Derive the value of the current value
        c = value >> (max_length - (length + 1));
        value = value - (c << (max_length - (length + 1)));
        symbol_offset = c - model.base_l[length] + model.offset_l[length];
        // Write out that value to the output file
        fprintf(output_file_pointer, "%d\n", model.symbols[symbol_offset-1]);
        // Read into a buffer however many bits were digested in this iteration
        // of the decompression
        buffer = bitlevel_read(bitlevel_file_pointer, length + 1);
        // update the value with the new intake bits from the buffer
        value = (value << (length + 1)) + buffer.value;
        // Increment the amount of symbols processed
        no_symbols_processed++;
    }
    free(bitlevel_file_pointer);
}