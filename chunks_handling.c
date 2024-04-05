#include "chunks_handling.h"
#include <stdio.h>
#include <stdlib.h>


void txt_file_to_bin_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size) {
    chunk_row_size *= 2;
    delete_files_in_directory("../chunks");
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file\n");
        return;
    }

    unsigned int *buffer = (unsigned int *)malloc(sizeof(unsigned int) * chunk_row_size * (col * 2 + 1));
    if (buffer == NULL){
        fprintf(stderr,"Error: Couldn't allocate memory\n");
        exit(1);
    }

    int chunkCount = 1;
    while (!feof(file)) {
        char filename[12]; // Increase filename buffer size to accommodate the extension
        sprintf(filename, "../chunks/%d.bin", chunkCount); // Change the extension to .bin

        FILE *new_file = fopen(filename, "wb"); // Open in binary mode
        if (!new_file) {
            fprintf(stderr, "Error: Couldn't open the file\n");
            fclose(file);
            free(buffer);
            return;
        }

        // Read chunk_row_size lines from the file
        int linesRead = 0;
        while (linesRead < chunk_row_size && fgets((char *)(buffer + linesRead * (col * 2 + 1)), col * 2 + 1, file)) {
            fwrite(buffer + linesRead * (col * 2 + 1), sizeof(unsigned int), col * 2 + 1, new_file); // Write binary data
            linesRead++;
        }
        fclose(new_file);

        chunkCount++;
    }
    free(buffer);
    fclose(file);
}

void txt_file_to_txt_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size) {
    chunk_row_size *= 2 + 1;
    delete_files_in_directory("../chunks");
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file\n");
        return;
    }

    unsigned int *buffer = (unsigned int *)malloc(sizeof(unsigned int) * chunk_row_size * (col * 2 + 1));
    if (buffer == NULL){
        fprintf(stderr,"Error: Couldn't allocate memory\n");
        exit(1);
    }

    int chunkCount = 1;
    while (!feof(file)) {
        char filename[10];
        sprintf(filename, "../chunks/%d.txt", chunkCount);

        FILE *new_file = fopen(filename, "w");
        if (!new_file) {
            fprintf(stderr, "Error: Couldn't open the file\n");
            fclose(file);
            free(buffer);
            return;
        }

        // Read chunk_row_size lines from the file
        int linesRead = 0;
        while (linesRead < chunk_row_size && fgets((char *)(buffer + linesRead * (col * 2 + 1)), col * 2 + 1, file)) {
            fprintf(new_file, "%s", (char *)(buffer + linesRead * (col * 2 + 1)));
            linesRead++;
        }
        fclose(new_file);

        chunkCount++;
    }
    free(buffer);
    fclose(file);
}


void delete_files_in_directory(const char *directory_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory_path);
    if (dir == NULL) {
        fprintf(stderr, "Error: opening directory failed %s\n", directory_path);
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);

            if (remove(file_path) != 0) {
                fprintf(stderr, "Error: deleting file failed: %s\n", file_path);
            } else {
                printf("Deleted file: %s\n", file_path);
            }
        }
    }
    closedir(dir);
}

//function that increases/decreases number in filenames like "1.bin"
char *change_chunk_number(char operation, const char *act_file) {
    char *num_extension = strchr(act_file, '.');
    int num_length = num_extension - act_file;
    char *num = malloc(num_length + 1);
    if (num == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    strncpy(num, act_file, num_length);
    num[num_length] = '\0';

    int num_value = atoi(num);

    if (operation == '+')
        num_value++;
    else if (operation == '-' && num_value > 0)
        num_value--;
    else {
        fprintf(stderr, "Error: Invalid operation or number cannot be decreased further\n");
        free(num); // Zwolnienie pamięci przed zwróceniem NULL
        return NULL;
    }

    char *new_filename = malloc(strlen(act_file) + 1);
    if (new_filename == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(num); // Zwolnienie pamięci przed zwróceniem NULL
        return NULL;
    }
    sprintf(new_filename, "%d%s", num_value, num_extension);

    free(num); // Zwolnienie pamięci używanej dla num
    return new_filename;
}