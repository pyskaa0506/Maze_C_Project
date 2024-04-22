#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <stdbool.h>
#include <stdint.h>

#define WALL 'X'
#define FREE_SPACE ' '
#define UP 'U'
#define DOWN 'D'
#define LEFT 'L'
#define RIGHT 'R'
#define START 'P'
#define TARGET 'K'
#define PATH '#'

// Function to add a node and its distance to the unvisited list
int* add_node_and_distance(int* list, int* size, int node_index, int distance);

// Read the maze from a file
char** read_maze(const char* filepath, int rows, int cols);

void backtrack_path(char*** current_chunk, int rows, int cols, int current_row, int current_col, int* loaded_chunk_number, int chunk_row_counter, int how_many_chunks);

// Implementation of Dijkstra's Algorithm
void dijkstra(int rows, int cols, int start_row, int start_col, int16_t chunk_row_counter, int how_many_chunks);

#endif /* PATH_FINDING_H */
