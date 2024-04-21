#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "chunks_handling.h"
#include "move_chunck.h"
#include "path_finding.h"

bool move_to_next_chunk(char** current_chunk, int16_t chunk_row_size, int16_t chunk_col_size, int16_t current_chunk_index, int16_t total_chunks, char direction) {
    if (direction == 'R') {
        if (current_chunk_index < total_chunks) {
            current_chunk_index++;
            char next_chunk_path[20];
            sprintf(next_chunk_path, "chunk%d.txt", current_chunk_index);
            char** next_chunk = read_txt_chunk(next_chunk_path, chunk_col_size, chunk_row_size);
            if (next_chunk == NULL) {
                printf("B³¹d wczytywania kolejnego chunku.\n");
                return false;
            } 
            else 
            {
                // zwolnienie pamieci poprzedniego kierunku
                for (int i = 0; i < chunk_row_size; i++) {
                    free(current_chunk[i]);
                }
                free(current_chunk);
                current_chunk = next_chunk;
                return true;
            }
        } 
        else 
        {
            printf("Nie ma wiêcej chunków w prawo.\n");
            return false;
        }
    }

    else if (direction == 'L') {
        if (current_chunk_index > 1) {
            current_chunk_index--;
            char prev_chunk_path[20];
            sprintf(prev_chunk_path, "chunk%d.txt", current_chunk_index);
            char** prev_chunk = read_txt_chunk(prev_chunk_path, chunk_col_size, chunk_row_size); // Poprawione wywo³anie funkcji
            if (prev_chunk == NULL) {
                printf("B³¹d wczytywania poprzedniego chunku.\n");
                return false;
            } 
            else 
            {
                for (int i = 0; i < chunk_row_size; i++) {
                    free(current_chunk[i]);
                }
                free(current_chunk);
                current_chunk = prev_chunk;
                return true;
            }
        } 
        else 
        {
            printf("Nie ma wiêcej chunków w lewo.\n");
            return false;
        }
    }

    else if (direction == 'U') 
    {
    if (current_chunk_index + total_chunks <= total_chunks * 2) {
    current_chunk_index += total_chunks;
    char up_chunk_path[20];
    sprintf(up_chunk_path, "chunk%d.txt", current_chunk_index);
    char** up_chunk = read_txt_chunk(up_chunk_path, chunk_col_size, chunk_row_size);
    if (up_chunk == NULL) 
    {
        printf("B³¹d wczytywania chunku powy¿ej.\n");
        return false;
    } 
    else 
    {
        for (int i = 0; i < chunk_row_size; i++) {
            free(current_chunk[i]);
        }

        free(current_chunk);
        current_chunk = up_chunk;
        return true;
    }
    } 
    else {
    printf("Nie ma wiêcej chunków u góry.\n");
    return false;
    }
    }

    else if (direction == 'D') 
    {
        if (current_chunk_index + total_chunks <= total_chunks * 2) {
    current_chunk_index += total_chunks;
    char down_chunk_path[20];
    sprintf(down_chunk_path, "chunk%d.txt", current_chunk_index);
    char** down_chunk = read_txt_chunk(down_chunk_path, chunk_col_size, chunk_row_size);
    if (down_chunk == NULL) {
        printf("B³¹d wczytywania chunku poni¿ej.\n");
        return false;
    } else 
    {
        for (int i = 0; i < chunk_row_size; i++) {
            free(current_chunk[i]);
        }
        free(current_chunk);
        current_chunk = down_chunk;
        return true;
    }
    } 
    else 
    {
    printf("Nie ma wiêcej chunków w dó³.\n");
    return false;
        }
    }


    return false; // false = nie uda³o siê przejœæ do kolejnego chunku
}


void find_exit(char* initial_chunk_path, int16_t chunk_row_size, int16_t chunk_col_size, int16_t total_chunks) {
    char** current_chunk = read_txt_chunk(initial_chunk_path, chunk_col_size, chunk_row_size);
    if (current_chunk == NULL) {
        printf("B³¹d wczytywania pierwszego chunku.\n");
        return;
    }

    int16_t current_chunk_index = 1; 
    int16_t target_row = -1; 
    int16_t target_col = -1;


    while (true) {

        dijkstra(current_chunk, chunk_row_size, chunk_col_size, 0, 0, "placeholder.txt");

        if (target_row != -1 && target_col != -1) {
            printf("Wyjœcie znaleziono w chunku %d.\n", current_chunk_index);
            break;
        }

        char direction = 'R'; 
        bool found_exit_in_next_chunk = false;

        while (direction != ' ') {
            found_exit_in_next_chunk = move_to_next_chunk(current_chunk, chunk_row_size, chunk_col_size, current_chunk_index, total_chunks, direction);
            if (found_exit_in_next_chunk) {
                if (target_row != -1 && target_col != -1) {
                    printf("Wyjœcie znaleziono w chunku %d.\n", current_chunk_index);
                    return;
                }

                break;
            }

            if (direction == 'R') {
                direction = 'L'; 
            } else if (direction == 'L') {
                direction = 'U'; 
            } else if (direction == 'U') {
                direction = 'D';
            } else if (direction == 'D') {
                printf("Nie znaleziono wyjœcia w ¿adnym z kierunków.\n");
                return;
            }
        }

        if (!found_exit_in_next_chunk) {
            printf("Nie znaleziono wyjœcia w ¿adnym z s¹siednich chunków.\n");
            return;
        }
    }
}

