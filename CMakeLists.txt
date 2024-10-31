cmake_minimum_required(VERSION 3.19)
project(duck_reflect)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(./)

file(GLOB_RECURSE SOURCES "reflect/*.cpp")

add_executable(main ${SOURCES} main.cpp)


















