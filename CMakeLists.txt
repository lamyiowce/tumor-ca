cmake_minimum_required(VERSION 3.5)
project(tumor)

set(CMAKE_CXX_STANDARD 17)

# Set compile flags for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
	message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -fopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-g ")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(include)

set(SOURCES
        automaton/Automaton.cpp
		automaton/Cycles.cpp
        automaton/Parameters.cpp
        automaton/State.cpp
		automaton/RandomEngine.cpp
        automaton/IrradiationProtocol.cpp
        )

add_executable(tumor
        main.cpp
        ${SOURCES}
        )

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
        tests/DiffusionTests.cpp
        tests/OneStepTests.cpp
        tests/DiffusionTests.cpp
        tests/IrradiationProtocolLoadFromFileTests.cpp
        tests/IntegrationTest.cpp
        )

target_link_libraries(tumor stdc++fs)
