cmake_minimum_required(VERSION 3.10)

# Project name
project(HelloWorld)

# Set C standard
set(CMAKE_C_STANDARD 11)

# Source files
set(SOURCES
    ./src/main.c
    ./src/input.c
)

# Executable
add_executable(hello ${SOURCES})