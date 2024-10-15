cmake_minimum_required(VERSION 3.29)
project(4_in_a_row_with_MinMax_AI C)

set(CMAKE_C_STANDARD 11)

include_directories(./headers)

add_executable(4_in_a_row_with_MinMax_AI
    computerGame.c
    Program.c
    resources.c
)
