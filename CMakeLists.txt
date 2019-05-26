cmake_minimum_required(VERSION 2.4)
project(gomoku)

include_directories("~/.brew/include")
link_directories("~/.brew/lib")
link_libraries("sfml-system")
link_libraries("sfml-graphics")
link_libraries("sfml-window")
link_libraries("pthread")


set(CMAKE_CXX_FLAGS " -std=c++11 -ofast -ffast-math -march=native ${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
        main.cpp
        GUI.cpp
        Board.cpp
        Analyzer.cpp
        Game.cpp
        GUIManager.cpp
        PlayerColor.cpp
        TextureManager.cpp
        MoveScore.cpp)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(gomoku ${SOURCE_FILES})
