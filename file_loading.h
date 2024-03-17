#ifndef FILE_LOADING_H
#define FILE_LOADING_H

#include <stdbool.h> //true and false

void print_usage();

void process_input(int argc, char *argv[], char **input_filename, char **output_filename);

char* get_input_filename(int argc, char *argv[]);

char* get_output_filename(int argc, char *argv[]);

bool file_exists(const char *filename);

bool is_valid_input_file(const char *filename);

void handle_invalid_input_file(const char *filename);

bool is_valid_maze_format(const char *filename); 

#endif
