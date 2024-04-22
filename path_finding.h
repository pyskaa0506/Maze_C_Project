#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <stdbool.h>
#include <stdint.h>

#define WALL 'X'
#define FREE_SPACE ' '
#define UP 'A'
#define DOWN 'B'
#define LEFT 'C'
#define RIGHT 'D'
#define START 'P'
#define TARGET 'K'
#define PATH '#'

// Function to add a node and its distance to the unvisited list
int* add_node_and_distance(int* list, int* size, int node_index, int distance);

// Read the maze from a file
char** read_maze(const char* filepath, int rows, int cols);

void backtrack_path(char** maze, int rows, int cols, int target_row, int target_col);

// Implementation of Dijkstra's Algorithm
void dijkstra(int rows, int cols, int start_row, int start_col, char *filepath, int16_t chunk_row_counter, int how_many_chunks);

#endif /* PATH_FINDING_H */
