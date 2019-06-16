cmake_minimum_required(VERSION 3.5)
project(tumor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -fopenmp")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -flto")


include_directories(src/automaton)
include_directories(third_party)

set(SOURCES
        src/automaton/Automaton.cpp
		src/automaton/Cycles.cpp
		src/automaton/Parameters.cpp
		src/automaton/State.cpp
		src/automaton/RandomEngine.cpp
		src/automaton/IrradiationProtocol.cpp
        )

set(TEST_SOURCES
		src/tests/tests_main.cpp
		src/tests/MatlabRandomEngineTests.cpp
		src/tests/ReplenishSubstrateTests.cpp
		src/tests/StateHelper.cpp
		src/tests/StateHelper.h
		src/tests/IrradiateTumorTests.cpp
		src/tests/CellDivisionTests.cpp
		src/tests/MetaboliseNutrientsTests.cpp
		src/tests/SetLocalStatesTests.cpp
		src/tests/SetGlobalStatesTests.cpp
		src/tests/DiffusionTests.cpp
		src/tests/OneStepTests.cpp
		src/tests/DiffusionTests.cpp
		src/tests/IrradiationProtocolLoadFromFileTests.cpp
		src/tests/IntegrationTest.cpp
		)

add_executable(tumor
        src/main.cpp
        ${SOURCES}
        )

add_executable(tests
		${TEST_SOURCES}
        ${SOURCES}
        )

add_executable(run-simple
        src/simple-run.cpp
        ${SOURCES}
        )

add_executable(protocol_generator
        src/protocol_generator.cpp
        )

target_link_libraries(tumor stdc++fs)
