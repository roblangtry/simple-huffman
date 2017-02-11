#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]);
int parse_commandline_args(int argc, char *argv[]);
void print_bad_input_message(char *message);
int run_compression(char *input_filename, char *output_filename, int verbose_flag);
int run_decompression(char *input_filename, char *output_filename, int verbose_flag);