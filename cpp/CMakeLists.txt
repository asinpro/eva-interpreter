cmake_minimum_required(VERSION 3.14)
project(eva_interpreter_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/Environment.cpp
    src/AST.cpp
    src/Parser.cpp
    src/Eva.cpp
    src/Transformer.cpp
)

# Library
add_library(eva_lib ${SOURCES})

# Test executables
add_executable(test_environment tests/test_environment.cpp)
target_link_libraries(test_environment eva_lib)

add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser eva_lib)

add_executable(test_eva tests/test_eva.cpp)
target_link_libraries(test_eva eva_lib)

add_executable(test_transformer tests/test_transformer.cpp)
target_link_libraries(test_transformer eva_lib)

add_executable(test_class tests/test_class.cpp)
target_link_libraries(test_class eva_lib)

add_executable(test_class_simple tests/test_class_simple.cpp)
target_link_libraries(test_class_simple eva_lib)

# CLI executable
add_executable(eva bin/eva.cpp)
target_link_libraries(eva eva_lib)

# Enable testing
enable_testing()
add_test(NAME EnvironmentTest COMMAND test_environment)
add_test(NAME ParserTest COMMAND test_parser)
add_test(NAME EvaTest COMMAND test_eva)
add_test(NAME TransformerTest COMMAND test_transformer)
add_test(NAME ClassTest COMMAND test_class)