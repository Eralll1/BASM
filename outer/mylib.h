#ifndef MYLIB
#define MYLIB

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

#define max(a,b) ((a) > (b) ? (a) : (b))

int myrand(int min, int max);
size_t ceil_to_power_of_two(size_t size);
void print_bin(FILE* output, unsigned int x);
int atoi_int(char* start);


size_t lfind (char* str, char symb);
int is_letter (char symb);
int is_number (char symb);
int is_all_numbers (char* str, char stop);
int is_good(char letter);
size_t count_symb(char* str, char symb);
void repl_symb(char* str, size_t length, char old, char new_);
int is_all(char* str, char symb);

#endif