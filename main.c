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

    // Znajd? odpowiedni chunk na podstawie wspó?rz?dnych lub innych kryteriów
    Chunk* current_chunk = find_chunk_by_coordinates("../chunks", 0, 0); // Przyk?adowe wspó?rz?dne (0, 0)


    // Wczytanie labiryntu
    char** maze = read_maze(filepath, rows, cols);

    // Losowe wspó?rz?dne pocz?tkowe i ko?cowe
    int start_row = 1;
    int start_col = 0;
    int target_row = rows - 1;
    int target_col = cols - 1;

    // Wywo?anie funkcji dijkstra_wrapper
    dijkstra_wrapper(current_chunk, start_row, start_col, target_row, target_col, filepath);

    // Zwolnienie pami?ci
    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);

    return EXIT_SUCCESS;
}
