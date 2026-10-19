#include "helper.h"


int can_be_name(char symb){
    return is_letter(symb) 
        || is_number(symb) 
        || symb == '$' 
        || symb == '_' 
        || symb == '%' 
        || symb == '&'
        || symb == '"'
        || symb == '/'
        || symb == '-'
    ;
}

word txt2word(char* txt){
    word res = 0;

    char is_neg = (txt[0] == '-') ? 1 : 0;
    if (is_neg == 1) txt++;

    while (txt[0] >= '0' && txt[0] <= '9'){
        res = res*10 + txt[0] - '0';
        txt++;
    }

    if (is_neg) return res*(-1);
    else        return res; 
    
}

cmd_args parse_args(int argc, char* argv[]){

    cmd_args res = {};

    int opt = 0;

    while ((opt = getopt(argc, argv, "i:o:")) != -1) {
        switch (opt) {
            case 'i':
                res.input_filename = optarg;
                break;
            case 'o':
                res.output_filename = optarg;
                break;
            default:
                fprintf(stderr, "Bad command line arg '%s'", optarg);
                exit(1);
            
        }
    }

    return res;
}