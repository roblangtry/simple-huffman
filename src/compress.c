#include "compress.h"
int run_compression(char *input_filename, char *output_filename, int verbose_flag){
    FILE *input_file_pointer;
    FILE *output_file_pointer;
    int value;
    struct probability_list list;
    input_file_pointer = fopen(input_filename, "r");
    output_file_pointer = fopen(output_filename, "w");
    while (fscanf(input_file_pointer, "%d\n", &value) != EOF) {
        fprintf(output_file_pointer, "%d\n", value);
    }

    list = evaluate_symbol_probabilities(input_file_pointer);
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
