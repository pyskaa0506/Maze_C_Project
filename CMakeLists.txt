cmake_minimum_required(VERSION 3.26)
project(Maze_C_Project C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Maze_C_Project
        main.c
        file_loading.c
        file_loading.h
        chunks_handling.h
        chunks_handling.c
        bin_chunks_handling.h
        bin_chunks_handling.c
        path_finding.c
        path_finding.h
        move_chunk.c
        move_chunk.h
        file_saving.c)
