cmake_minimum_required(VERSION 3.10)

project(HelloWorld C)
set(CMAKE_C_STANDARD 11)

add_executable(hello
    ./src/main.c
)

# Find dialog header and libraries
find_path(DIALOG_INCLUDE_DIR dialog.h)
find_library(DIALOG_LIBRARY dialog)

# Find wide-char ncurses
find_library(NCURSESW_LIBRARY ncursesw)

# Find math library (libm)
find_library(MATH_LIBRARY m)

# if (NOT DIALOG_INCLUDE_DIR OR NOT DIALOG_LIBRARY OR NOT NCURSESW_LIBRARY OR NOT MATH_LIBRARY)
#     message(FATAL_ERROR "Required libs not found: dialog, ncursesw, m. On Ubuntu: sudo apt-get install libdialog-dev libncursesw5-dev")
# endif()

target_include_directories(hello PRIVATE ${DIALOG_INCLUDE_DIR})
target_link_libraries(hello PRIVATE ${DIALOG_LIBRARY} ${NCURSESW_LIBRARY} ${MATH_LIBRARY})