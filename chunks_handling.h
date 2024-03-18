#ifndef MAZE_C_PROJECT_CHUNKS_HANDLING_H
#define MAZE_C_PROJECT_CHUNKS_HANDLING_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dirent.h>

char *chng_chnk_nbr(char operation, const char *act_file); //function that increases/decreases number in filenames like "1.bin"
uint8_t convert_char_to_bit(char c); //function that converts a character to a bit (X to 1, anything else to 0)
void compress_data(char *line, int length, uint8_t *compressed); //function that compresses data
void print_compressed_data(uint8_t *compressed, int length); //function that prints compressed data
void write_compressed_data_to_file(uint8_t *compressed, int length, FILE *binaryFile); //function that writes compressed data to a file
void delete_files_in_directory(const char *directoryPath); //function that deletes all files in a directory
#endif //MAZE_C_PROJECT_CHUNKS_HANDLING_H
