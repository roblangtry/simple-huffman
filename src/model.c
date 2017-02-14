#include "model.h"


struct model_input recursive_codeword_length(struct huffman_tree_node node, int level)
{
    struct model_input this_mi, left_mi, right_mi;
    struct symbol_length_pair sl_pair;
    int i;
    sl_pair.symbol = node.value;
    sl_pair.length = level;
    if(node.right == NULL && node.left == NULL){
        this_mi.no_symbols = 1;
        this_mi.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair));
        this_mi.list[0] = sl_pair;
    } else {
        this_mi.no_symbols = 0;
        this_mi.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair));
    }
    if(node.right != NULL){
        right_mi = recursive_codeword_length(*(node.right), level + 1);
        this_mi.list = realloc(this_mi.list, sizeof(struct symbol_length_pair) * (this_mi.no_symbols + right_mi.no_symbols));
        i = 0;
        while (i < right_mi.no_symbols){
            this_mi.list[this_mi.no_symbols + i] = right_mi.list[i];
            i++;
        }
        this_mi.no_symbols = this_mi.no_symbols + right_mi.no_symbols;
    }
    if(node.left != NULL){
        left_mi = recursive_codeword_length(*(node.left), level + 1);
        this_mi.list = realloc(this_mi.list, sizeof(struct symbol_length_pair) * (this_mi.no_symbols + left_mi.no_symbols));
        i = 0;
        while (i < left_mi.no_symbols){
            this_mi.list[this_mi.no_symbols + i] = left_mi.list[i];
            i++;
        }
        this_mi.no_symbols = this_mi.no_symbols + left_mi.no_symbols;
    }
    return this_mi;
}

struct model_input create_model_input(struct huffman_tree_node root){
    struct model_input model_input;
    model_input = recursive_codeword_length(root, 0);
    model_input.message_length = root.frequency;
    qsort(model_input.list, model_input.no_symbols, sizeof(struct symbol_length_pair), compare_symbol_length);
    return model_input;
}


struct model create_model(struct model_input model_input){
    int i, j, k, next_base, L_minus_l_minus_1, lj_limit;
    int no_symbols;
    int length_max;
    struct model model;
    no_symbols = model_input.no_symbols;
    model.message_length = model_input.message_length;
    model.no_symbols = no_symbols;
    model.symbols = (int *) malloc(sizeof(int) * no_symbols);
    i = 0;
    while(i < no_symbols){
        model.symbols[i] = model_input.list[i].symbol;
        i++;
    }
    length_max = model_input.list[no_symbols - 1].length;
    model.length_max = length_max;
    model.no_words = (int *) malloc(sizeof(int) * (length_max + 1));
    model.base_l = (int *) malloc(sizeof(int) * (length_max + 1));
    model.offset_l = (int *) malloc(sizeof(int) * (length_max + 1));
    model.lj_limit = (int *) malloc(sizeof(int) * (length_max + 1));
    i = 0;
    j = 0;
    while(i < length_max){
        k = 0;
        model.offset_l[i] = j + 1;
        while(model_input.list[j].length == (i + 1)){
            k++;
            j++;
        }
        model.no_words[i] = k;
        if(i == 0){
            model.base_l[i] = 0;
        }else{
            model.base_l[i] = 2 * (model.base_l[i - 1] + model.no_words[i - 1]);
        }
        if ((i + 1) == length_max){
            model.lj_limit[i] = pow(2, length_max);
        }else{
            next_base = 2 * (model.base_l[i] + model.no_words[i]);
            L_minus_l_minus_1 = length_max - (i+1) - 1;
            lj_limit = (next_base * pow(2, L_minus_l_minus_1));
            model.lj_limit[i] = lj_limit;
        }
        i++;
    }
    model.no_words[length_max] = 0;
    model.base_l[length_max] = 2 * (model.base_l[length_max - 1] + model.no_words[length_max - 1]);
    model.offset_l[length_max] = j + 1;
    model.lj_limit[length_max] = 0;
    return model;
}

void print_model(struct model model){
    int i;
    i = 0;
    printf("=======================\n");
    printf("%10s | %10s\n", "Offset", "Symbol");
    printf("=======================\n");
    while(i < model.no_symbols){
        printf("%10d | %10d\n", i + 1, model.symbols[i]);
        i++;
    }
    printf("=======================\n");
    i = 0;
    printf("=======================\n");
    printf("%10s | %10s | %10s | %10s | %10s\n", "l", "wl", "base", "offset", "lj_limit");
    printf("=======================\n");
    while(i <= model.length_max){
        printf("%10d | %10d | %10d | %10d | %10d\n",i + 1, model.no_words[i], model.base_l[i], model.offset_l[i], model.lj_limit[i]);
        i++;
    }
    printf("=======================\n");
}


void print_model_input(struct model_input model_input){
    int i;
    printf("=======================\n");
    printf("no_symbols -> %d\n", model_input.no_symbols);
    printf("=======================\n");
    printf("%10s | %10s\n", "Symbol", "Length");
    printf("=======================\n");
    i = 0;
    while(i < model_input.no_symbols){
        printf("%10d | %10d\n", model_input.list[i].symbol, model_input.list[i].length);
        i++;
    }
    printf("=======================\n");
}

void write_model_input_to_file(struct model_input model_input, FILE * output_file_pointer){
    int no_symbols;
    int message_length;
    int i;
    struct symbol_length_pair sl_pair;
    message_length = model_input.message_length;
    fwrite(&message_length, sizeof(int), 1, output_file_pointer);
    no_symbols = model_input.no_symbols;
    fwrite(&no_symbols, sizeof(int), 1, output_file_pointer);
    i = 0;
    while(i < no_symbols){
        sl_pair = model_input.list[i];
        fwrite(&(sl_pair.symbol), sizeof(int), 1, output_file_pointer);
        fwrite(&(sl_pair.length), sizeof(int), 1, output_file_pointer);
        i++;
    }
    fflush(output_file_pointer);
}
struct model_input read_model_input_from_file(FILE * input_file_ptr){
    struct model_input model_input;
    int temp;
    int i;
    struct symbol_length_pair sl_pair;
    fread(&temp, sizeof(int), 1, input_file_ptr);
    model_input.message_length = temp;
    fread(&temp, sizeof(int), 1, input_file_ptr);
    model_input.no_symbols = temp;
    model_input.list = (struct symbol_length_pair *) malloc(sizeof(struct symbol_length_pair) * temp);
    i = 0;
    while(i < model_input.no_symbols){
        fread(&temp, sizeof(int), 1, input_file_ptr);
        sl_pair.symbol = temp;
        fread(&temp, sizeof(int), 1, input_file_ptr);
        sl_pair.length = temp;
        model_input.list[i] = sl_pair;
        i++;
    }
    return model_input;
}