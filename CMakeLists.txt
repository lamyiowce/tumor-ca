cmake_minimum_required(VERSION 3.10)
project(tumor)

set(CMAKE_CXX_STANDARD 11)

# Set default compile flags for GCC  # TODO do wywalenia, na Windowsie tego potrzebowalem
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(include)

add_executable(tumor
        main.cpp
        automaton/Automaton.cpp
		automaton/Cycles.cpp
        automaton/Parameters.cpp
        automaton/State.cpp
        include/Automaton.h
		include/Cycles.h
        include/Parameters.h
        include/util.h)