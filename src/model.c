#include "model.h"


struct model_input recursive_codeword_length(struct huffman_tree_node node, uint64_t level)
{
    /* A recursive function to map the depth of the huffman tree (as lengths) */
    struct model_input this_mi, left_mi, right_mi;
    struct symbol_length_pair sl_pair;
    sl_pair.symbol = node.value;
    sl_pair.length = level;
    if(node.right == NULL && node.left == NULL){
        /*this is a leaf node so set it to length 1 and add the symbol length pair */
        this_mi.no_symbols = 1;
        this_mi.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair));
        this_mi.list[0] = sl_pair;
    } else if (node.right == NULL){
        return recursive_codeword_length(*(node.right), level + 1);
    } else if (node.left == NULL){
        return recursive_codeword_length(*(node.left), level + 1);
    } else{
        right_mi = recursive_codeword_length(*(node.right), level + 1);
        left_mi = recursive_codeword_length(*(node.left), level + 1);
        this_mi.no_symbols = right_mi.no_symbols + left_mi.no_symbols;
        this_mi.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair) * this_mi.no_symbols);
        memcpy(this_mi.list, right_mi.list, sizeof(struct symbol_length_pair) * right_mi.no_symbols);
        memcpy(&(this_mi.list[right_mi.no_symbols]), left_mi.list, sizeof(struct symbol_length_pair) * left_mi.no_symbols);
        free(right_mi.list);
        free(left_mi.list);
    }
    return this_mi;
}

struct model_input create_model_input(struct huffman_tree_node root){
    /* Create the model input from the huffman tree */
    struct model_input model_input;
    /* Recursively analyse the tree to get the codeword lengths for symbols */
    model_input = recursive_codeword_length(root, 0);
    /* The frequency of the root node is the total number of symbols in the file */
    model_input.message_length = root.frequency;
    /* Sort the model_input list so it has highest frequency elements at the 'top'(0 offset) */
    qsort(model_input.list, model_input.no_symbols, sizeof(struct symbol_length_pair), compare_symbol_length);
    return model_input;
}
uint64_t compare_symbol_offset (const void * a, const void * b)
{
    /* Compare the symbol offset for qsort */
    uint64_t value_a, value_b;
    value_a = (*(struct symbol_offset*)a).symbol;
    value_b = (*(struct symbol_offset*)b).symbol;
    return (value_a - value_b);
}
void handle_symbol_data_for_model(struct model_input model_input, struct model * model_ptr){
    uint64_t i = 0;
    /* Allocate enough memory for the binary search table and symbols */
    model_ptr->binary_search_table = malloc(sizeof(size_t) * MAX_SYMBOL);
    model_ptr->symbols = (uint32_t *) malloc(sizeof(uint64_t) * model_input.no_symbols);
    /* While the index is less than the number of symbols */
    while(i < model_input.no_symbols){
        /* Add the symbol to the model */
        model_ptr->symbols[i] = model_input.list[i].symbol;
        /* Add that symbol offset to the binary search table */
        model_ptr->binary_search_table[model_ptr->symbols[i]] = i;
        i++;
    }
}
uint64_t find_symbol_offset(uint64_t symbol, struct model model){
    
    return model.binary_search_table[symbol];
}

struct model create_model(struct model_input model_input){
    uint64_t length, next_base, L_minus_l_minus_1, lj_limit;
    uint64_t no_symbols;
    uint64_t no_words;
    uint64_t current_offset;
    uint64_t length_max;
    struct model model;
    no_symbols = model_input.no_symbols;
    model.message_length = model_input.message_length;
    model.no_symbols = no_symbols;
    /* Add the symbols and the binary search table to the model */
    handle_symbol_data_for_model(model_input, &model);
    length_max = model_input.list[no_symbols - 1].length;
    /* Add the length to the model and allocate the memory for the */
    /* w_l, Base_l, Offset_l and lj_limit values */
    model.length_max = length_max;
    model.no_words = (uint64_t *) malloc(sizeof(uint64_t) * (length_max + 1));
    model.base_l = (uint64_t *) malloc(sizeof(uint64_t) * (length_max + 1));
    model.offset_l = (uint64_t *) malloc(sizeof(uint64_t) * (length_max + 1));
    model.lj_limit = (uint64_t *) malloc(sizeof(uint64_t) * (length_max + 1));
    length = 0;
    current_offset = 0;
    /* For each length work out the values */
    while(length < length_max){
        no_words = 0;
        model.offset_l[length] = current_offset + 1;
        /* While there are more values of this length add to current_offset and no_words */
        while(current_offset < model_input.no_symbols && model_input.list[current_offset].length == (length + 1)){
            no_words++;
            current_offset++;
        }
        /* Add the no_words to this level of the model */
        model.no_words[length] = no_words;
        /* Calculate the Base_l */
        if(length == 0){
            model.base_l[length] = 0;
        }else{
            model.base_l[length] = 2 * (model.base_l[length - 1] + model.no_words[length - 1]);
        }
        /* Calculate the lj_limit */
        if ((length + 1) == length_max){
            model.lj_limit[length] = pow(2, length_max);
        }else{
            next_base = 2 * (model.base_l[length] + model.no_words[length]);
            L_minus_l_minus_1 = length_max - (length + 1) - 1;
            lj_limit = (next_base * pow(2, L_minus_l_minus_1));
            model.lj_limit[length] = lj_limit;
        }
        length++;
    }
    /* Values for the final level of the model (ie 1 beyond the largest codeword length) */
    model.no_words[length_max] = 0;
    model.base_l[length_max] = 2 * (model.base_l[length_max - 1] + model.no_words[length_max - 1]);
    model.offset_l[length_max] = current_offset + 1;
    model.lj_limit[length_max] = 0;
    return model;
}

void print_model(struct model model){
    /* print the model for debugging */
    uint64_t i;
    i = 0;
    printf("=======================\n");
    printf("%10s | %10s\n", "Offset", "Symbol");
    printf("=======================\n");
    while(i < model.no_symbols){
        printf("%10lu | %10u\n", i + 1, model.symbols[i]);
        i++;
    }
    printf("=======================\n");
    i = 0;
    printf("=======================\n");
    printf("%10s | %10s | %10s | %10s | %10s\n", "l", "wl", "base", "offset", "lj_limit");
    printf("=======================\n");
    while(i <= model.length_max){
        printf("%10lu | %10lu | %10lu | %10lu | %10lu\n",i + 1, model.no_words[i], model.base_l[i], model.offset_l[i], model.lj_limit[i]);
        i++;
    }
    printf("=======================\n");
}


void print_model_input(struct model_input model_input){
    /* print the model for debugging */
    uint64_t i;
    printf("=======================\n");
    printf("no_symbols -> %lu\n", model_input.no_symbols);
    printf("=======================\n");
    printf("%10s | %10s\n", "Symbol", "Length");
    printf("=======================\n");
    i = 0;
    while(i < model_input.no_symbols){
        printf("%10lu | %10lu\n", model_input.list[i].symbol, model_input.list[i].length);
        i++;
    }
    printf("=======================\n");
}

void write_model_input_to_file(struct model_input model_input, FILE * output_file_pointer){
    /* Write the model_input to the file so the decodeder can read it */
    uint64_t no_symbols;
    uint64_t message_length;
    uint64_t i;
    struct symbol_length_pair sl_pair;
    struct bitlevel_file_pointer * bl_file_output_ptr;
    bl_file_output_ptr = get_bitlevel_file_pointer(output_file_pointer);
    /* Get and write out the message length and no. symbols first so that the decoder knows how large the  */
    /* model input is and can accurately read it */
    message_length = model_input.message_length;
    write_elias_value(bl_file_output_ptr, message_length);
    no_symbols = model_input.no_symbols;
    write_elias_value(bl_file_output_ptr, no_symbols);
    i = 0;
    /* While there are still symbols left write out their value and length */
    while(i < no_symbols){
        sl_pair = model_input.list[i];
        write_elias_value(bl_file_output_ptr, sl_pair.symbol);
        write_elias_value(bl_file_output_ptr, sl_pair.length);
        i++;
    }
    bitlevel_flush(bl_file_output_ptr);
    free(bl_file_output_ptr);
}
struct model_input read_model_input_from_file(FILE * input_file_ptr){
    /* Read the model_input from the file as set by the encoder */
    struct model_input model_input;
    uint64_t temp;
    uint64_t i;
    struct symbol_length_pair sl_pair;
    struct bitlevel_file_pointer * bl_file_input_ptr;
    bl_file_input_ptr = get_unbuffered_bitlevel_file_pointer(input_file_ptr);
    temp = read_elias_value(bl_file_input_ptr);
    /* Get the message length and no. symbols first so that the decoder knows how large the  */
    /* model input is and can accurately read it */
    model_input.message_length = temp;
    temp = read_elias_value(bl_file_input_ptr);
    model_input.no_symbols = temp;
    model_input.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair) * temp);
    i = 0;
    /* While there are still symbols left read in their value and length */
    while(i < model_input.no_symbols){
        temp = read_elias_value(bl_file_input_ptr);
        sl_pair.symbol = temp;
        temp = read_elias_value(bl_file_input_ptr);
        sl_pair.length = temp;
        model_input.list[i] = sl_pair;
        i++;
    }
    free(bl_file_input_ptr);
    return model_input;
}


void free_model(struct model model){
    /* Free all the components of the model */
    free(model.no_words);
    free(model.base_l);
    free(model.offset_l);
    free(model.lj_limit);
    free(model.binary_search_table);
    free(model.symbols);
}
void free_model_input(struct model_input model_input){
    /* Free the model input list */
    free(model_input.list);
}