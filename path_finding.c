#include <stdio.h>
#include <stdlib.h>
#include "path_finding.h"

// Function to add a node and its distance to the unvisited list
int* add_node_and_distance(int* list, int* size, int node_index, int distance) {
    int* temp = realloc(list, (*size + 2) * sizeof(int));
    if (temp == NULL) {
        printf("Error reallocating memory.\n");
        free(list);
        return NULL;
    }
    list = temp;
    list[*size] = node_index;
    list[*size + 1] = distance;
    (*size) += 2;
    return list;
}

// Read the maze from a file
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
    if ((current_row <= *loaded_chunk_number * chunk_row_counter - 1) && (current_row >= (*loaded_chunk_number - 1) * chunk_row_counter)) {
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
        free((*current_chunk)[i]);
    }
    free(*current_chunk);

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

void backtrack_path(char*** current_chunk, int rows, int cols, int current_row, int current_col, int* loaded_chunk_number, int chunk_row_counter, int how_many_chunks)
{
    int number_of_steps = 0;
    FILE* file = fopen("../steps.txt", "w");

    while (find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks) != START) {
        number_of_steps+=1;
        char step = find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks);
        if (!file) {
            printf("Error opening file.\n");
            return;
        }
        fprintf(file, "%c", step);

        if (step == LEFT)
        {
            current_col -= 1;
        }
        else if (step == RIGHT)
        {
            current_col += 1;
        }
        else if (step == UP)
        {
            current_row -= 1;
        }
        else if (step == DOWN)
        {
            current_row += 1;
        }

        char new_char = find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks);

        if (current_row < 0 || current_row >= rows || current_col < 0 || current_col >= cols) {
            printf("Error: Path goes out of bounds.\n");
            break;
        }
        else if (new_char == WALL) {
            printf("Error: Path goes through a wall.\n");
            break;
        }
        else if (new_char == PATH) {
            printf("Error: Path goes through itself.\n");
            break;
        }
        else if (new_char == START) {
            printf("(%d, %d)\n", current_row, current_col);
            break;
        }
    }

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    FILE* count_file = fopen("../steps_count.txt", "w");
    if (!count_file) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(count_file, "%d", number_of_steps);

    fclose(file);
    fclose(count_file);
}




// Implementation of Dijkstra's Algorithm
void dijkstra(int rows, int cols, int start_row, int start_col, int16_t chunk_row_counter, int how_many_chunks)
{
    int loaded_chunk_number = 1;
    char** current_chunk = read_maze("../chunks/1.txt", chunk_row_counter, cols);

    char found = 0;
    int* unvisited = NULL;
    int unvisited_size = 0;
    unvisited = add_node_and_distance(unvisited, &unvisited_size, start_row * cols + start_col, 0);

    while (unvisited_size > 0 && found == 0) {
        // Find the node with the minimum distance
        int min_index = 1;
        for (int i = 3; i < unvisited_size; i += 2) {
            if (unvisited[i] < unvisited[min_index]) {
                min_index = i;
            }
        }

        int current = unvisited[min_index - 1];
        int current_distance = unvisited[min_index];
        // Remove the current node from the unvisited list
        unvisited[min_index - 1] = unvisited[unvisited_size - 2];
        unvisited[min_index] = unvisited[unvisited_size - 1];
        unvisited_size -= 2;

        int current_row = current / cols;
        int current_col = current % cols;

        // current_row and current_col == TARGET
        if (find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, &current_chunk, how_many_chunks)
                    == TARGET) {
            break;  // Stop the algorithm if the target is reached
        }

        // Check and update four possible directions
        int direction_offsets[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        char direction_marks[4] = {DOWN, UP, RIGHT, LEFT};

        for (int i = 0; i < 4; i++) {
            int neighbor_row = current_row + direction_offsets[i][0];
            int neighbor_col = current_col + direction_offsets[i][1];
            char neighbor_char = find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, neighbor_col, neighbor_row, &current_chunk, how_many_chunks);
            if (neighbor_row >= 0 && neighbor_row < rows && neighbor_col >= 0 && neighbor_col < cols && (neighbor_char == FREE_SPACE || neighbor_char == TARGET))
            {
                if (neighbor_char == TARGET) {
                    found = 1;
                    backtrack_path(&current_chunk, rows, cols, current_row, current_col, &loaded_chunk_number, chunk_row_counter, how_many_chunks);
                    break;
                }
                else
                {
                    // mark the direction you came from into the neighbor cell
                    find_and_save_char_into_chunk(direction_marks[i], &loaded_chunk_number, chunk_row_counter, cols,
                                                  neighbor_col, neighbor_row, &current_chunk, how_many_chunks);
                    unvisited = add_node_and_distance(unvisited, &unvisited_size, neighbor_row * cols + neighbor_col,
                                                      current_distance + 1);
                }
            }
        }
    }
    free(unvisited);
    for (int i = 0; i < chunk_row_counter; i++) {
        free(current_chunk[i]);
    }
    free(current_chunk);
}

