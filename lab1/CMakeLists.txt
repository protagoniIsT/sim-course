cmake_minimum_required(VERSION 3.16)
project(TisaSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include_directories(include)

file(GLOB SRC_FILES src/*.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_SOURCE_DIR}/src/main.cpp)

add_executable(sim.out ${SRC_FILES} src/main.cpp)

add_subdirectory(external/gflags)
target_link_libraries(sim.out PRIVATE gflags)

set_target_properties(sim.out PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)

add_executable(sim_tests.out ${SRC_FILES} tests/test_helpers.cpp)
target_link_libraries(sim_tests.out PRIVATE gflags)

set_target_properties(sim_tests.out PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)
