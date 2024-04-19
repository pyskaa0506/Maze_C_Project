#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <stdbool.h>
#include <stdint.h>

#define WALL '#'
#define FREE_SPACE ' '
#define UP 'A'
#define DOWN 'B'
#define LEFT 'C'
#define RIGHT 'D'
#define START 'P'  // Starting point
#define TARGET 'K'  // Target point
#define PATH '#' // Path

// Function to add a node and its distance to the unvisited list
int* add_node_and_distance(int* list, int* size, int node_index, int distance);

// Read the maze from a file
char** read_maze(const char* filepath, int rows, int cols);

// Save the maze to a file
void save_maze(const char* filepath, char** maze, int rows, int cols);

void backtrack_path(char** maze, int rows, int cols, int target_row, int target_col);

// Implementation of Dijkstra's Algorithm
void dijkstra(char** maze, int rows, int cols, int start_row, int start_col);

#endif /* PATH_FINDING_H */
