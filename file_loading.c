#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //dla getopta
#include "file_loading.h"

// f-cja do wy�wietlania info o u�yciu programy
void print_usage(){
    printf("Usage of the programme - program_name -f [maze_file.txt] [-o output_file.txt] [-h]\n");

}

// przetwarzanie argument�w za pomoc� getopta
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
        exit(2); // B��d 2. Brak mo�liwo�ci wczytania pliku
    }

    if (!file_exists(*input_filename)) {
        fprintf(stderr, "Error. Unable to open input file - %s\n", *input_filename);
        exit(2); // B��d 2. Brak mo�liwo�ci wczytania pliku
    }

    if (!is_valid_input_file(*input_filename)) {
        fprintf(stderr, "Error. Invalid input file format - %s\n", *input_filename);
        exit(4); // B��d 4. Nieprawid�owy format pliku
    }

    if (!is_valid_maze_format(*input_filename)) {
        fprintf(stderr, "Error. Invalid maze format - %s\n", *input_filename);
        exit(5); // B��d 5. Nieprawid�owy format labiryntu
}
}

//pobieranie nazwy pliku z argumet�w wiersza polece�
char* get_input_filename(int argc, char *argv[]){
    char *input_filename = NULL;
    char *output_filename = NULL;
    process_input(argc, argv, &input_filename, &output_filename);
    return input_filename;
}

//pobierania nazwy pliku wyj�ciowego z argument�w wiersza polece�
char* get_output_filename(int argc, char *argv[]) {
    char *input_filename = NULL;
    char *output_filename = NULL;
    process_input(argc, argv, &input_filename, &output_filename);
    return output_filename;
}

//sprawdzenie czy plik istnieje w systemie
bool file_exists(const char *filename){
    FILE *file = fopen(filename, "r");
    if(file){
        fclose(file);
        return true;
    }
    return false;
}

//sprawdzenie czy plik ma prawid�owe rozszerzenie '.txt' oraz czy mo�na go otworzy� i wczyta�
bool is_valid_input_file( const char *filename){
    if (file_exists(filename)){
        const char *extension = ".txt";
        if (strstr(filename, extension) != NULL){
            return true;
        }
    }
    return false;
}

//plik wej�ciowy jest nieprawidlowy lub nie mo�na go otworzy�
void handle_invalid_input_file(const char *filename){
    fprintf(stderr, "Error. Invalid input file - %s\n", filename);
    exit(EXIT_FAILURE);
}


//funkcja do testowania poprawno�ci formatu labiryntu
bool is_valid_maze_format(const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file) //je�eli plik = NULL
    {
        fprintf(stderr, "Error. Unable to open file - %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int num_p = 0;
    int num_k = 0;
    //num_p oraz num_k to liczniki wej�� i wyj��

    while(fgets(line, sizeof(line), file)) 
    {
        for (int i = 0; line[i]; i++){
            if (line[i] == 'P')
            {
                num_p++;
            }
            else if(line[i] == 'K')
            {
                num_k++;
            }
            else if(line[i] != ' ' && line[i] != 'X') //nieprawid�owy znak
            {
                fclose(file);
                return false; 
            }
        }
    }

    fclose(file);

    if(num_p != 1 || num_k != 1){
        return false;
    }

    return true;
}