#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "path_finding.h"
#include "chunks_handling.h"

#define MAX_QUEUE_SIZE 10000

typedef struct {
    int16_t x;
    int16_t y;
}Point;

typedef struct {
    Point point;
    int distance;
}QueueNode;

typedef struct {
    QueueNode *array;
    int front;
    int rear;
    int capacity;
} Queue;

typedef struct {
    int16_t x;
    int16_t y;
    int chunk_index;
} PlayerPosition;


Queue *createQueue(int capacity) 
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue -> capacity = capacity;
    queue -> front = queue -> rear = -1;
    queue -> array = (QueueNode *)malloc(queue -> capacity * sizeof(QueueNode));
    return queue;
}

bool isEmpty(Queue *queue) 
{
    return queue -> front == -1;
}

// dodaje element do kolejki 
void enqueue(Queue *queue, QueueNode item) 
{
    if ((queue -> rear + 1) % queue -> capacity == queue -> front)
        return;

    queue -> rear = (queue -> rear + 1) % queue -> capacity;
    queue -> array[queue -> rear] = item;

    if (queue -> front == -1){
        queue -> front = queue -> rear;
    }
}

//usuwa i zwraca pierwszy element z kolejki 
QueueNode dequeue(Queue *queue) {
    if (isEmpty(queue))
        return (QueueNode){0,0,0};
    QueueNode item = queue -> array[queue -> front];
    if (queue -> front == queue -> rear){
        queue -> front = queue -> rear = -1;
    }
    else{
        queue -> front = (queue -> front + 1) % queue -> capacity;
    }

    return item;

}


//czy znajduje sie w labiryncie dany punkt
bool isValid(Point point, int16_t col, int16_t row) {
    return (point.x >= 0 && point.x < col && point.y >= 0 && point.y < row);
}


bool is_valid_cell(const char *input_filename, int x, int y) 
{
    FILE *file = fopen(input_filename, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Error. Unable to open input file %s\n", input_filename);
        exit(EXIT_FAILURE);
    }

    char line[1024];
    int row_counter = 0;

    // przesuwa wskaznik pliku na odpowiednia linie
    while (fgets(line, sizeof(line), file) && row_counter < y) 
    {
        row_counter++;

    }

    if (line[x] == 'X' || line[x] == 'P' || line[x] == 'K' || line[x] == ' ') 
    {
        fclose(file);
        return true;
    } 
    else 
    {
        fclose(file);
        return false;
    }
}


bool is_valid_position(PlayerPosition position, int16_t col, int16_t row) {
    return (position.x >= 0 && position.x < col && position.y >= 0 && position.y < row);
}

bool is_within_current_chunk(PlayerPosition position, int16_t chunk_col, int16_t chunk_row) {
    return (position.x >= 0 && position.x < chunk_col && position.y >= 0 && position.y < chunk_row);
}

void move_to_next_chunk(PlayerPosition *position, int chunk_col) {
    position->chunk_index++;
    position->x = 0;
}

/*
// funkcja algorytmu Dijkstra 
char *find_path_in_current_chunk(const char *input_filename, int16_t col, int16_t row) 
{
    int16_t chunk_rows_counter = 50; // jaki musi by? rozmiar chunku?

    int chunk_counter = txt_file_to_txt_chunks(input_filename,col, row, chunk_rows_counter);

    // Tworzenie kolejki do bfsa
    Queue *queue =createQueue(MAX_QUEUE_SIZE);

    enqueue(queue, (QueueNode){{0, 0}, 0});

    // odwiedzone 
    bool **visited = (bool **)malloc(row * sizeof(bool *));
    for (int i = 0; i < row; i++) 
    {
        visited[i] = (bool *)calloc(col, sizeof(bool));
    }

    // BFS
    Point exit_point = {-1, -1};

    while (!isEmpty(queue)) {
        QueueNode current = dequeue(queue);
        Point current_point = current.point;
        int distance = current.distance;

        // czy jest wyj?ciem
        if (current_point.x == col - 1 && current_point.y == row - 1) {
            exit_point = current_point;
            break;
        }

        Point directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // left, tight, up and down
        for (int i = 0; i < 4; i++) {
            Point adjacent = {current_point.x + directions[i].x, current_point.y + directions[i].y};

            if (isValid(adjacent, col, row) && !visited[adjacent.y][adjacent.x] && is_valid_cell(input_filename, adjacent.x, adjacent.y)) 
            {
                enqueue(queue, (QueueNode){adjacent, distance + 1});
                visited[adjacent.y][adjacent.x] = true;
            }
        }
    }


    for (int i = 0; i < row; i++) {
        free(visited[i]);
    }
    free(visited);
    free(queue-> array);
    free(queue);

    char *path = NULL;
    if (exit_point.x != -1 && exit_point.y != -1) 
    {
        path = (char *)malloc(MAX_QUEUE_SIZE * sizeof(char));
        if (path == NULL) 
        {
            fprintf(stderr, "Error. Memory allocation failed for path\n");
            exit(EXIT_FAILURE);
        }

        Point current_point = exit_point;
        int path_length = 0;
        while (!(current_point.x == 0 && current_point.y == 0)) {
            Point directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; 
            for (int i = 0; i < 4; i++) {
                Point adjacent = {current_point.x + directions[i].x, current_point.y + directions[i].y};
                if (isValid(adjacent, col, row) && visited[adjacent.y][adjacent.x] && visited[adjacent.y][adjacent.x] == visited[current_point.y][current_point.x] - 1) 
                {
                    if (directions[i].x == 1)
                        path[path_length++] = 'R';
                    else if (directions[i].x == -1)
                        path[path_length++] = 'L';
                    else if (directions[i].y == 1)
                        path[path_length++] = 'D';
                    else if (directions[i].y == -1)
                        path[path_length++] = 'U';
                    current_point = adjacent;
                    break;
                }
            }
        }

        // odwrocenie sciezki (odpowiednia kolejnosc)
        for (int i = 0; i < path_length / 2; i++) {
            char temp = path[i];
            path[i] = path[path_length - i - 1];
            path[path_length - i - 1] = temp;
        }
        path[path_length] = '\0';
    }

    return path;
}
*/

char *find_path_with_chunk_movement(const char *input_filename, int16_t col, int16_t row) {
    int16_t chunk_rows_counter = 50;
    PlayerPosition player = {0, 0, 0};

    int chunk_counter = txt_file_to_txt_chunks(input_filename, col, row, chunk_rows_counter);
    Queue *queue = createQueue(MAX_QUEUE_SIZE);
    enqueue(queue, (QueueNode){{player.x, player.y}, 0});
    bool **visited = (bool **)malloc(row * sizeof(bool *));
    for (int i = 0; i < row; i++) {
        visited[i] = (bool *)calloc(col, sizeof(bool));
    }
    Point exit_point = {-1, -1};
    while (!isEmpty(queue)) {
        QueueNode current = dequeue(queue);
        Point current_point = current.point;
        int distance = current.distance;
        if (!is_within_current_chunk(player, col, chunk_rows_counter)) {
            move_to_next_chunk(&player, col);
            memset(visited, 0, row * sizeof(bool *));
            continue;
        }
        if (current_point.x == col - 1 && current_point.y == row - 1 && player.chunk_index == chunk_counter - 1) {
            exit_point = current_point;
            break;
        }
        if (current_point.x == col - 1 && current_point.y == chunk_rows_counter - 1 && player.chunk_index < chunk_counter - 1) {
            move_to_next_chunk(&player, col);
            memset(visited, 0, row * sizeof(bool *));
            continue;
        }
        Point directions[] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            Point adjacent = {current_point.x + directions[i].x, current_point.y + directions[i].y};
            if (isValid(adjacent, col, row) && !visited[adjacent.y][adjacent.x] && is_valid_cell(input_filename, adjacent.x, adjacent.y)) {
                enqueue(queue, (QueueNode){adjacent, distance + 1});
                visited[adjacent.y][adjacent.x] = true;
            }
        }
    }
    for (int i = 0; i < row; i++) {
        free(visited[i]);
    }
    free(visited);
    free(queue->array);
    free(queue);
    char *path = NULL;
    if (exit_point.x != -1 && exit_point.y != -1) {
        // Implementacja ?cie?ki
    }
    return path;
}


PathInfo find_path(const char *input_filename, int16_t col, int16_t row) {
    char *path = find_path_with_chunk_movement(input_filename, col, row);

    // zworecenie kirunki i sciezki
    PathInfo path_info;
    path_info.path = path;
    path_info.direction = ' '; // niepotrzebne, dlt ze kierunek jest zawarty w sciezce
    return path_info;
}

// okre?lenia kierunku na podstawie ostatniego ruchu w sciezce
char determine_next_direction(const char *path) {
    int length = strlen(path);
    if (length > 0) {
        return path[length - 1]; // kierunek jest zapisany na ko?cu ?cie?ki
    } else {
        return ' '; // domyslnie zwraca spacje (gdy brak przejscia)
    }
}

void save_path_to_file(const char *path) {
    FILE *file = fopen("temp_path.txt", "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file for writing.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", path);

    fclose(file);
}