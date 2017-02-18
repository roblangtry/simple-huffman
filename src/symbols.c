#include "symbols.h"

int compare_symbol_length (const void * a, const void * b)
{
    // Compare 2 symbol lengths to be used for qsort
    int value_a, value_b;
    value_a = (*(struct probability_point*)a).occurrences;
    value_b = (*(struct probability_point*)b).occurrences;
    return (value_a - value_b);
}
struct probability_list evaluate_symbol_probabilities(FILE * input_file_pointer){
    // Read the input file and add the symbol probabilities to the probability list
    int temp_value;
    struct probability_list list;
    // Initialise_the probability list
    list = initialise_probabilities_list(input_file_pointer);
    // While there are symbols to read; read them.
    while (fscanf(input_file_pointer, "%d\n", &temp_value) != EOF) {
        // Add the read value to the proabability list
        add_to_probability_list(&list, temp_value);
    }
    return list;
}
struct probability_list initialise_probabilities_list(FILE * input_file_pointer){
    int value;
    struct probability_list list;
    struct probability_point point;
    // Read in the first value of the list so we can assign it
    fscanf(input_file_pointer, "%d\n", &value);
    // Setup the probability point
    point.value = value;
    point.occurrences = 1;
    // Setup the list assigning enough memory for first element
    list.list_length = 1;
    list.list = malloc(sizeof(struct probability_point));
    list.list[0] = point;
    return list;
}
void add_to_probability_list(struct probability_list * list, int value){
    // Add the given value to the probability list (using pointer for assignment)
    int i;
    int list_len;
    int added;
    struct probability_point point;
    list_len = (*list).list_length;
    i = 0;
    added = 0;
    // check to see if the value exists in the list already
    while (i < list_len && added == 0){
        point = (*list).list[i];
        // If we have found the value add 1 to the number of times it has occurred
        if(point.value == value){
            point.occurrences = point.occurrences + 1;
            added = 1;
            (*list).list[i] = point;
        }
        i++;
    }
    // The value doesnt exist in the list so add it
    if(added == 0){
        // Add 1 to the list length
        (*list).list_length = (*list).list_length + 1;
        // Allocate more memory to the list
        (*list).list = realloc((*list).list, sizeof(struct probability_point) * (*list).list_length);
        // Setup the probability point
        point.value = value;
        point.occurrences = 1;
        // Add the point the list
        (*list).list[(*list).list_length - 1] = point;
    }
}

int compare_probability (const void * a, const void * b)
{
    // Compare 2 probability points to be used for qsort
    int value_a, value_b;
    value_a = (*(struct probability_point*)a).occurrences;
    value_b = (*(struct probability_point*)b).occurrences;
    return (value_b - value_a);
}


void sort_symbol_probabilities(struct probability_list * list){
    // Sort the probability list
   qsort((*list).list, (*list).list_length, sizeof(struct probability_point), compare_probability);
}



void print_symbol_frequencies(struct probability_list list){
    // Print the symbol frequencies of the probability list
    int symbol, freq, index;
    index = 0;
    printf("=======================\n");
    printf("%10s | %10s\n", "Symbol", "Frequency");
    printf("=======================\n");
    // While the index value being read is less than the list length
    while(list.list_length > index){
        // print the symbol and frequency
        symbol = list.list[index].value;
        freq = list.list[index].occurrences;
        printf("%10d | %10d\n", symbol, freq);
        index++;
    }
    printf("=======================\n");
}
void free_probability_list(struct probability_list list){
    // Free the list of the probability list
    free(list.list);
}
