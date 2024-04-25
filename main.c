#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"
#include "chunks_handling.h"
#include "bin_chunks_handling.h"
#include "path_finding.h"
#include "file_saving.h"

int main(int argc, char *argv[]) {
    // deklaracja zmiennych na nazwy plików
    char *input_filename = NULL;
    char *output_filename = NULL;
    char *filepath = NULL;
    int16_t chunk_rows_counter = 50; // How many rows should be packed into a chunk (file)
    int16_t rows, cols; // Zmiana na int16_t
    int how_many_chunks = 0; // will probably be used in the future


//    input_filename = "../default_maps/maze.bin";

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

    bool is_binary = is_binary_file_v2(input_filename);
    // sprawdzenie, czy plik jest binarny czy tekstowy
    if (is_binary) {
        printf("File is binary.\n");
        if (is_valid_binary_maze_format_v2(input_filename)) {
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
            how_many_chunks = txt_file_to_txt_chunks(filepath, cols, rows,
                                                     chunk_rows_counter); //this separates the file into chunks and returns the number of chunks created
        } else {
            printf("The maze format is invalid.\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("Finding path... This may take a while.\n");
    dijkstra(rows, cols, 1, 0, chunk_rows_counter, how_many_chunks);  // Assuming starting point at (1,0) !!!

    int num_letters;
    FILE *input_file = fopen("../tmp/steps_count.txt", "r");
    if (input_file == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    fscanf(input_file, "%d", &num_letters);
    fclose(input_file);

    reverse_file("../tmp/steps.txt", "../tmp/reversed.txt", num_letters);
    change_directions("../tmp/reversed.txt", "../tmp/reversed_directions.txt", num_letters);
    add_char_to_end("../tmp/reversed_directions.txt", 'R');

    if (is_binary) {
        int code_words = bin_compress("../tmp/reversed_directions.txt", num_letters, "../tmp/final_output.txt");
        if (output_filename == NULL){
            printf("You didn't specify the output filename. Do you want to modify original file? y/n \n");
            char answer;
            scanf("%c", &answer);
            if (answer == 'y' || answer == 'Y'){
                modify_bin_file(input_filename, code_words , "../tmp/final_output.txt");
            } else if (answer == 'n' || answer == 'N'){
                printf("Saving to output.bin...\n");
                copy_file_bin(input_filename, "output.bin", "../results/");
                modify_bin_file("../results/output.bin", code_words , "../tmp/final_output.txt");
            } else {
                printf("Invalid answer.\n");
            }
        } else{
            copy_file_bin(input_filename, output_filename, "../results/");
            modify_bin_file(output_filename, code_words , "../tmp/final_output.txt");
        }

    } else {
        txt_compress("../tmp/reversed_directions.txt", num_letters, "../tmp/final_output.txt");
        if (output_filename == NULL) {
            print_file_to_console("../tmp/final_output.txt");
        } else {
            copy_file("../tmp/final_output.txt", output_filename, "../results/");
        }
    }

    delete_files_in_directory("../tmp");
    return EXIT_SUCCESS;
}