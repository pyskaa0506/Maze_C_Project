#ifndef MAZE_C_PROJECT_CHUNKS_HANDLING_H
#define MAZE_C_PROJECT_CHUNKS_HANDLING_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>

char *change_chunk_number(char operation, const char *act_file); //function that increases/decreases number in filenames like "1.bin"
void delete_files_in_directory(const char *directory_path); //function that deletes all files in a directory
int txt_file_to_txt_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_rows_counter); //function that reads a txt file and writes its content to multiple txt files
int txt_file_to_bin_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size); //function that reads a txt file and writes its content to multiple bin files
char **read_txt_chunk(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size); //function that reads a txt file and returns its content as a 2D array
#endif //MAZE_C_PROJECT_CHUNKS_HANDLING_H
