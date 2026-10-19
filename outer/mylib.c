#include "mylib.h"

int myrand(int min, int max){
    return rand() % (max - min + 1) + min;
}

size_t ceil_to_power_of_two(size_t size){
    size_t res = 1;
    while (res <= size){
        res <<= 1;
    }
    return res;
}

void print_bin(FILE* output, unsigned int x) {
    for (int i = (int)(sizeof x * CHAR_BIT) - 1; i >= 0; i--)
        fprintf(output, "%c", (x >> i) & 1u ? '1' : '0');
}

int atoi_int(char* start){
    int res = 0;
    while (start[0] >= '0' && start[0] <= '9'){
        res = res*10 + start[0] - '0';
        start++;
    }
    return res;
}

size_t lfind (char* str, char symb) {
    size_t ind = 0;
    while (str[ind] != symb && str[ind] != '\0'){
        ind++;
    }
    if (str[ind] != symb) {
        return -1;
    } else {
        return ind;
    }
}

int is_letter (char symb) {
    if (('A' <= symb && symb <= 'Z') || ('a' <= symb && symb <= 'z'))
        return 1;
    return 0;
}

int is_number (char symb) {
    if ('0' <= symb && symb <= '9')
        return 1;
    return 0;
}

int is_all_numbers (char* str, char stop) {
    size_t ind = 0;
    while (str[ind] != '\0' && str[ind] != stop) {
        if (str[ind] < '0' || str[ind] > '9'){
            return 0;
        }
        ind++;
    }
    return 1;
}

int is_good(char letter){
    return (letter == '\0') || ('z' >= letter && letter >= 'a') || ('Z' >= letter && letter >= 'A');
}

size_t count_symb(char* str, char symb) {
    size_t amount = 0;
    for (size_t ind = 0; str[ind] != '\0'; ind++)
        if (str[ind] == symb)
            amount++;
    return amount;
}

void repl_symb(char* str, size_t length, char old, char new_) {
    for (size_t ind = 0; ind < length; ind++)
        if(str[ind] == old)
            str[ind] = new_;
}

int is_all(char* str, char symb){
    size_t ind = 0;
    while (str[ind] != '\0') {
        if (str[ind] != symb){
            return 0;
        }
        ind++;
    }
    return 1;
}