#ifndef MAZE_C_PROJECT_CHUNKS_HANDLING_H
#define MAZE_C_PROJECT_CHUNKS_HANDLING_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>


void delete_files_in_directory(const char *directory_path); //function that deletes all files in a directory
int txt_file_to_txt_chunks(const char *filepath, int16_t col, int16_t row ,int16_t chunk_rows_counter); //function that reads a txt file and saves it in chunks

// arguments:
// filepath - filepath to chunk for e.g. "../chunks/1.txt"
// col - number of columns in the input file
// chunk_row_counter - originally chosen  number of rows in a chunk
char **read_txt_chunk(char *filepath, int16_t col, int16_t chunk_row_counter); //function that reads a txt file and returns its content as a 2D array
#endif //MAZE_C_PROJECT_CHUNKS_HANDLING_H
