#include "chunks_handling.h"


int main(int argc, char *argv[]) {
    char *filepath = "../default_maps/25x50_20.txt";
    int16_t col = 25;
    int16_t row = 50; //remember it is 2 * row + 1 in file
    int16_t chunk_rows_counter = 50; // How many rows should be packed into a chunk (file)
    int chunk_counter = txt_file_to_txt_chunks(filepath, col, row, chunk_rows_counter); //this separates the file into chunks and returns the number of chunks created


    //tutorial
    char *filepath_chunk = "../chunks/3.txt";
    char **chunk = read_txt_chunk(filepath_chunk, col, row, chunk_rows_counter); //this reads the first chunk and returns it as a 2D array that you can work on

    // and if you want to print every chunk char by char
    for (int i = 1; i <= chunk_counter; i++) {
        char filename[10];
        sprintf(filename, "../chunks/%d.txt", i);
        char **chunk = read_txt_chunk(filename, col, row, chunk_rows_counter);
        for (int j = 0; j < chunk_rows_counter; j++) {
            for (int k = 0; k < col * 2 + 1; k++) {
                printf("%c", chunk[j][k]);
            }
            printf("\n");
        }
    }

    return 0;
}
