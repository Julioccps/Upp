cmake_minimum_required(VERSION 3.16)
project(upp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

file(GLOB LIB_SOURCES lib/*.cpp)

add_library(upp_libs STATIC ${LIB_SOURCES})

add_executable(upp src/main.cpp)

target_link_libraries(upp PRIVATE upp_libs)
