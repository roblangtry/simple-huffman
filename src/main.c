#include "main.h"
int main ( int argc, char *argv[] ){
    int state = parse_commandline_args(argc, argv);
    int verbose = 0;
    if (state == -1){
        print_bad_input_message(argv[0]);
        return -1;
    }
    if (state >= 2){
        verbose = 1;
        state = state - 2;
    }
    if (state == 0){
        return run_compression(argv[2 + verbose], argv[3 + verbose], verbose);
    } else{
        return run_decompression(argv[2 + verbose], argv[3 + verbose], verbose);
    }
}
int parse_commandline_args(int argc, char *argv[]){
    int state = -1;
    if (argc >= 4){
        //verbose hasnt been specified so should only be 3 inputs
        if (strcmp("-c", argv[1]) == 0 || strcmp("--compress", argv[1]) == 0){
            state = 0;
        } else if (strcmp("-d", argv[1]) == 0 || strcmp("--decompress", argv[1]) == 0){
            state = 1;
        }
    }
    if (argc == 5){
        //verbose hasnt been specified so should only be 3 inputs
        if (strcmp("-v", argv[2]) == 0 || strcmp("--verbose", argv[2]) == 0){
            state = state + 2;
        } else{
            state = -1;
        }
    }
    return state;
}
void print_bad_input_message(char *message){
    printf("Error: '%s' input incorrectly formatted!\n", message);
    printf("For compression format as such:\n");
    printf("   %s -c [--verbose|-v] <infile> <outfile>\n", message);
    printf("   %s --compress [--verbose|-v] <infile> <outfile>\n", message);
    printf("For decompression format as such:\n");
    printf("   %s -d [--verbose|-v] <infile> <outfile>\n", message);
    printf("   %s -decompress [--verbose|-v] <infile> <outfile>\n", message);
}