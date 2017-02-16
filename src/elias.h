#include <stdlib.h>
#include "bitlevel_io.h"
#include <math.h>
#ifndef ELIAS_CODE
#define ELIAS_CODE
struct bitlevel_object calculate_elias_value(int value);
void write_elias_value(struct bitlevel_file_pointer * file_pointer, int value);
int read_elias_value(struct bitlevel_file_pointer * file_pointer);
#endif