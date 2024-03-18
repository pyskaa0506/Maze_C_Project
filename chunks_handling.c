#include "chunks_handling.h"

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
void write_compressed_data_to_file(uint8_t *compressed, int length, FILE *binaryFile) {
    fwrite(compressed, sizeof(uint8_t), length, binaryFile);
}

//function that increases/decreases number in filenames like "1.bin"
char *chng_chnk_nbr(char operation, const char *act_file) {
    char *num_extension = strchr(act_file, '.');
    int num_length = num_extension - act_file;
    char *num = malloc(num_length + 1);
    if (num == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return NULL;
    }

    strncpy(num, act_file, num_length);
    num[num_length] = '\0';

    int num_value = atoi(num);

    if (operation == '+')
        num_value++;
    else if (operation == '-' && num_value > 0)
        num_value--;
    else {
        fprintf(stderr, "Error: Invalid operation or number cannot be decreased further\n");
        free(num); // Zwolnienie pamięci przed zwróceniem NULL
        return NULL;
    }

    char *new_filename = malloc(strlen(act_file) + 1);
    if (new_filename == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(num); // Zwolnienie pamięci przed zwróceniem NULL
        return NULL;
    }
    sprintf(new_filename, "%d%s", num_value, num_extension);

    free(num); // Zwolnienie pamięci używanej dla num
    return new_filename;
}

void delete_files_in_directory(const char *directoryPath) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(directoryPath);
    if (dir == NULL) {
        fprintf(stderr, "Error: opening directory failed %s\n", directoryPath);
        return;
    }
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", directoryPath, entry->d_name);

            if (remove(filePath) != 0) {
                fprintf(stderr, "Error: deleting file failed: %s\n", filePath);
            } else {
                printf("Deleted file: %s\n", filePath);
            }
        }
    }
    closedir(dir);
}