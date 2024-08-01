cmake_minimum_required(VERSION 3.28)
project(CarAccidentsVisualization)

set(CMAKE_CXX_STANDARD 14)

if (WIN32)
    set(SFML_DIR "${CMAKE_SOURCE_DIR}/SFML-2.6.1/lib/cmake/SFML")
endif(WIN32)

find_package(SFML 2.6.1 COMPONENTS system window graphics audio REQUIRED)

add_executable(CarAccidentsVisualization main.cpp Window.h Window.cpp)
target_link_libraries(CarAccidentsVisualization sfml-system sfml-window sfml-graphics sfml-audio)