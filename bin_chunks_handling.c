//
// Created by pyska on 11.04.2024.
//

#include "bin_chunks_handling.h"
void read_bin_file(char *filepath, int chunk_row_counter) {

    FILE *file = fopen(filepath, "rb");
    if (file == NULL) {
        printf("Error: File not found.\n");
        return;
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

    unsigned int wall_and_path_counter = 0;

    for (int i = 0; i < counter; i++) {
        unsigned char *buffer = (char *) malloc(sizeof(char) * 3);
        fread(buffer, sizeof(char), 3, file);
        if (buffer[0] == separator && (buffer[1] == wall || buffer[1] == path)) {
            printf("Correct format.\n");
        } else {
            printf("Incorrect format.\n");
        }
        printf("Buffer  %d: %d %d %d\n", i , buffer[0], buffer[1], buffer[2]);
    }
    fclose(file);
}
