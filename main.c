#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"
#include "chunks_handling.h"
#include "bin_chunks_handling.h"
#include "path_finding.h"
#include "move_chunck.h"

int main(int argc, char *argv[]) {
    // deklaracja zmiennych na nazwy plików
    char *input_filename = NULL;
    char *output_filename = NULL;
    char* filepath = NULL;
    int16_t chunk_rows_counter = 50; // How many rows should be packed into a chunk (file)
    int16_t rows, cols; // Zmiana na int16_t
    int how_many_chunks = 0; // will probably be used in the future


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
    if (is_binary_file_v2(input_filename)) {
        printf("File is binary.\n");
        if (is_valid_binary_maze_format_v2(input_filename)) {
            printf("Binary maze format is valid.\n");
            how_many_chunks = read_bin_file(input_filename, chunk_rows_counter);
            filepath = "../default_maps/bin_output.txt";
            if (get_maze_dimensions(filepath, &cols, &rows)) {
                printf("Maze dimensions: %dx%d\n", cols, rows);
            } else {
                fprintf(stderr, "Failed to get maze dimensions from file '%s'.\n", input_filename);
                return EXIT_FAILURE;

            }
        } else {
            printf("Binary maze format is invalid.\n");
            exit(EXIT_FAILURE);
        }
    } else {
        printf("File is text.\n");
        if (is_valid_maze_format_v2(input_filename)) {
            printf("The maze format is valid.\n");
            filepath = input_filename;
            if (get_maze_dimensions(filepath, &cols, &rows)) {
                printf("Maze dimensions: %dx%d\n", cols, rows);
            } else {
                fprintf(stderr, "Failed to get maze dimensions from file '%s'.\n", input_filename);
                return EXIT_FAILURE;
            }
            how_many_chunks = txt_file_to_txt_chunks(filepath, cols, rows, chunk_rows_counter); //this separates the file into chunks and returns the number of chunks created
        } else {
            printf("The maze format is invalid.\n");
            exit(EXIT_FAILURE);
        }
    }

    char** maze = read_maze(filepath, rows, cols);
    //djikstra rn only works nicely for file 25x50_20.txt, but don't worry, it will be fixed
    //dijkstra(maze, rows, cols, 1, 0, filepath);  // Assuming starting point at (1,0) !!!

    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);

    // zwolnienie pamiêci
    //free(input_filename);
    //free(output_filename);

    return EXIT_SUCCESS;
}