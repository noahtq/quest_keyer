cmake_minimum_required(VERSION 3.28)
project(quist_keyer)

find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 23)

add_executable(quist_keyer src/main.cpp
        src/quist_keyer/facilities.cpp
        src/quist_keyer/facilities.h)
target_link_libraries(quist_keyer ${OpenCV_LIBS})

add_subdirectory(tests/Google_tests)
