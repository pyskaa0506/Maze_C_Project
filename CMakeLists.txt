cmake_minimum_required(VERSION 3.26)
project(Maze_C_Project C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Maze_C_Project
        file_loading.c
        file_loading.h
        chunks_handling.h
        main.c
        chunks_handling.c)
