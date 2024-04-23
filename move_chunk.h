
#ifndef MAZE_BIN_MOVE_CHUNK_H
#define MAZE_BIN_MOVE_CHUNK_H
char** read_maze(const char* filepath, int rows, int cols);
char find_and_return_char_from_chunk(int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char ***current_chunk, int how_many_chunks);
void find_and_save_char_into_chunk(char new_char, int *loaded_chunk_number, int chunk_row_counter, int cols, int current_col, int current_row, char ***current_chunk, int how_many_chunks);
#endif //MAZE_BIN_MOVE_CHUNK_H
