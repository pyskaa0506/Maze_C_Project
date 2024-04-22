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


void backtrack_path(char** maze, int rows, int cols, int target_row, int target_col) {
    int current_row = target_row;
    int current_col = target_col;

    // WARNING !!!
    // a really dumb way to get the original maze
    // it would be better to save copy of each labyrinth path, load and save it there
    // either way it's going to be needed to save the path into chunks so...
    char** maze_original = read_maze("../default_maps/25x50_20.txt", rows, cols);

    while (maze[current_row][current_col] != START) {
        char step = maze[current_row][current_col];
        maze[current_row][current_col] = PATH;
        maze_original[current_row][current_col] = PATH;

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
        if (current_row < 0 || current_row >= rows || current_col < 0 || current_col >= cols) {
            printf("Error: Path goes out of bounds.\n");
            break;
        }
        else if (maze[current_row][current_col] == WALL) {
            printf("Error: Path goes through a wall.\n");
            break;
        }
        else if (maze[current_row][current_col] == PATH) {
            printf("Error: Path goes through itself.\n");
            break;
        }
        else if (maze[current_row][current_col] == START) {
            printf("(%d, %d)\n", current_row, current_col);
            break;
        }
    }
    maze[target_row][target_col] = PATH; // Ensure target remains marked
    maze_original[target_row][target_col] = PATH;


    // print labirynt, it's not nor being saves yet !
    for (int i = 0; i < rows; i++) {
        printf("%s", maze[i]);
    }

    // Free the memory allocated for maze_original
    for (int i = 0; i < rows; i++) {
        free(maze_original[i]);
    }
    free(maze_original);
}

char find_and_return_char_from_chunk(int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char **current_chunk, int how_many_chunks){
    if ((current_row <= *loaded_chunk_number * chunk_row_counter - 1) && (current_row >= (*loaded_chunk_number - 1) * chunk_row_counter)) {
        return current_chunk[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col];
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
        free(current_chunk[i]);
    }
    free(current_chunk);

    current_chunk = read_maze(new_path, chunk_row_counter, cols);

    return current_chunk[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col];
}

void find_and_save_char_into_chunk(char direction_marks, int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char **current_chunk, int how_many_chunks){
    // save the char into the chunk
    if ((current_row <= *loaded_chunk_number * chunk_row_counter - 1) && (current_row >= (*loaded_chunk_number - 1) * chunk_row_counter)) {
        current_chunk[current_row - (*loaded_chunk_number - 1) * chunk_row_counter][current_col] = direction_marks;
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
}

// Implementation of Dijkstra's Algorithm
void dijkstra(int rows, int cols, int start_row, int start_col, char *filepath, int16_t chunk_row_counter, int how_many_chunks) {
    // Debugging variables
    int debug_most_unvisited_nodes = 0;
    /////////////

    int loaded_chunk_number = 3;
    char** current_chunk = read_maze("../chunks/3.txt", chunk_row_counter, cols);

    /////////////
//    print maze using find_and_return_char_from_chunk
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c", find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, j, i, current_chunk, how_many_chunks));
            fflush(stdout);
        }
        printf("\n ");
    }

    ///////////// set 60row 20 col to "O"
//    find_and_save_char_into_chunk('O', &loaded_chunk_number, chunk_row_counter, cols, 0, 60, current_chunk, how_many_chunks);

    char found = 0;
    int* unvisited = NULL;
    int unvisited_size = 0;
    unvisited = add_node_and_distance(unvisited, &unvisited_size, start_row * cols + start_col, 0);

 /*   while (unvisited_size > 0 && found == 0) {
        // Find the node with the minimum distance
        int min_index = 1;
        for (int i = 3; i < unvisited_size; i += 2) {
            if (unvisited[i] < unvisited[min_index]) {
                min_index = i;
            }
        }

        // Debugging, to be removed
        if (unvisited_size > debug_most_unvisited_nodes) {
            debug_most_unvisited_nodes = unvisited_size;
        }
        ////////////////////

        int current = unvisited[min_index - 1];
        int current_distance = unvisited[min_index];
        // Remove the current node from the unvisited list
        unvisited[min_index - 1] = unvisited[unvisited_size - 2];
        unvisited[min_index] = unvisited[unvisited_size - 1];
        unvisited_size -= 2; // potential memory leak here, should use realloc (?)

        int current_row = current / cols;
        int current_col = current % cols;

        if (maze[current_row][current_col] == TARGET) {
            break;  // Stop the algorithm if the target is reached
        }

        // Check and update four possible directions
        int direction_offsets[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        char direction_marks[4] = {DOWN, UP, RIGHT, LEFT};

        for (int i = 0; i < 4; i++) {
            int neighbor_row = current_row + direction_offsets[i][0];
            int neighbor_col = current_col + direction_offsets[i][1];
            if (neighbor_row >= 0 && neighbor_row < rows && neighbor_col >= 0 && neighbor_col < cols && (maze[neighbor_row][neighbor_col] == FREE_SPACE || maze[neighbor_row][neighbor_col] == TARGET))
            {
                if (maze[neighbor_row][neighbor_col] == TARGET) {
                    found = 1;
                    backtrack_path(maze, rows, cols, current_row, current_col);
                    break;
                } else if (maze[neighbor_row][neighbor_col] == FREE_SPACE)
                maze[neighbor_row][neighbor_col] = direction_marks[i];
                unvisited = add_node_and_distance(unvisited, &unvisited_size, neighbor_row * cols + neighbor_col, current_distance + 1);
            }
        }
    }
*/
    printf("Most unvisited nodes: %d\n", debug_most_unvisited_nodes);

    free(unvisited);
}

