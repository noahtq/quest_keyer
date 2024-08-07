cmake_minimum_required(VERSION 3.5)
project(quest_keyer_backend_test CXX)

add_executable(${PROJECT_NAME} test_main.cc
        ../controllers/QuestKeyerAPI_KeyerAPI.h
        ../quest_keyer/keyer/quest_keyer.h
        ../quest_keyer/keyer/quest_keyer.h
        ../quest_keyer/include/json.hpp)

# ##############################################################################
# If you include the drogon source code locally in your project, use this method
# to add drogon 
# target_link_libraries(${PROJECT_NAME} PRIVATE drogon)
#
# and comment out the following lines
target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

ParseAndAddDrogonTests(${PROJECT_NAME})
