#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"
#include "chunks_handling.h"
#include "path_finding.h"

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
    
    const char* filepath = "../default_maps/25x50_20.txt";
    int16_t rows, cols; // Zmiana na int16_t
    get_maze_dimensions(filepath, &cols, &rows); // Przekazanie wskaŸników na int16_t

   if (!get_maze_dimensions(input_filename, &cols, &rows)) {
        fprintf(stderr, "Failed to get maze dimensions from file '%s'.\n", input_filename);
        return EXIT_FAILURE;
    }
    printf("Maze dimensions: %dx%d\n", cols, rows);

    char** maze = read_maze(filepath, rows, cols);
    dijkstra(maze, rows, cols, 1, 0);  // Assuming starting point at (1,0) !!!

    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);

    // zwolnienie pami?ci zaalokowanej na nazwy plików
    //free(input_filename);
    free(output_filename);

    /*
    Tutorial do move_chuck
    char initial_chunk_path[20] = "chunk1.txt";

    int16_t chunk_row_size = 10;
    int16_t chunk_col_size = 10;

    int16_t total_chunks = 10;

    find_exit(initial_chunk_path, chunk_row_size, chunk_col_size, total_chunks);

    */



    /*
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
    */

    return EXIT_SUCCESS;
}