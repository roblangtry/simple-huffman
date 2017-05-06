#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "compress.h"
#include "decompress.h"
#define BUFFER_SIZE 1048576
int main(uint64_t argc, char *argv[]);
uint64_t parse_commandline_args(uint64_t argc, char *argv[]);
void print64_t_bad_input_message(char *message);