cmake_minimum_required(VERSION 3.10)
project(MatrixMultiplication)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wextra")

# Busca e linka a biblioteca de threads
find_package(Threads REQUIRED)

# Inclui os fontes e gera o executável
file(GLOB SOURCES "src/*.c")
add_executable(meu_exe ${SOURCES})
target_link_libraries(meu_exe PRIVATE Threads::Threads)