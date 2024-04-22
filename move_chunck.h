#ifndef MOVE_CHUNCK
#define MOVE_CHUNCK

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char** maze;
    int rows;
    int cols;
    
} Chunk;

// Funkcja wczytuj¹ca chunk z pliku
Chunk* load_chunk(const char* directory_path, int chunk_number);

// Funkcja zapisuj¹ca chunk do pliku
void save_chunk(const char* filepath, Chunk* chunk);

// Funkcja znajduj¹ca punkt wejœcia w danym chunku
void find_entry_point(Chunk* chunk, int* entry_row, int* entry_col);

void dijkstra_wrapper(Chunk* current_chunk, int global_start_row, int global_start_col, int global_target_row, int global_target_col, char* maze_filepath);

void convert_coordinates(Chunk* current_chunk, int global_row, int global_col, int* local_row, int* local_col);

Chunk* find_chunk_by_coordinates(const char* directory_path, int global_row, int global_col);

//free chunk 157

#endif /* MOVE_CHUNCK*/

