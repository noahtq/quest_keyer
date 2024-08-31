cmake_minimum_required(VERSION 3.5)
project(quest_keyer_backend_test CXX)

find_package(OPENCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} test_main.cc
        ../controllers/QuestKeyerAPI_KeyerAPI.h
        ../quest_keyer/keyer/quest_keyer.h
        ../include/json.hpp
        ../quest_keyer/include/quest_seq_lib.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${OpenCV_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/quest_keyer/lib/libquest_image_seq_0.0.3.a)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)


ParseAndAddDrogonTests(${PROJECT_NAME})
