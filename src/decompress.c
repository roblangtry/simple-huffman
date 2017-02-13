#include "decompress.h"
int run_decompression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    struct model_input model_input;
    struct model model;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    model_input = read_model_input_from_file(input_file_pointer);
    if (verbose_flag == 1)
        print_model_input(model_input);
    model = create_model(model_input);
    if (verbose_flag == 1)
        print_model(model);
    write_decompressed_file(input_file_pointer, output_file_pointer, model);
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}

void write_decompressed_file(FILE * input_file_pointer, FILE * output_file_pointer, struct model model){
    int value;
    int i, m, c, x;
    int V, L;
    struct bitlevel_file_pointer * bitlevel_file_pointer;
    bitlevel_file_pointer = get_bitlevel_file_pointer(input_file_pointer);
    struct bitlevel_object buffer;
    m = 0;
    L = model.length_max;
    buffer = bitlevel_read(bitlevel_file_pointer, L);
    V = buffer.value;
    while (m < model.message_length){
        i = 0;
        while(V >= model.lj_limit[i]){
            i++;
        }
        c = V >> (L - (i + 1));
        V = V - (c << (L - (i + 1)));
        x = c - model.base_l[i] + model.offset_l[i];
        fprintf(output_file_pointer, "%d\n", model.symbols[x-1]);
        buffer = bitlevel_read(bitlevel_file_pointer, i + 1);
        V = (V << (i + 1)) + buffer.value;
        m++;
    }
}