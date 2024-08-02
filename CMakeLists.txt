cmake_minimum_required(VERSION 3.28)
project(quest_keyer VERSION 0.0.1)

find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 23)
set(FinalEXEName "quest_keyer_${CMAKE_PROJECT_VERSION}") #Append software version number to end of executable

add_executable(${FinalEXEName} src/main.cpp
include/quest_seq_lib.h)
target_link_libraries(${FinalEXEName} ${OpenCV_LIBS})
target_link_libraries(${FinalEXEName} ${CMAKE_SOURCE_DIR}/lib/libquest_image_seq_0.0.1.a)

add_subdirectory(tests/Google_tests)
