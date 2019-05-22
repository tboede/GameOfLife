cmake_minimum_required (VERSION 3.3)
project(GameOfLife)

set(GameOfLife_VERSION_MAJOR 1)
set(GameOfLife_VERSION_MINOR 0)

add_subdirectory("yaml-cpp")

set(SOURCES main.cpp world.cpp cell.cpp json11.cpp)

include_directories("yaml-cpp/include")

add_executable(GameOfLife ${SOURCES})

target_link_libraries(GameOfLife "yaml-cpp")

