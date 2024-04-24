#include "chunks_handling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void free_txt_chunk(char **buffer, int16_t chunk_row_counter) {
    for (int i = 0; i < chunk_row_counter; i++) {
        free(buffer[i]);
    }
    free(buffer);
}

char **read_txt_chunk(char *filepath, size_t col, size_t chunk_row_counter) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file\n");
        return NULL;
    }

    char **buffer = (char **)malloc(sizeof(char *) * chunk_row_counter);
    if (buffer == NULL)
    {
        fprintf(stderr,"Error: Couldn't allocate memory\n");
        fclose(file);
        return NULL;
    }


    for (int i = 0; i < chunk_row_counter; i++) {
        buffer[i] = (char *)malloc(sizeof(char) * (col * 2 + 3));
        if (buffer[i] == NULL){
            fprintf(stderr,"Error: Couldn't allocate memory\n");
            fclose(file);
            // Zwolnienie pamiêci zaalokowanej wczeœniej
            for (int j = 0; j < i; j++) {
                free(buffer[j]);
            }
            free(buffer);
            return NULL;
        }
        fgets(buffer[i], col * 2 + 3, file);
    }

    fclose(file);
    return buffer;
}

int txt_file_to_txt_chunks(const char *filepath, size_t col, size_t row, size_t chunk_rows_counter) {
    int cols_in_file = col * 2 + 3;
    delete_files_in_directory("../chunks");
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file\n");
        return -1;
    }

    unsigned int *buffer = (unsigned int *)malloc(sizeof(unsigned int) * chunk_rows_counter * (cols_in_file));
    if (buffer == NULL){
        fprintf(stderr,"Error: Couldn't allocate memory\n");
        exit(1);
    }

    int chunkCount = 1;
    while (!feof(file)) {
        char filename[20];
        sprintf(filename, "../chunks/%d.txt", chunkCount);

        FILE *new_file = fopen(filename, "w");
        if (!new_file) {
            fprintf(stderr, "Error: Couldn't open the file\n");
            fclose(file);
            free(buffer);
            return -1;
        }

        // Read chunk_rows_counter lines from the file
        int linesRead = 0;
        while (linesRead < chunk_rows_counter && fgets((char *)(buffer + linesRead * (cols_in_file)), cols_in_file, file)) {
            fprintf(new_file, "%s",(char *)(buffer + linesRead * (cols_in_file)));
            linesRead++;
        }



        while (linesRead < chunk_rows_counter) {
            for (int i = 0; i < col * 2 + 1; i++) fprintf(new_file, "X");
            fprintf(new_file, "\n");
            linesRead++;
        }
        fclose(new_file);

        chunkCount++;
    }
    free(buffer);
    fclose(file);
    return chunkCount - 1;
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
            }
//            else {
//                printf("Deleted file: %s\n", file_path);
//            }
        }
    }
    closedir(dir);
}

