#include "symbols.h"

int compare_symbol_length (const void * a, const void * b)
{
    // Compare 2 symbol lengths to be used for qsort
    int value_a, value_b;
    value_a = (*(struct probability_point*)a).occurrences;
    value_b = (*(struct probability_point*)b).occurrences;
    return (value_a - value_b);
}
struct probability_list evaluate_symbol_probabilities(FILE * input_file_pointer, int general){
    // Read the input file and add the symbol probabilities to the probability list
    int temp_value;
    unsigned char c_value[2000];
    unsigned int i_value[2000];
    struct probability_list list;
    struct probability_point point;
    int * val_list;
    int list_size, i, alloted;
    // Initialise_the probability list
    val_list = malloc(sizeof(int));
    list = initialise_probabilities_list(input_file_pointer, general);
    // While there are symbols to read; read them.
    list_size = 0;
    if(general == 0){
        while (fscanf(input_file_pointer, "%d\n", &temp_value) != EOF) {
            // Add the read value to the proabability list
            //add_to_probability_list(&list, temp_value);
            val_list = add_to_val_list(val_list, &list_size, temp_value);
        }
    } else if(general == 1){
        alloted = fread(c_value, sizeof(char), 2000, input_file_pointer);
        while (alloted > 0) {
            i = 0;
            while(i < alloted){
                // Add the read value to the proabability list
                temp_value = c_value[i];
                //add_to_probability_list(&list, temp_value);
                val_list = add_to_val_list(val_list, &list_size, temp_value);
                i++;
            }
            alloted = fread(c_value, sizeof(char), 2000, input_file_pointer);
        }
    } else{
        alloted = fread(i_value, sizeof(unsigned int), 2000, input_file_pointer);
        while (alloted > 0) {
            i = 0;
            while(i < alloted){
                // Add the read value to the proabability list
                temp_value = i_value[i];
                //add_to_probability_list(&list, temp_value);
                val_list = add_to_val_list(val_list, &list_size, temp_value);
                i++;
            }
            alloted = fread(i_value, sizeof(unsigned int), 2000, input_file_pointer);
        }
    }
    i = 0;
    while(i < list_size){
        if(val_list[i] > 0){
            point.value = i;
            point.occurrences = val_list[i];
            add_to_probability_list(&list, point);
        }
        i++;
    }
    return list;
}
int * add_to_val_list(int * list, int * size, int value){
    int i;
    if(value >= *size){
        i = *size;
        *size = 10 * value + 1;
        list = realloc(list, sizeof(int) * (*size));
        while(i<*size){
            list[i] = 0;
            i++;
        }
        list[value]++;
    } else{
        list[value]++;
    }
    return list;
}
struct probability_list initialise_probabilities_list(FILE * input_file_pointer, int general){
    int value;
    unsigned char c_value;
    struct probability_list list;
    struct probability_point point;
    // Setup the list assigning enough memory for first element
    list.list_length = 0;
    list.list = malloc(sizeof(struct probability_point));
    return list;
}
void add_to_probability_list(struct probability_list * list, struct probability_point point){
    // Add the given value to the probability list (using pointer for assignment)
    // Add 1 to the list length
    (*list).list_length = (*list).list_length + 1;
    // Allocate more memory to the list
    list->list = realloc(list->list, sizeof(struct probability_point) * list->list_length);
    // Add the point the list
    (*list).list[(*list).list_length - 1] = point;
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
