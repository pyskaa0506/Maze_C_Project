#ifndef PATH_FINDING_H
#define PATH_FINDING_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char *path;
    char direction; // kierunki oznaczylam - U (up), D (down), L (left), R (right)
} PathInfo;

char determine_next_direction(const char *path);

PathInfo find_path(const char *input_filename, int16_t col, int16_t row);

bool is_valid_cell(const char *input_filename, int x, int y) ;

#endif /* PATH_FINDING_H */
