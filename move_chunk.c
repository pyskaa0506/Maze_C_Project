#include <stdio.h>
#include <stdlib.h>
#include "move_chunk.h"

char** read_maze(const char* filepath, int rows, int cols) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Error opening file.\n");
        return NULL;
    }

    char** maze = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        maze[i] = malloc((cols + 1) * sizeof(char));
        fgets(maze[i], cols + 2, file);
    }

    fclose(file);
    return maze;
}

char find_and_return_char_from_chunk(int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char ***current_chunk, int how_many_chunks){
    if (current_row < 0 || current_col < 0) return 0;
    if ((current_row <= (*loaded_chunk_number * chunk_row_counter) - 1) && (current_row >= (*loaded_chunk_number - 1) * chunk_row_counter)) {
        if ((current_row < 0) || (current_col < 0) || (current_row >= (chunk_row_counter * (*loaded_chunk_number))) || (current_col >= cols)) {
            printf("Error: Path goes out of bounds, Col: %d/%d Row: %d (%d-%d) Chunk: %d ", current_col, cols, current_row, ((*loaded_chunk_number - 1) * chunk_row_counter),(*loaded_chunk_number) * chunk_row_counter -1 ,*loaded_chunk_number);
            printf("Char: %c ",(*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col]);
            printf("Debug conditions: %d %d %d %d\n", current_row < 0, current_col < 0, (current_row >= (chunk_row_counter * (*loaded_chunk_number))), (current_col >= cols));
            return 0;
        }
        return (*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col];
    }
    if (current_row > (*loaded_chunk_number * chunk_row_counter) - 1)
    {
        while ((current_row > (*loaded_chunk_number * chunk_row_counter) - 1) && (*loaded_chunk_number * chunk_row_counter < how_many_chunks * chunk_row_counter - 1))
        {
            *loaded_chunk_number += 1;
        }
    } else {
        while ((current_row < ((*loaded_chunk_number - 1) * chunk_row_counter)) && *loaded_chunk_number > 1)
        {
            *loaded_chunk_number -= 1;
        }
    }
    char new_path[25];
    snprintf(new_path, 25, "../chunks/%d.txt", *loaded_chunk_number);

    for (int i = 0; i < chunk_row_counter; i++) {
        free((*current_chunk)[i]);
    }
    free(*current_chunk);

    (*current_chunk) = read_maze(new_path, chunk_row_counter, cols);

    return (*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col];
}

void find_and_save_char_into_chunk(char new_char, int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char ***current_chunk, int how_many_chunks){
    if (current_row < 0 || current_col < 0) return;
    if ((current_row <= *loaded_chunk_number * chunk_row_counter - 1) && (current_row >= (*loaded_chunk_number - 1) * chunk_row_counter)) {
        (*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col] = new_char;
    }
    if (current_row > (*loaded_chunk_number * chunk_row_counter) - 1)
    {
        while ((current_row > (*loaded_chunk_number * chunk_row_counter) - 1) && (*loaded_chunk_number * chunk_row_counter < how_many_chunks * chunk_row_counter - 1))
        {
            *loaded_chunk_number += 1;
        }
    } else {
        while ((current_row < ((*loaded_chunk_number - 1) * chunk_row_counter)) && *loaded_chunk_number > 1)
        {
            *loaded_chunk_number -= 1;
        }
    }
    char new_path[25];
    snprintf(new_path, 25, "../chunks/%d.txt", *loaded_chunk_number);

    for (int i = 0; i < chunk_row_counter; i++) {
        if ((*current_chunk)[i] != NULL) {
//            printf("Freeing %s", (*current_chunk)[i]);
            free((*current_chunk)[i]);
            (*current_chunk)[i] = NULL; // Nullify the pointer after freeing memory
//            printf("Freed %s", (*current_chunk)[i]);
        }
    }

    free(*current_chunk);
    *current_chunk = NULL;

    (*current_chunk) = read_maze(new_path, chunk_row_counter, cols);
    (*current_chunk)[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col] = new_char;

    // Save the updated chunk back to the file
    FILE* file = fopen(new_path, "w");
    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < chunk_row_counter; i++) {
        fprintf(file, "%s", (*current_chunk)[i]);
    }
    fclose(file);
}