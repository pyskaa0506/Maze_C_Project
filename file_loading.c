#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include "file_loading.h"

// f-cja do wyszwietlania info o uzyciu programy
void print_usage(){
    printf("Usage of the programme - program_name -f [maze_file.txt] [-o output_file.txt] [-h]\n");

}

// przetwarzanie argumentów za pomoca getopta
void process_input(int argc, char *argv[], char **input_filename, char **output_filename) {
    int opt;

    // Inicjalizacja output_filename na NULL
    *output_filename = NULL;

    while ((opt = getopt(argc, argv, "f:o:h")) != -1) {
        switch (opt) {
            case 'f':
                *input_filename = optarg;
                break;
            case 'o':
                *output_filename = optarg;
                break;
            case 'h':
                print_usage();
                exit(EXIT_SUCCESS);
            default:
                print_usage();
                exit(EXIT_FAILURE);
        }
    }

    if (*input_filename == NULL){
        print_usage();
        exit(2); // blad 2. Brak mozliwosci wczytania pliku
    }

    if (!file_exists(*input_filename)) {
        fprintf(stderr, "Error. Unable to open input file - %s\n", *input_filename);
        exit(2); // blad 2. Brak mozliwosci wczytania pliku
    }

    if (!is_valid_input_file(*input_filename)) {
        fprintf(stderr, "Error. Invalid input file format - %s\n", *input_filename);
        exit(4); // blad 4. Nieprawidlowy format pliku
    }

    if (!is_valid_maze_format(*input_filename)) {
        fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename);
        exit(5); // blad 5. Nieprawidlowy format labiryntu
    }
}

// czy plik istnieje w systemie
bool file_exists(const char *filename){
    FILE *file = fopen(filename, "r");
    if(file){
        fclose(file);
        return true;
    }
    return false;
}

//sprawdzenie czy plik ma prawidlowe rozszerzenie '.txt' oraz czy mozna go otworzyc i wczytac
bool is_valid_input_file( const char *filename){
    if (file_exists(filename)){
        const char *extension = ".txt";
        if (strstr(filename, extension) != NULL){
            return true;
        }
    }
    return false;
}

//funkcja do testowania poprawnosci formatu labiryntu
#define MAX_LINE_LENGTH 10000

bool is_valid_maze_format(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        return false;
    }

    char line[MAX_LINE_LENGTH];
    int num_p = 0, num_k = 0;
    size_t expected_line_length = 0;
    size_t line_num = 0;

    while (fgets(line, sizeof(line), file)) {
        line_num++;

        // sprawdza dlugosc linii
        size_t line_length = strlen(line);
        if (line_length > MAX_LINE_LENGTH) {
            fprintf(stderr, "Error. Line too long in maze file (line %zu).\n", line_num);
            fclose(file);
            return false;
        }

        // usuwa znak nowej liniji jesli istnieje
        if (line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
            line_length--;
        }

        // czy wszystkie linije maja ta sama dlugosc
        if (line_num == 1) {
            expected_line_length = line_length;
        } else {
            if (line_length != expected_line_length) {
                fprintf(stderr, "Error. Inconsistent line length in maze file.\n");
                fclose(file);
                return false;
            }
        }

        // czy linija zawiera prawidlowe znaki
        for (size_t i = 0; i < line_length; i++) {
            if (line[i] == 'P') {
                num_p++;
            } else if (line[i] == 'K') {
                num_k++;
            } else if (line[i] != ' ' && line[i] != 'X') {
                fprintf(stderr, "Error. Invalid character in maze file (line %zu).\n", line_num);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);

    // czy istnieje tylko jedno wejscie i wyjscie
    if (num_p != 1 || num_k != 1) {
        fprintf(stderr, "Error. Incorrect number of entrances or exits in maze file.\n");
        return false;
    }

    return true;
}
