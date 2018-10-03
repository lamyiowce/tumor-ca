cmake_minimum_required(VERSION 3.10)
project(tumor)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(tumor
        main.cpp
        ca/Automaton.cpp
        ca/Parameters.cpp
        ca/State.cpp
        include/Automaton.h
        include/Parameters.h
        include/util.h)