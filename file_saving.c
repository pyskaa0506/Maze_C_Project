#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <ctype.h>
#include "file_saving.h"


void reverse_file(const char *input_file, const char *output_file, int num_letters) {
    FILE *input_fp = fopen(input_file, "r");
    FILE *output_fp = fopen(output_file, "w");

    if (input_fp == NULL || output_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fseek(input_fp, -1, SEEK_END);

    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(input_fp);
        fputc(ch, output_fp);
        fseek(input_fp, -2, SEEK_CUR);
    }

    // Close files
    fclose(input_fp);
    fclose(output_fp);
}
//changes L to R, U to D, D to U, R to L
void change_directions(char *input_file, char *output_file, int num_letters){
    FILE *input_fp = fopen(input_file, "r");
    FILE *output_fp = fopen(output_file, "w");

    if (input_fp == NULL || output_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(input_fp);
        if (ch == 'L') {
            fputc('R', output_fp);
        } else if (ch == 'R') {
            fputc('L', output_fp);
        } else if (ch == 'U') {
            fputc('D', output_fp);
        } else if (ch == 'D') {
            fputc('U', output_fp);
        }
    }

    // Close files
    fclose(input_fp);
    fclose(output_fp);

}
int bin_compress(const char *input_file, int num_letters, const char *final_output) {
    FILE *output_fp = fopen(input_file, "r");
    FILE *final_fp = fopen(final_output, "w");

    if (output_fp == NULL || final_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    int code_words = 1;
    char prev_char = fgetc(output_fp);
    int count = 1;
    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(output_fp);
        if (ch == prev_char) {
            count++;
        } else {
            fprintf(final_fp, "%c%d", prev_char, count);
            prev_char = ch;
            count = 1;
            code_words++;
        }
    }

    // Write the last character
    fprintf(final_fp, "%c%d", prev_char, count);

    // Close files
    fclose(output_fp);
    fclose(final_fp);
    return code_words;
}


// this function compresses characters like U 2 L 3 to UULLL
void txt_compress(const char *input_file, int num_letters, const char *final_output) {
    FILE *output_fp = fopen(input_file, "r");
    FILE *final_fp = fopen(final_output, "w");

    if (output_fp == NULL || final_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char prev_char = fgetc(output_fp);
    int count = 1;
    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(output_fp);
        if (ch == prev_char) {
            count++;
        } else {
            if (count > 1) {
//                fprintf(final_fp, "%c %d\n", prev_char, count);
                fprintf(final_fp, "FOWARD %d\n", count);
            } else {
//                fprintf(final_fp, "%c %d\n", prev_char, count);
                fprintf(final_fp, "FOWARD 1\n");
            }
            if (prev_char == 'U') {
                if (ch == 'L') {
                    fprintf(final_fp, "TURNLEFT\n");
                } else {
                    fprintf(final_fp, "TURNRIGHT\n");
                }
            } else if (prev_char == 'L') {
                if (ch == 'U') {
                    fprintf(final_fp, "TURNRIGHT\n");
                } else {
                    fprintf(final_fp, "TURNLEFT\n");
                }
            } else if (prev_char == 'D') {
                if (ch == 'L') {
                    fprintf(final_fp, "TURNRIGHT\n");
                } else {
                    fprintf(final_fp, "TURNLEFT\n");
                }
            } else if (prev_char == 'R') {
                if (ch == 'U') {
                    fprintf(final_fp, "TURNLEFT\n");
                } else {
                    fprintf(final_fp, "TURNRIGHT\n");
                }
            }
            prev_char = ch;
            count = 1;
        }
    }

    // Write the last character
    if (count > 1) {
        fprintf(final_fp, "FOWARD %d\n", count);
    } else {
        fprintf(final_fp, "FOWARD 1\n");
    }

    // Close files
    fclose(output_fp);
    fclose(final_fp);
}

//adds a character to the end of the file
void add_char_to_end(const char *input_file, char ch){
    FILE *input_fp = fopen(input_file, "a");

    if (input_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    fputc(ch, input_fp);

    // Close files
    fclose(input_fp);

}

void print_file_to_console(const char *input_file) {
    FILE *input_fp = fopen(input_file, "r");

    if (input_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(input_fp)) != EOF) {
        printf("%c", ch);
    }

    // Close files
    fclose(input_fp);
}

void copy_file(const char *input_file, const char *output_file, const char *path) {

    // adds the path to the output file
    char output_file_path[100];
    sprintf(output_file_path, "%s/%s", path, output_file);

    FILE *input_fp = fopen(input_file, "r");
    FILE *output_fp = fopen(output_file_path, "w");

    if (input_fp == NULL || output_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(input_fp)) != EOF) {
        fputc(ch, output_fp);
    }

    // Close files
    fclose(input_fp);
    fclose(output_fp);
}

void copy_file_bin(const char *original_file, const char *copied_file, const char *destination_path) {
    FILE *original_fp = fopen(original_file, "rb"); // Open original file for reading in binary mode
    if (!original_fp) {
        perror("Error opening original file");
        return;
    }

    // Construct the full path for the copied file in the destination folder
    char full_path[FILENAME_MAX];
    snprintf(full_path, sizeof(full_path), "%s/%s", destination_path, copied_file);

    FILE *copied_fp = fopen(full_path, "wb"); // Open or create destination file for writing in binary mode
    if (!copied_fp) {
        perror("Error opening copied file");
        fclose(original_fp);
        return;
    }

    // Read and write data in blocks
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), original_fp)) > 0) {
        fwrite(buffer, 1, bytes_read, copied_fp);
    }

    // Close files
    fclose(original_fp);
    fclose(copied_fp);
}

void print_from_offset(FILE *file, int offset){
    fseek(file, offset, SEEK_SET);
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch);
    }
}

void modify_bin_file(char *input_bin_file, int code_words, char *output_file){
    printf("Codewords: %d\n", code_words);
    FILE *file = fopen(input_bin_file, "r+b");
    if (file == NULL) {
        perror("Error");
        return;
    }

    // Seek to the position of counter
    fseek(file, 29, SEEK_SET);

    int32_t counter;
    int32_t solution_offset;
    fread(&counter, sizeof(int32_t), 1, file);
    fread(&solution_offset, sizeof(int32_t), 1, file);
    printf("Counter: %d\n", counter);
    printf("Solution offset: %d\n", solution_offset);

    // Calculate new_solution_offset
    int32_t new_solution_offset = counter * 3 + 33;
    printf("New solution offset: %d\n", new_solution_offset);

    // Write new_solution_offset to the file
    fseek(file, 29 + sizeof(int32_t), SEEK_SET);
    fwrite(&new_solution_offset, sizeof(int32_t), 1, file);
    fseek(file, 29 + sizeof(int32_t), SEEK_SET);
    fread(&solution_offset, sizeof(int32_t), 1, file);
//    printf("Solution offset: %d\n", solution_offset);

    // Move the file pointer to the new solution_offset position
    fseek(file, new_solution_offset, SEEK_SET);

    //write solution id at start of offset 0x52524243
    unsigned int solution_id = 0x52524243;
    fwrite(&solution_id, sizeof(unsigned int), 1, file);
    fseek(file, new_solution_offset, SEEK_SET);
    fread(&solution_id, sizeof(unsigned int), 1, file);
//    printf("Solution id: %d\n", solution_id);

    //write number of code words
    fseek(file, 0, SEEK_END);
    fwrite(&code_words, sizeof(int32_t), 1, file);
    fseek(file, -4, SEEK_CUR);
    fread(&code_words, sizeof(int32_t), 1, file);
//    printf("Code words: %d\n", code_words);

    fseek(file, 0, SEEK_END);
    FILE *output_fp = fopen(output_file, "r");
    if (output_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    unsigned char ch;
    unsigned int num;
    while (fscanf(output_fp, "%c%d", &ch, &num) == 2) {
        // Write the character to the output file
        fwrite(&ch, sizeof(char), 1, file);

        // Write the number as binary to the output file
        fwrite(&num, sizeof(char), 1, file);
    }
    fclose(output_fp);
    fclose(file);
}