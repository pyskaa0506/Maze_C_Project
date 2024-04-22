#include <stdio.h>
#include <stdlib.h>
#include "file_saving.h"


void reverse_file(const char *input_file, const char *output_file, int num_letters) {
    FILE *input_fp = fopen(input_file, "r");
    FILE *output_fp = fopen(output_file, "w");

    if (input_fp == NULL || output_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Move file pointer to the end
    fseek(input_fp, -1, SEEK_END);

    // Write characters in reverse order to the output file
    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(input_fp);
        fputc(ch, output_fp);
        fseek(input_fp, -2, SEEK_CUR); // Move the pointer to the previous character
    }

    // Close files
    fclose(input_fp);
    fclose(output_fp);
}

// this function compresses characters like U 2 L 3 to UULLL
void change_directions_to_instructions_and_compress(const char *input_file, char starting_pos, int num_letters, const char *final_output) {
    FILE *output_fp = fopen(input_file, "r");
    FILE *final_fp = fopen(final_output, "w");

    if (output_fp == NULL || final_fp == NULL) {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    char prev_char = starting_pos;
    int count = 1;
    for (int i = 0; i < num_letters; i++) {
        char ch = fgetc(output_fp);
        if (ch == prev_char) {
            count++;
        } else {
            if (count > 1) {
                fprintf(final_fp, "FOWARD %d\n", count);
            } else {
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
