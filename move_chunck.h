#ifndef MOVE_CHUNCK
#define MOVE_CHUNCK

#include <stdbool.h>
#include <stdint.h>

#define START 'P'
#define TARGET 'K'

bool move_to_next_chunk(char** current_chunk, int16_t chunk_row_size, int16_t chunk_col_size, int16_t current_chunk_index, int16_t total_chunks, char direction);

void find_exit(char* initial_chunk_path, int16_t chunk_row_size, int16_t chunk_col_size, int16_t total_chunks);

#endif /* MOVE_CHUNCK*/

