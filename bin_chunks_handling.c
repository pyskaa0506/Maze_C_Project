#include "bin_chunks_handling.h"
#include "chunks_handling.h"

void write_code_world_to_file(FILE *file, unsigned char wall_or_path, unsigned char how_many, int col, unsigned int *how_many_already_written, int P_X, int P_Y, int K_X, int K_Y, unsigned int *how_many_rows_written) {
    for (int i = 0; i < how_many + 1; i++) {
        if ((*how_many_rows_written == P_Y) && (*how_many_already_written == P_X-1)) {
            fwrite("P", sizeof(char), 1, file);
            (*how_many_already_written)++;
        } else if ((*how_many_rows_written == K_Y) && (*how_many_already_written == K_X-1)){
            fwrite("K", sizeof(char), 1, file);
            (*how_many_already_written)++;
        }
        else {
            fwrite(&wall_or_path, sizeof(char), 1, file);
            (*how_many_already_written)++;
        }
        if (*how_many_already_written == col) {
            fwrite("\n", sizeof(char), 1, file);
            *how_many_already_written = 0;
            (*how_many_rows_written)++;
        }
    }
}
int read_bin_file(char *filepath, int chunk_row_counter) {

    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Error: File not found.\n");
        exit(EXIT_FAILURE);
    }

    int32_t fileID;
    int8_t esc;
    int16_t col;
    int16_t row;
    int16_t P_X;
    int16_t P_Y;
    int16_t K_X;
    int16_t K_Y;
    int32_t counter;
    int32_t solution_offset;
    char separator;
    char wall;
    char path;

    fread(&fileID, sizeof(int32_t), 1, file);
    fread(&esc, sizeof(int8_t), 1, file);
    fread(&col, sizeof(int16_t), 1, file);
    fread(&row, sizeof(int16_t), 1, file);
    fread(&P_X, sizeof(int16_t), 1, file);
    fread(&P_Y, sizeof(int16_t), 1, file);
    fread(&K_X, sizeof(int16_t), 1, file);
    fread(&K_Y, sizeof(int16_t), 1, file);
    fseek(file, 12, SEEK_CUR);
    fread(&counter, sizeof(int32_t), 1, file);
    fread(&solution_offset, sizeof(int32_t), 1, file);
    fread(&separator, sizeof(char), 1, file);
    fread(&wall, sizeof(char), 1, file);
    fread(&path, sizeof(char), 1, file);


    printf("File ID: %d\n", fileID);
    printf("ESC: %d\n", esc);
    printf("Col: %d\n", col);
    printf("Row: %d\n", row);
    printf("P_X: %d\n", P_X);
    printf("P_Y: %d\n", P_Y);
    printf("K_X: %d\n", K_X);
    printf("K_Y: %d\n", K_Y);
    printf("Counter: %d\n", counter);
    printf("Solution offset: %d\n", solution_offset);
    printf("Separator: %d\n", separator);
    printf("Wall: %d\n", wall);
    printf("Path: %d\n", path);

    FILE *file_output = fopen("../default_maps/bin_output.txt", "w");
    unsigned int how_many_already_written = 0;
    unsigned int how_many_rows_written = 1;

    unsigned char *buffer = (unsigned char *)malloc(sizeof(unsigned char) * 3);
        if (buffer == NULL) {
        printf("Error: Memory allocation failed.\n");
        fclose(file); 
        exit(EXIT_FAILURE);
        }

        for (int i = 0; i < counter; i++) {
            fread(buffer, sizeof(unsigned char), 3, file);
            if (buffer[0] == separator && (buffer[1] == wall || buffer[1] == path)) {
            write_code_world_to_file(file_output, buffer[1], buffer[2], col, &how_many_already_written, P_X, P_Y, K_X, K_Y, &how_many_rows_written);
         } 
        else {
        printf("Error: Input file is incorrect. Terminating program.\n");
        exit(EXIT_FAILURE);
        }
        }

    free(buffer); // Zwolnienie bufora po zako?czeniu p?tli

    fclose(file);
    fclose(file_output);
    return txt_file_to_txt_chunks("../default_maps/bin_output.txt", (int16_t)((col - 1) / 2), (int16_t)((row - 1) / 2), chunk_row_counter);
}
