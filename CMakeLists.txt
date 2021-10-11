# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("spic-prj-api")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(spic-prj-api Main.cpp)
target_link_libraries(spic-prj-api ${SDL2_LIBRARIES})