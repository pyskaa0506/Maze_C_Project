#ifndef MAZE_C_PROJECT_CHUNKS_HANDLING_H
#define MAZE_C_PROJECT_CHUNKS_HANDLING_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>


void delete_files_in_directory(const char *directory_path); //function that deletes all files in a directory
int txt_file_to_txt_chunks(const char *filepath, size_t col, size_t row, size_t chunk_rows_counter); //function that reads a txt file and saves it in chunks
char **read_txt_chunk(char *filepath, size_t col, size_t chunk_row_counter); //function that reads a chunk from a txt file
#endif //MAZE_C_PROJECT_CHUNKS_HANDLING_H
