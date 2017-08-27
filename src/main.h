#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "compress.h"
#include "decompress.h"
#include "new.h"
#define BUFFER_SIZE 1048576
#define NEW_ENCODE 2000
#define NEW_DECODE 3000
int main(uint64_t argc, char *argv[]);
uint64_t parse_commandline_args(uint64_t argc, char *argv[]);
void print_bad_input_message(char *message);