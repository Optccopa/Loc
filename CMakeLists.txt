cmake_minimum_required(VERSION 3.10)
project(loc VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(loc src/main.cpp)

add_custom_command(
    TARGET loc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "loc path: $<TARGET_FILE:loc>"
)