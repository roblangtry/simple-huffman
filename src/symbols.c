#include "symbols.h"

int compare_symbol_length (const void * a, const void * b)
{
    int value_a, value_b;
    value_a = (*(struct probability_point*)a).occurrences;
    value_b = (*(struct probability_point*)b).occurrences;
    return (value_a - value_b);
}
struct probability_list evaluate_symbol_probabilities(FILE * input_file_pointer){
    int temp_value;
    struct probability_list list;
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


void sort_symbol_probabilities(struct probability_list * list){
   qsort((*list).list, (*list).list_length, sizeof(struct probability_point), compare_probability);
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
