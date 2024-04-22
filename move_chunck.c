#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "chunks_handling.h"
#include "move_chunck.h"
#include "path_finding.h"

#include <stdio.h>
#include <stdlib.h>


Chunk* load_chunk(const char* directory_path, int chunk_number) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s/%d.txt", directory_path, chunk_number);

    printf("Attempting to open file: %s\n", filepath);

    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file %s\n", filepath);
        return NULL;
    }

    Chunk* chunk = (Chunk*)malloc(sizeof(Chunk));
    if (!chunk) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d %d\n", &chunk->rows, &chunk->cols) != 2) {
        fprintf(stderr, "Error: Couldn't read dimensions from file %s\n", filepath);
        fclose(file);
        free(chunk);
        return NULL;
    }

    chunk->maze = (char**)malloc(chunk->rows * sizeof(char*));
    if (!chunk->maze) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        free(chunk);
        return NULL;
    }
    for (int i = 0; i < chunk->rows; i++) {
        chunk->maze[i] = (char*)malloc((chunk->cols + 1) * sizeof(char));
        if (!chunk->maze[i]) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            for (int j = 0; j < i; j++) {
                free(chunk->maze[j]);
            }
            free(chunk->maze);
            free(chunk);
            return NULL;
        }
    }

    for (int i = 0; i < chunk->rows; i++) {
        if (fgets(chunk->maze[i], chunk->cols + 1, file) == NULL) {
            fprintf(stderr, "Error: Couldn't read maze from file %s\n", filepath);
            fclose(file);
            for (int j = 0; j < chunk->rows; j++) {
                free(chunk->maze[j]);
            }
            free(chunk->maze);
            free(chunk);
            return NULL;
        }
    }

    fclose(file);
    return chunk;
}



void save_chunk(const char* filepath, Chunk* chunk) {
    FILE *file = fopen(filepath, "w");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file %s for writing\n", filepath);
        return;
    }

    fprintf(file, "%d %d\n", chunk->rows, chunk->cols);

    for (int i = 0; i < chunk->rows; i++) {
        fprintf(file, "%s\n", chunk->maze[i]);
    }

    fclose(file);
}

void find_entry_point(Chunk* chunk, int* entry_row, int* entry_col) {
    for (int i = 0; i < chunk->rows; i++) {
        for (int j = 0; j < chunk->cols; j++) {
            if (chunk->maze[i][j] == 'P') {
                *entry_row = i;
                *entry_col = j;
                return;
            }
        }
    }
    *entry_row = -1;
    *entry_col = -1;
}

void convert_coordinates(Chunk* current_chunk, int global_row, int global_col, int* local_row, int* local_col) {
    int chunk_offset_row = global_row % current_chunk->rows;
    int chunk_offset_col = global_col % current_chunk->cols;

    *local_row = chunk_offset_row;
    *local_col = chunk_offset_col;
}


void dijkstra_wrapper(Chunk* current_chunk, int global_start_row, int global_start_col, int global_target_row, int global_target_col, char* maze_filepath) {
    int local_start_row, local_start_col;
    int local_target_row, local_target_col;

    convert_coordinates(current_chunk, global_start_row, global_start_col, &local_start_row, &local_start_col);
    convert_coordinates(current_chunk, global_target_row, global_target_col, &local_target_row, &local_target_col);

    dijkstra(current_chunk->maze, current_chunk->rows, current_chunk->cols, local_start_row, local_start_col, maze_filepath);
}

Chunk* find_chunk_by_coordinates(const char* directory_path, int global_row, int global_col) {
    DIR *dir;
    struct dirent *entry;
    Chunk *found_chunk = NULL;

    dir = opendir(directory_path);
    if (dir == NULL) {
        fprintf(stderr, "Error: opening directory failed %s\n", directory_path);
        return NULL;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);

            // Pobierz numer chunka z nazwy pliku
            int chunk_number = atoi(entry->d_name);

            Chunk *chunk = load_chunk(directory_path, chunk_number);
            if (chunk == NULL) {
                fprintf(stderr, "Error: Couldn't load chunk from file %s\n", file_path);
                continue;
            }

            if (global_row < chunk->rows && global_col < chunk->cols) {
                found_chunk = chunk;
                break;
            } else {
                free(chunk);
            }
        }
    }
    closedir(dir);
    return found_chunk;
}
