cmake_minimum_required(VERSION 3.10)
project(tumor)

set(CMAKE_CXX_STANDARD 17)

add_executable(tumor main.cpp State.cpp State.h Automaton.cpp Automaton.h Parameters.cpp Parameters.h util.h)