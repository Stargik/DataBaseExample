cmake_minimum_required(VERSION 3.21)
project(DataBaseExample C)

set(CMAKE_C_STANDARD 99)

add_executable(DataBaseExample main.c Structures.h Task.h Cource.h ErrorChecks.h ConsoleActions.h)
