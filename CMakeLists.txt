cmake_minimum_required(VERSION 3.16)
project(ProjetoLP2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

if(SOURCES)
    add_executable(${PROJECT_NAME} ${SOURCES})
else()
    message(STATUS "No source files found in src/. Add .cpp or .c files to src/ to build.")
endif()
