cmake_minimum_required(VERSION 3.26)
project(Projeto1 C)

set(CMAKE_C_STANDARD 11)

add_executable(Projeto1 main.c
        biblioteca.c
        biblioteca.h)
