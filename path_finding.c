#include <stdio.h>
#include <stdlib.h>
#include "path_finding.h"

// Function to add a node and its distance to the unvisited list
int* add_node_and_distance(int* list, int* size, int node_index, int distance) {
    int* temp = realloc(list, (*size + 2) * sizeof(int));
    if (temp == NULL) {
        printf("Error reallocating memory.\n");
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

// Save the maze to a file
void save_maze(const char* filepath, char** maze, int rows, int cols) {
    FILE* file = fopen(filepath, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < rows; i++) {
        fputs(maze[i], file);
        fputc('\n', file);  // Add a newline after each row
    }

    fclose(file);
}

void backtrack_path(char** maze, int rows, int cols, int target_row, int target_col) {
    int current_row = target_row;
    int current_col = target_col;

    // WARNING !!!
    // a really dumb way to get the original maze
    // it would be better to save copy of each labirynt path, load and save it there
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
        printf("%s", maze_original[i]);
    }

    // Free the memory allocated for maze_original
    for (int i = 0; i < rows; i++) {
        free(maze_original[i]);
    }
    free(maze_original);
}


// Implementation of Dijkstra's Algorithm
void dijkstra(char** maze, int rows, int cols, int start_row, int start_col) {

    // Debugging variables
    int debug_most_unvisited_nodes = 0;
    /////////////


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
        unvisited_size -= 2;

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

    printf("Most unvisited nodes: %d\n", debug_most_unvisited_nodes);

    free(unvisited);
}

