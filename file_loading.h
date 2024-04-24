#ifndef FILE_LOADING_H
#define FILE_LOADING_H

#include <stdbool.h> //true and false
#include <stdint.h> //int16_t

#define MAX_LINE_LENGTH 1030
#define MAX_LINE_LENGTH_BIN 7000

void print_usage();

void process_input(int argc, char *argv[], char **input_filename, char **output_filename);

bool file_exists(const char *filename);

bool is_valid_input_file(const char *filename);

bool is_valid_maze_format_v2(const char *filename);

bool is_binary_file_v2(const char *filename);

bool is_valid_binary_maze_format_v2(const char *filename);

bool get_maze_dimensions(const char *filename, int16_t *col, int16_t *row);

#endif
