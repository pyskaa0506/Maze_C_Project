#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include "file_loading.h"

// f-cja do wyszwietlania info o uzyciu programy
// f-cja do wyswietlania info o uzyciu programy
void print_usage(){
    printf("Usage of the program: \n");
    printf("program_name -f [maze_file.txt] [-o output_file.txt] [-h]\n\n");
    printf("Options:\n");
    printf("-f\tSpecify the input maze file (required)\n");
    printf("-o\tSpecify the output file (optional)\n");
    printf("-h\tDisplay this help message\n\n");
    printf("Description:\n");
    printf("This program reads a maze from a text file and performs various operations on it.\n");
    printf("It can validate the maze format, find a solution path, and output the result to a file.\n");
    printf("The maze file should contain a rectangular grid of characters representing walls 'X', paths ' ',\n");
    printf("starting point 'P', and exit 'K' point. The program supports both text and binary maze file formats.\n");
}


// przetwarzanie argumentów za pomoca getopta
void process_input(int argc, char *argv[], char **input_filename, char **output_filename) {
    int opt;

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
    if(is_binary_file_v2(*input_filename)){
        if(!is_valid_binary_maze_format_v2(*input_filename)){
            fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename);
            exit(5); // blad 5. Nieprawidlowy format labiryntu
        }
    }
    else
    {
        if(!is_valid_input_file(*input_filename)){
            fprintf(stderr, "Error. Invalid input file format - %s\n", *input_filename);
            exit(4); // blad 4. Nieprawidlowy format pliku
        }  

        if(!is_valid_maze_format_v2(*input_filename)){
            fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename);
            exit(5); // blad 5. Nieprawidlowy format labiryntu
        }
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

// PLIK TEKSTOWY

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
bool is_valid_maze_format_v2(const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        return false;
    }

    char line[MAX_LINE_LENGTH];
    int num_p = 0, num_k = 0;
    size_t expected_line_length = 0;
    size_t line_num = 0;

    while(fgets(line, sizeof(line), file)){
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
            if ((line_num == 1 || i == 0|| i == line_length - 1) && line[i] != 'X' && line[i] != 'P' && line[i] != 'K') {
                fprintf(stderr, "Error. Border of maze must be composed of 'X' characters.\n");
                fclose(file);
                return false;
            } else {
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
    }
    fclose(file);

    // czy istnieje tylko jedno wejscie i wyjscie
    if (num_p != 1 || num_k != 1) {
        fprintf(stderr, "Error. Incorrect number of entrances or exits in maze file.\n");
        return false;
    }

    return true;
}

// PLIK BINARNY 

// funkcja do sprawdzenia czy plik jest binarny 
bool is_binary_file_v2(const char *filename){
    FILE *file = fopen(filename, "rb"); // Otwieramy plik w trybie binarnym tylko do odczytu
    if (!file) {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        return false;
    }

    bool binary = false;
    int ch;
    while ((ch = fgetc(file)) != EOF) { 
        if (ch < 0x07 || ch > 0x7E) { 
            binary = true;
            break;
        }
    }

    fclose(file);
    return binary;
}

// funkcja do sprawdzenia poprawnosci formatu labiryntu dla pliku binarnego
bool is_valid_binary_maze_format_v2(const char *filename){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        return false;
    }

    char line[MAX_LINE_LENGTH_BIN];
    size_t expected_line_length = 0;
    size_t line_num = 1;

    while(fread(line, 2, MAX_LINE_LENGTH_BIN, file) > 0){ 
        line_num++;

        // sprawdzenie d?ugosci linii
        size_t line_length = strlen(line);
        if (line_length > MAX_LINE_LENGTH_BIN) {
            fprintf(stderr, "Error. Line too long in maze file (line %zu).\n", line_num);
            fclose(file);
            return false;
        }
    
        // pierwsza linia = ustalenie oczekiwanej dlugosci
        if (line_num == 2) {
            expected_line_length = line_length;
        } else {
            if (line_length != expected_line_length) {
                fprintf(stderr, "Error. Inconsistent line length in maze file.\n");
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);

    return true;
}

bool get_maze_dimensions(const char *filename, int16_t *col, int16_t *row) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file - %s\n", filename);
        return false;
    }

    int16_t max_col = 0;
    int16_t current_row = 0;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        current_row++;

        // Usuni?cie bia?ych znaków z ko?ca linii
        int16_t line_length = strlen(line);
        while (line_length > 0 && (line[line_length - 1] == '\n' || line[line_length - 1] == '\r' || line[line_length - 1] == ' ')) {
            line_length--;
        }

        if (line_length > max_col) {
            max_col = line_length;
        }
    }

    *col = max_col;
    *row = current_row;

    fclose(file);
    return true;
}
