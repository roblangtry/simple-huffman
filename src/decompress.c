#include "decompress.h"
int run_decompression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    int value, temp;
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
    while (fread(&value, sizeof(int), 1, input_file_pointer)){
        fprintf(output_file_pointer, "%d\n", value);
    }
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}