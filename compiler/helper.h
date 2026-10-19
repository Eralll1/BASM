#include "../globals/globals.h"
#include <stddef.h>
#include <getopt.h>

typedef struct {
    char* input_filename;
    char* output_filename;
} cmd_args;

int can_be_name(char symb);
word txt2word(char* txt);
cmd_args parse_args(int argc, char* argv[]);

