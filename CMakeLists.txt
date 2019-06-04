cmake_minimum_required (VERSION 3.3)
project(GameOfLife)

set(GameOfLife_VERSION_MAJOR 1)
set(GameOfLife_VERSION_MINOR 0)

add_subdirectory("yaml-cpp")

set(GOL_SRC gameoflife.cpp world.cpp cell.cpp json11.cpp)
set(VIEW_SRC gameoflife_viewer.cpp  json11.cpp)

include_directories("yaml-cpp/include" "spdlog/include" "cxxopts/include")

add_executable(GameOfLife ${GOL_SRC})
add_executable(GameOfLifeViewer ${VIEW_SRC})

target_link_libraries(GameOfLife "yaml-cpp")
target_link_libraries(GameOfLifeViewer "yaml-cpp")

