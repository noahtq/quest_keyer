find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(keying run_key.cpp
        ${PROJECT_SOURCE_DIR}/quest_keyer/keyer/quest_keyer.h
        ${PROJECT_SOURCE_DIR}/quest_keyer/keyer/quest_keyer.cpp
        ${PROJECT_SOURCE_DIR}/quest_keyer/include/quest_seq_lib.h)
target_link_libraries(keying PUBLIC ${OpenCV_LIBS})
target_link_libraries(keying PUBLIC ${CMAKE_SOURCE_DIR}/quest_keyer/lib/libquest_image_seq_0.0.2.a)
