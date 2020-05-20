cmake_minimum_required(VERSION 3.15)
project(t1_raw C)

set(CMAKE_C_STANDARD 11)

add_executable(main main.c include/Filters.h include/Tools.h include/ImageProcessing.h)
target_link_libraries (main m)