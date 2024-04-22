
#ifndef MAZE_BIN_FILE_SAVING_H
#define MAZE_BIN_FILE_SAVING_H

void reverse_file(const char *input_file, const char *output_file, int num_letters);
void change_directions_to_instructions_and_compress(const char *input_file, char starting_pos, int num_letters, const char *final_output);
#endif //MAZE_BIN_FILE_SAVING_H
