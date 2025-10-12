cmake_minimum_required(VERSION 3.16)
project(TisaSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

include_directories(include)

file(GLOB SRC_FILES src/*.cpp)
file(GLOB TEST_MAIN tests/main.cpp)

add_executable(main.out ${SRC_FILES} ${TEST_MAIN})

set_target_properties(main.out PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build
)
