#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
            fprintf(final_fp, "%c %d", prev_char, count);
            prev_char = ch;
            count = 1;
            code_words++;
        }
    }

    // Write the last character
    fprintf(final_fp, "%c %d", prev_char, count);

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

void modify_bin_file(char *input_bin_file, int code_words, char *output_file){
    FILE *file = fopen(input_bin_file, "r+b");
    fseek(file, 29, SEEK_CUR);
    int32_t counter;
    int32_t solution_offset;
    fread(&counter, sizeof(int32_t), 1, file);
    fread(&solution_offset, sizeof(int32_t), 1, file);
    printf("Counter: %d\n", counter);
    printf("Solution offset: %d\n", solution_offset);

    fseek(file, -4, SEEK_CUR);
    int32_t new_solution_offset = counter * 3 + 29;
    fwrite(&new_solution_offset, sizeof(int32_t), 1, file);

    fseek(file, 0, SEEK_SET);
    fseek(file, solution_offset, SEEK_CUR);

    //write solution id at start of offset 0x52524243
    int32_t solution_id = 0x52524243;
    fwrite(&solution_id, sizeof(int32_t), 1, file);
    //write number of code words
    fwrite(&code_words, sizeof(int32_t), 1, file);


    FILE *output_fp = fopen(output_file, "r");
    if (output_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }
    char ch;
    while ((ch = fgetc(output_fp)) !=EOF) {
        fwrite(&ch, sizeof(char), 1, file);
    }
    fclose(output_fp);
    fclose(file);
}