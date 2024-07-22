cmake_minimum_required(VERSION 3.28)
project(quist_keyer)

find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 23)

add_executable(quist_keyer main.cpp)
target_link_libraries(quist_keyer ${OpenCV_LIBS})

add_subdirectory(Google_tests)
