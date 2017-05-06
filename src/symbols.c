#include "symbols.h"

uint64_t compare_symbol_length (const void * a, const void * b)
{
    // Compare 2 symbol lengths to be used for qsort
    uint64_t value_a, value_b;
    value_a = (*(struct probability_point64_t*)a).occurrences;
    value_b = (*(struct probability_point64_t*)b).occurrences;
    return (value_a - value_b);
}
struct probability_list evaluate_symbol_probabilities(FILE * input_file_pointer, uint64_t general){
    // Read the input file and add the symbol probabilities to the probability list
    uint32_t temp_value;
    unsigned char c_value[BUFFER_SIZE];
    uint32_t i_value[BUFFER_SIZE];
    struct probability_list list;
    struct probability_point64_t point64_t;
    uint64_t * val_list;
    uint64_t list_size, i, alloted;
    uint64_t nval;
    // Initialise_the probability list
    val_list = calloc(MAX_SYMBOL, sizeof(uint64_t));
    list = initialise_probabilities_list(input_file_pointer, general);
    // While there are symbols to read; read them.
    list_size = 0;
    if(general == 0){
        while (fscanf(input_file_pointer, "%d\n", &temp_value) != EOF) {
            // Add the read value to the proabability list
            //add_to_probability_list(&list, temp_value);
            nval = add_to_val_list(val_list, temp_value);
            if(nval > list_size){
                list_size = nval;
            }
        }
    } else if(general == 1){
        alloted = fread(c_value, sizeof(char), BUFFER_SIZE, input_file_pointer);
        while (alloted > 0) {
            i = 0;
            while(i < alloted){
                // Add the read value to the proabability list
                temp_value = c_value[i];
                //add_to_probability_list(&list, temp_value);
                nval = add_to_val_list(val_list, temp_value);
                if(nval > list_size){
                    list_size = nval;
                }
                i++;
            }
            alloted = fread(c_value, sizeof(char), BUFFER_SIZE, input_file_pointer);
        }
    } else{
        alloted = fread(i_value, sizeof(uint32_t), BUFFER_SIZE, input_file_pointer);
        while (alloted > 0) {
            i = 0;
            while(i < alloted){
                // Add the read value to the proabability list
                temp_value = i_value[i];
                //add_to_probability_list(&list, temp_value);
                nval = add_to_val_list(val_list, temp_value);
                if(nval > list_size){
                    list_size = nval;
                }
                i++;
            }
            alloted = fread(i_value, sizeof(uint32_t), BUFFER_SIZE, input_file_pointer);
        }
    }
    i = 0;

    while(i <= list_size){
        if(val_list[i] > 0){
            point64_t.value = i;
            point64_t.occurrences = val_list[i];
            add_to_probability_list(&list, point64_t);
        }
        i++;
    }
    return list;
}
void CHECK_SYMBOL_RANGE(uint32_t value){
    if ((value < 1) || (value > MAX_SYMBOL)) {
        fprintf(stderr, "Symbol %d is out of range.\n", value);
        exit(-1);
    }
}

uint64_t add_to_val_list(uint64_t * list, uint32_t value){
    CHECK_SYMBOL_RANGE(value);
    list[value]++;
    return value;
}
struct probability_list initialise_probabilities_list(FILE * input_file_pointer, uint64_t general){
    uint64_t value;
    unsigned char c_value;
    struct probability_list list;
    struct probability_point64_t point64_t;
    // Setup the list assigning enough memory for first element
    list.list_length = 0;
    list.list = malloc(sizeof(struct probability_point64_t));
    return list;
}
void add_to_probability_list(struct probability_list * list, struct probability_point64_t point64_t){
    // Add the given value to the probability list (using pointer for assignment)
    // Add 1 to the list length
    (*list).list_length = (*list).list_length + 1;
    // Allocate more memory to the list
    list->list = realloc(list->list, sizeof(struct probability_point64_t) * list->list_length);
    // Add the point64_t the list
    (*list).list[(*list).list_length - 1] = point64_t;
}

uint64_t compare_probability (const void * a, const void * b)
{
    // Compare 2 probability point64_ts to be used for qsort
    uint64_t value_a, value_b;
    value_a = (*(struct probability_point64_t*)a).occurrences;
    value_b = (*(struct probability_point64_t*)b).occurrences;
    return (value_b - value_a);
}


void sort_symbol_probabilities(struct probability_list * list){
    // Sort the probability list
   qsort((*list).list, (*list).list_length, sizeof(struct probability_point64_t), compare_probability);
}



void print_symbol_frequencies(struct probability_list list){
    // print the symbol frequencies of the probability list
    uint64_t symbol, freq, index;
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
