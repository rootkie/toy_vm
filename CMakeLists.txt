project(emulator C)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wshadow -s -O2 -funroll-loops -m64 -Wno-unused-parameter")
add_executable(${PROJECT_NAME} ./memory.c ./cpu.c ./emulator.c)
