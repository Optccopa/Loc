cmake_minimum_required(VERSION 3.28)
project(loc LANGUAGES CXX)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_STANDARD 20)

add_executable(loc src/main.cpp)
