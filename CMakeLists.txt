﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("spic-groep-m")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(spic-groep-m Main.cpp)
target_link_libraries(spic-groep-m ${SDL2_LIBRARIES})