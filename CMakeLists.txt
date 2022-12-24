cmake_minimum_required(VERSION 3.23)
project(MOT_CParser C)

set(CMAKE_C_STANDARD 99)

include_directories(include/)
add_executable(MOT_CParser src/callouts.c src/circular_buffer.c src/execute.c src/parser/parser.c main.c include/parser/parser_interface.h)
