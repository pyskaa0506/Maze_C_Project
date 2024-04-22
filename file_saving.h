
#ifndef MAZE_BIN_FILE_SAVING_H
#define MAZE_BIN_FILE_SAVING_H
void modify_bin_file(char *input_bin_file, int code_words, char *output_file);
int compress_for_bin(const char *input_file, char starting_pos, int num_letters, const char *final_output);
void reverse_file(const char *input_file, const char *output_file, int num_letters);
void change_directions_to_instructions_and_compress(const char *input_file, char starting_pos, int num_letters, const char *final_output);
#endif //MAZE_BIN_FILE_SAVING_H
