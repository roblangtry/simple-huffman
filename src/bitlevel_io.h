#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "symbols.h"
#ifndef BITLEVEL_IO_CODE
#define BITLEVEL_IO_CODE
struct bitlevel_object {
    int value;
    int length;
};
struct bitlevel_file_pointer {
    FILE * file_pointer;
    int buffer;
    int current_length;
};
struct bitlevel_file_pointer * get_bitlevel_file_pointer(FILE * file_pointer);
size_t bitlevel_write(struct bitlevel_file_pointer * bitlevel_file_pointer, struct bitlevel_object write_object);
struct bitlevel_object bitlevel_read(struct bitlevel_file_pointer * bitlevel_file_pointer, int length);
size_t bitlevel_flush(struct bitlevel_file_pointer * bitlevel_file_pointer);
#endif