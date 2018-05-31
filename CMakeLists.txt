cmake_minimum_required(VERSION 3.8)
project(spectre)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1")

set(SOURCE_FILES main.c)
add_executable(spectre ${SOURCE_FILES})
