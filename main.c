#include <stdio.h>
#include <stdlib.h>
#include "chunks_handling.h"




int main(int argc, char *argv[]) {
    char *filepath = "../default_maps/150x100_0.txt";
    int16_t col = 150;
    int16_t row = 100;
    int16_t chunk_row_size = 50; // How many rows should be packed into a chunk (file)
    txt_file_to_bin_chunks(filepath, col, row, chunk_row_size);
    return 0;
}