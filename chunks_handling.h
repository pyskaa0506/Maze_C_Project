#ifndef MAZE_C_PROJECT_CHUNKS_HANDLING_H
#define MAZE_C_PROJECT_CHUNKS_HANDLING_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>

char *change_chunk_nuber(char operation, const char *act_file); //function that increases/decreases number in filenames like "1.bin"
uint8_t convert_char_to_bit(char c); //function that converts a character to a bit (X to 1, anything else to 0)
void compress_data(char *line, int length, uint8_t *compressed); //function that compresses data
void print_compressed_data(uint8_t *compressed, int length); //function that prints compressed data
void write_compressed_data_to_file(uint8_t *compressed, int length, FILE *binary_file); //function that writes compressed data to a file
void delete_files_in_directory(const char *directory_path); //function that deletes all files in a directory
void txt_file_to_txt_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size); //function that reads a txt file and writes its content to multiple txt files
int txt_file_to_bin_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size); //function that reads a txt file and writes its content to multiple bin files
#endif //MAZE_C_PROJECT_CHUNKS_HANDLING_H
