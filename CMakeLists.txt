cmake_minimum_required(VERSION 3.10)

# Project name
project(task_manager C)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Add source files
set(SOURCES
    src/main.c
    src/task.c
)

# Add include directories
include_directories(src)

# Create the executable
add_executable(task_manager ${SOURCES})

