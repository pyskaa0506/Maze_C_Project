#include "chunks_handling.h"

void txt_file_to_txt_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size) {
    chunk_row_size *= 2;
    delete_files_in_directory("../chunks");
    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file\n");
        return;
    }

    unsigned int *buffer = (unsigned int *)malloc(sizeof(unsigned int) * chunk_row_size * (col * 2 + 1));
    if (buffer == NULL){
        fprintf(stderr,"Error: Couldn't allocate memory\n");
        exit(1);
    }

    int chunkCount = 1;
    while (!feof(file)) {
        char filename[10];
        sprintf(filename, "../chunks/%d.txt", chunkCount);

        FILE *new_file = fopen(filename, "w");
        if (!new_file) {
            fprintf(stderr, "Error: Couldn't open the file\n");
            fclose(file);
            free(buffer);
            return;
        }

        // Read chunk_row_size lines from the file
        int linesRead = 0;
        while (linesRead < chunk_row_size && fgets((char *)(buffer + linesRead * (col * 2 + 1)), col * 2 + 1, file)) {
            fprintf(new_file, "%s", (char *)(buffer + linesRead * (col * 2 + 1)));
            linesRead++;
        }
        fclose(new_file);

        chunkCount++;
    }
    free(buffer);
    fclose(file);
}

int txt_file_to_bin_chunks(char *filepath, int16_t col, int16_t row, int16_t chunk_row_size) {
    delete_files_in_directory("../chunks");
    int16_t cols_len_in_file = col * 2 + 1;
    chunk_row_size *= 2;
    char *line_buffer = (char *)malloc(sizeof(char) * (cols_len_in_file + 2)); // Include space for newline and null terminator
    if (line_buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return -1;
    }

    const char *directory_path = "../chunks";
    char filename[20];

    FILE *binary_file = NULL;
    int linesRead = 0;
    int chunkCount = 1;

    FILE *file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Error: Couldn't open the file\n");
        free(line_buffer);
        return -1;
    }

    while (fgets(line_buffer, cols_len_in_file + 2, file) != NULL) {
        if (binary_file == NULL || linesRead >= chunk_row_size) {
            if (binary_file != NULL) {
                fclose(binary_file);
            }
            snprintf(filename, sizeof(filename), "%s/%d.bin", directory_path, chunkCount);
            binary_file = fopen(filename, "wb");
            if (!binary_file) {
                fprintf(stderr, "Error: Failed to create binary output file\n");
                free(line_buffer);
                fclose(file);
                return -1;
            }
            linesRead = 0;
            chunkCount++;
        }

        uint8_t compressed[cols_len_in_file / 8 + 1]; // +1 in case the length is not a multiple of 8
        compress_data(line_buffer, cols_len_in_file, compressed);
        write_compressed_data_to_file(compressed, cols_len_in_file / 8 + 1, binary_file);
        linesRead++;
    }

    if (binary_file != NULL) {
        fclose(binary_file);
    }

    free(line_buffer);
    fclose(file);
    return (int)(chunkCount - 1);
}

//function that converts a character to a bit (X to 1, anything else to 0)
uint8_t convert_char_to_bit(char c) {
    if (c == 'X') {
        return 1;
    } else if (c == ' ' || c == 'K' || c == 'P'){
        return 0;
    }
    return 0;
}

//function that compresses data
void compress_data(char *line, int length, uint8_t *compressed) {
    for (int i = 0; i < length; i += 8) {
        uint8_t byte = 0;
        for (int j = 0; j < 8 && i + j < length; j++) {
            byte |= convert_char_to_bit(line[i + j]) << (7 - j);
        }
        compressed[i / 8] = byte;
    }
}

//function that prints compressed data
void print_compressed_data(uint8_t *compressed, int length) {
    for (int i = 0; i < length; i++) {
        for (int j = 7; j >= 0; j--) {
            printf("%d", (compressed[i] >> j) & 1);
        }
        printf(" ");
    }
    printf("\n");
}
//function that writes compressed data to a file
void write_compressed_data_to_file(uint8_t *compressed, int length, FILE *binary_file) {
    if (binary_file == NULL) {
        fprintf(stderr, "Error: File pointer is NULL\n");
        return;
    }

    size_t bytes_written = fwrite(compressed, sizeof(uint8_t), length, binary_file);
    if (bytes_written != length) {
        fprintf(stderr, "Error: Failed to write all compressed data to file\n");

        if (ferror(binary_file)) {
            perror("fwrite");
        }
    }
}


void delete_files_in_directory(const char *directory_path) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directory_path);
    if (dir == NULL) {
        fprintf(stderr, "Error: opening directory failed %s\n", directory_path);
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", directory_path, entry->d_name);

            if (remove(file_path) != 0) {
                fprintf(stderr, "Error: deleting file failed: %s\n", file_path);
            } else {
                printf("Deleted file: %s\n", file_path);
            }
        }
    }
    closedir(dir);
}

//function that increases/decreases number in filenames like "1.bin"
//char *change_chunk_nuber(char operation, const char *act_file) {
//    char *num_extension = strchr(act_file, '.');
//    int num_length = num_extension - act_file;
//    char *num = malloc(num_length + 1);
//    if (num == NULL) {
//        fprintf(stderr, "Error: Memory allocation failed\n");
//        return NULL;
//    }
//
//    strncpy(num, act_file, num_length);
//    num[num_length] = '\0';
//
//    int num_value = atoi(num);
//
//    if (operation == '+')
//        num_value++;
//    else if (operation == '-' && num_value > 0)
//        num_value--;
//    else {
//        fprintf(stderr, "Error: Invalid operation or number cannot be decreased further\n");
//        free(num); // Zwolnienie pamięci przed zwróceniem NULL
//        return NULL;
//    }
//
//    char *new_filename = malloc(strlen(act_file) + 1);
//    if (new_filename == NULL) {
//        fprintf(stderr, "Error: Memory allocation failed\n");
//        free(num); // Zwolnienie pamięci przed zwróceniem NULL
//        return NULL;
//    }
//    sprintf(new_filename, "%d%s", num_value, num_extension);
//
//    free(num); // Zwolnienie pamięci używanej dla num
//    return new_filename;
//}