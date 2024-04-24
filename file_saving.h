
#ifndef MAZE_BIN_FILE_SAVING_H
#define MAZE_BIN_FILE_SAVING_H
void print_file_to_console(const char *input_file);
void add_char_to_end(const char *input_file, char ch);
void reverse_file(const char *input_file, const char *output_file, int num_letters);
void txt_compress(const char *input_file, int num_letters, const char *final_output);
int bin_compress(const char *input_file, int num_letters, const char *final_output);
void modify_bin_file(char *input_bin_file, int code_words, char *output_file);
void change_directions(char *input_file, char *output_file, int num_letters);
void copy_file(const char *input_file, const char *output_file, const char *path);
void copy_file_bin(const char *original_file, const char *copied_file, const char *destination_path);
#endif //MAZE_BIN_FILE_SAVING_H
