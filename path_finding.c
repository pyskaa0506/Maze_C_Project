#include <stdio.h>
#include <stdlib.h>
#include "path_finding.h"
#include "move_chunk.h"

// Function to add a node and its distance to the unvisited list
int* add_node_and_distance(int* list, int* size, int node_index, int distance) {
    int* temp = realloc(list, (*size + 2) * sizeof(int));
    if (temp == NULL) {
        printf("Error reallocating memory.\n");
        free(list);
        return NULL;
    }
    list = temp;
    list[*size] = node_index;
    list[*size + 1] = distance;
    (*size) += 2;
    return list;
}

void backtrack_path(char*** current_chunk, int rows, int cols, int current_row, int current_col, int* loaded_chunk_number, int chunk_row_counter, int how_many_chunks)
{
    int number_of_steps = 0;
    FILE* file = fopen("../tmp/steps.txt", "w");

    while (find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks) != START) {
        number_of_steps+=1;
        char step = find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks);
        if (!file) {
            printf("Error opening file.\n");
            return;
        }
        fprintf(file, "%c", step);

        if (step == LEFT)
        {
            current_col -= 1;
        }
        else if (step == RIGHT)
        {
            current_col += 1;
        }
        else if (step == UP)
        {
            current_row -= 1;
        }
        else if (step == DOWN)
        {
            current_row += 1;
        }

        char new_char = find_and_return_char_from_chunk(loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, current_chunk, how_many_chunks);

        if (current_row < 0 || current_row >= rows || current_col < 0 || current_col >= cols) {
            printf("Error: Path goes out of bounds.\n");
            break;
        }
        else if (new_char == WALL) {
            printf("Error: Path goes through a wall.\n");
            break;
        }
        else if (new_char == PATH) {
            printf("Error: Path goes through itself.\n");
            break;
        }
        else if (new_char == START) {
            printf("(%d, %d)\n", current_row, current_col);
            break;
        }
    }

    if (!file) {
        printf("Error opening file.\n");
        return;
    }

    FILE* count_file = fopen("../tmp/steps_count.txt", "w");
    if (!count_file) {
        printf("Error opening file.\n");
        return;
    }
    fprintf(count_file, "%d", number_of_steps);

    fclose(file);
    fclose(count_file);
}




// Implementation of Dijkstra's Algorithm
void dijkstra(int rows, int cols, int start_row, int start_col, int16_t chunk_row_counter, int how_many_chunks)
{
    int loaded_chunk_number = 1;
    bool *loaded_chunks = malloc(how_many_chunks * sizeof(bool));
    for (int i = 0; i < how_many_chunks; i++) {
        loaded_chunks[i] = false;
    }
    char** current_chunk = read_maze("../chunks/1.txt", chunk_row_counter, cols);

    if (current_chunk == NULL) {
        printf("Error. Unable to read maze.\n");
        return;
    }

    char found = 0;
    int* unvisited = NULL;
    int unvisited_size = 0;
    unvisited = add_node_and_distance(unvisited, &unvisited_size, start_row * cols + start_col, 0);

    if (unvisited == NULL) {
        printf("Error. Unable to allocate memory.\n");
        free(current_chunk); // Zwolnienie pamiêci w przypadku b³êdu
        return;
    }

    while (unvisited_size > 0 && found == 0) {
        // Find the node with the minimum distance
        int min_index = 1;
        for (int i = 3; i < unvisited_size; i += 2) {
            if (unvisited[i] < unvisited[min_index]) {
                min_index = i;
            }
        }

        int current = unvisited[min_index - 1];
        int current_distance = unvisited[min_index];
        // Remove the current node from the unvisited list
        unvisited[min_index - 1] = unvisited[unvisited_size - 2];
        unvisited[min_index] = unvisited[unvisited_size - 1];
        unvisited_size -= 2;

        int current_row = current / cols;
        int current_col = current % cols;

        // current_row and current_col == TARGET
        if (find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, current_col, current_row, &current_chunk, how_many_chunks)
                    == TARGET) {
            break;  // Stop the algorithm if the target is reached
        }

        // Check and update four possible directions
        int direction_offsets[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        char direction_marks[4] = {DOWN, UP, RIGHT, LEFT};

        for (int i = 0; i < 4; i++) {
            int neighbor_row = current_row + direction_offsets[i][0];
            int neighbor_col = current_col + direction_offsets[i][1];
            char neighbor_char = find_and_return_char_from_chunk(&loaded_chunk_number, chunk_row_counter, cols, neighbor_col, neighbor_row, &current_chunk, how_many_chunks);
            if (neighbor_row >= 0 && neighbor_row < rows && neighbor_col >= 0 && neighbor_col < cols && (neighbor_char == FREE_SPACE || neighbor_char == TARGET))
            {
                if (neighbor_char == TARGET) {
                    found = 1;
                    backtrack_path(&current_chunk, rows, cols, current_row, current_col, &loaded_chunk_number, chunk_row_counter, how_many_chunks);
                    break;
                }
                else
                {
                    // mark the direction you came from into the neighbor cell
                    find_and_save_char_into_chunk(direction_marks[i], &loaded_chunk_number, chunk_row_counter, cols,
                                                  neighbor_col, neighbor_row, &current_chunk, how_many_chunks);
                    unvisited = add_node_and_distance(unvisited, &unvisited_size, neighbor_row * cols + neighbor_col,
                                                      current_distance + 1);
                }
            }
        }
        if(loaded_chunks[loaded_chunk_number - 1] == false)
        {
            loaded_chunks[loaded_chunk_number - 1] = true;
            printf("Loaded chunk %d/%d\n", loaded_chunk_number, how_many_chunks);
        }
    }


    free(unvisited);
    for (int i = 0; i < chunk_row_counter; i++) {
        free(current_chunk[i]);
    }
    free(current_chunk);
}

