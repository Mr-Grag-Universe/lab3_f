cmake_minimum_required(VERSION 3.16)
project(l3 C)

set(CMAKE_C_STANDARD 99)

include_directories(libs headers libs/headers)

add_executable(l3 main.c table_creation.c table_1_funcs.c table_2_funcs.c table_functions.c spetial_operations.c free_delete.c
        libs/MyString.c
        libs/KGetLine.c
        libs/FGetLine.c
        print_table.c free_delete.c find.c)
