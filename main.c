#include <stdio.h>
#include <stdlib.h>
#include "file_loading.h"

int main(int argc, char *argv[]) {
    // deklaracja zmiennych na nazwy plik�w
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

    // poprawnosc formatu labiryntu
    if (is_valid_maze_format(input_filename)) {
        printf("The maze format is valid.\n");
    } else {
        printf("The maze format is invalid.\n");
    }

    // zwolnienie pami�ci zaalokowanej na nazwy plik�w
    //free(input_filename);
    free(output_filename);

    return EXIT_SUCCESS;
}
