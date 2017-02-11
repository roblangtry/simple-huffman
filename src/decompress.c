#include "decompress.h"
int run_decompression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    int value;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        fprintf(output_file_pointer, "%d\n", value);
    }
    fclose(input_file_pointer);
    fclose(output_file_pointer);
    return 1;
}