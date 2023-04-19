cmake_minimum_required(VERSION 3.1...3.26)
project(
    conway
    VERSION 0.1
    LANGUAGES CXX)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(conway src/main.cpp src/conway.cpp)
target_link_libraries(conway ${SDL2_LIBRARIES})
