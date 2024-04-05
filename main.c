#include "chunks_handling.h"

int main(int argc, char *argv[]) {
    char *filepath = "../default_maps/25x50_20.txt";
    int16_t col = 25;
    int16_t row = 50; //remember it is 2 * row + 1 in file
    int16_t chunk_row_size = 40; // How many rows should be packed into a chunk (file)
    txt_file_to_bin_chunks(filepath, col, row, chunk_row_size);
    return 0;
}
