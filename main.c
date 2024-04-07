
#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"
#include "chunks_handling.h"

int main(int argc, char *argv[]) {
    // deklaracja zmiennych na nazwy plików
    char *input_filename = NULL;
    char *output_filename = NULL;

    // getopt()
    process_input(argc, argv, &input_filename, &output_filename);

    printf("Input filename: %s\n", input_filename);
    printf("Output filename: %s\n", output_filename);

    // sprawdzenie czy plik istnieje
    if (file_exists(input_filename)) {
        printf("Input file exists.\n");
    } else {
        printf("Input file does not exist.\n");
    }

    // sprawdzenie, czy plik jest binarny czy tekstowy
    if (is_binary_file_v2
(input_filename)) {
        printf("File is binary.\n");
        if (is_valid_binary_maze_format_v2(input_filename)) {
            printf("Binary maze format is valid.\n");
        } else {
            printf("Binary maze format is invalid.\n");
        }
    } else {
        printf("File is text.\n");
        if (is_valid_maze_format_v2(input_filename)) {
            printf("The maze format is valid.\n");
        } else {
            printf("The maze format is invalid.\n");
        }
    }

    // zwolnienie pamiêci zaalokowanej na nazwy plików
    //free(input_filename);
    free(output_filename);
    
        char *filepath = "../default_maps/25x50_20.txt";
    int16_t col = 25;
    int16_t row = 50; //remember it is 2 * row + 1 in file
    int16_t chunk_rows_counter = 50; // How many rows should be packed into a chunk (file)
    int chunk_counter = txt_file_to_txt_chunks(filepath, col, row, chunk_rows_counter); //this separates the file into chunks and returns the number of chunks created


    //tutorial
    char *filepath_chunk = "../chunks/3.txt";
    char **chunk = read_txt_chunk(filepath_chunk, col, row, chunk_rows_counter); //this reads the first chunk and returns it as a 2D array that you can work on

    // and if you want to print every chunk char by char
    for (int i = 1; i <= chunk_counter; i++) {
        char filename[10];
        sprintf(filename, "../chunks/%d.txt", i);
        char **chunk = read_txt_chunk(filename, col, row, chunk_rows_counter);
        for (int j = 0; j < chunk_rows_counter; j++) {
            for (int k = 0; k < col * 2 + 1; k++) {
                printf("%c", chunk[j][k]);
            }
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}