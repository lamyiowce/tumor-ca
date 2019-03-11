cmake_minimum_required(VERSION 3.5)
project(tumor)

set(CMAKE_CXX_STANDARD 17)

# Set default compile flags for GCC  # TODO do wywalenia, na Windowsie tego potrzebowalem
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
    set(FLAGS "-Wall -Wextra -pedantic -g")
    set(CMAKE_CXX_FLAGS_DEBUG "${FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "${FLAGS} -O3")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(include)

set(SOURCES
        automaton/Automaton.cpp
		automaton/Cycles.cpp
        automaton/Parameters.cpp
        automaton/State.cpp
		automaton/RandomEngine.cpp)

add_executable(tumor
        main.cpp
        ${SOURCES})

add_executable(tests
        ${SOURCES}
        tests/tests_main.cpp
        tests/MatlabRandomEngineTests.cpp
        tests/ReplenishSubstrateTests.cpp
        tests/StateHelper.cpp
        tests/StateHelper.h
        tests/IrradiateTumorTests.cpp
        tests/CellDivisionTests.cpp
        tests/MetaboliseNutrientsTests.cpp
        tests/SetLocalStatesTests.cpp
        tests/SetGlobalStatesTests.cpp
        tests/DiffusionTests.cpp)
