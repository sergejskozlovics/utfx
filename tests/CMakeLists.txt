cmake_minimum_required(VERSION 3.19)
project(utfx_tests C)

set(CMAKE_C_STANDARD 90)

add_executable(utfx_tests utfx_tests.c)
include_directories(..)

target_link_libraries(utfx_tests PUBLIC utfx)
