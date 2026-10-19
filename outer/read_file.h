#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

typedef struct{
    char* start;
    char* end;
} string;

typedef enum {
    FILE_OK = 0,
    FILE_BAD_NAME,
    // FILE_BAD_CONTENT,
} FileState;

typedef struct  {
    size_t text_length;
    size_t line_amount;
    char* text_shifted;
    char* text_real;
    int state;
    string* pointers;
} parsedFile;

parsedFile parseFile(const char* filename);

void read_file(parsedFile* file, const char *filename);
void set_file_size(parsedFile* file, const char* filename);
void prepare_lines(parsedFile* file);

void set_both_pointers(parsedFile* file);
void set_st_pointers(parsedFile* file);
void set_end_pointers(parsedFile* file); 

void destroy_parsed_file(parsedFile* file);