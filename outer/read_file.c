#include "read_file.h"
#include "mylib.h"

parsedFile parseFile(const char* filename){
    parsedFile res = {};

    assert(filename != NULL);

    set_file_size(&res, filename);
    if (res.text_length == 0){
        res.state = FILE_BAD_NAME;
        return res;
    }

    read_file(&res, filename);
    prepare_lines(&res);
    set_both_pointers(&res);


    return res;
}

void destroy_parsed_file(parsedFile* file){
    free(file->text_real);
    free(file->pointers);
}

void set_file_size(parsedFile* file, const char* filename){

    struct stat st = {};

    // if filename is invalid the programm stops
    if (stat(filename, &st) != 0) {
        file->text_length = 0;
    }

    file->text_length = st.st_size;
}

void read_file(parsedFile* file, const char *filename){
    
    // add 2 for \0 at the start and at the beggining
    file->text_real = calloc(file->text_length + 2, sizeof(char));

    FILE* opened_file = fopen(filename, "rb"); // " as it requires char* as argument // fopen = open+buffer
    assert(opened_file != NULL); // file existence is checked
    size_t real_size = fread(
        file->text_real + 1,
        sizeof(char),
        file->text_length,
        opened_file
    );
    fclose(opened_file);

    if (real_size != file->text_length){
        printf("Fucking Windows user. Go fuck yourself\n");
        file->text_real = realloc(file->text_real, real_size + 2);  // add 2 for \0 at the start and at the beggining
    }

    file->text_real[real_size+1] = '\0'; // the final symbol is \0
    file->text_real[0] = '\0'; // the first symbol is \0

    file->text_shifted = file->text_real + 1;
    file->text_length = real_size;
}

void prepare_lines(parsedFile* file){

    file->line_amount = count_symb(file->text_shifted, '\n');
    file->line_amount++; // line_amount is 1 more than amount of \n

    repl_symb(
        file->text_shifted,
        file->text_length,
        '\n',
        '\0'
    );

}

void set_both_pointers(parsedFile* file) {

    file->pointers = calloc(file->line_amount, sizeof(string));
    set_st_pointers(file);
    set_end_pointers(file);
}

void set_st_pointers(parsedFile* file) {

    char * cur_ptr = file->text_shifted; // pointer we will put
    size_t cur_ind = 0; // index to where we will put

    for (size_t ind = 0; ind <= file->text_length; ind++){
        if (file->text_shifted[ind] == '\0'){
            file->pointers[cur_ind].start = cur_ptr;
            cur_ptr = &file->text_shifted[ind] + 1;
            cur_ind++;
        }
    }
}

void set_end_pointers(parsedFile* file) {
    assert(file->pointers[0].start != NULL);

    for (size_t ind = 0; ind < file->line_amount - 1; ind++) {
        // pointer to the end is 2 chars to the left from the next pointer
        file->pointers[ind].end = file->pointers[ind+1].start - 2; 
    }

    size_t length_of_the_last = (
        file->text_length 
        -(size_t)file->pointers[file->line_amount-1].start
        +(size_t)file->pointers[0].start
    );

    char* the_last_pointer = file->pointers[file->line_amount-1].start + length_of_the_last -1;
    file->pointers[file->line_amount-1].end = the_last_pointer;
}

