cmake_minimum_required(VERSION 2.4)
project(gomoku)

include_directories("~/.brew/include")
link_directories("~/.brew/lib")
link_libraries("sfml-system")
link_libraries("sfml-graphics")
link_libraries("sfml-window")


set(CMAKE_CXX_FLAGS " -std=c++11 -ofast -ffast-math -march=native ${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
        main.cpp
        GUI.cpp
        GUI.hpp
        Board.cpp
        Board.hpp
        IAnalyzer.hpp
        Analyzer.cpp
        Analyzer.hpp
        Game.cpp
        Game.hpp
        BoardPos.hpp
        BoardSquare.hpp
        GUIManager.cpp
        GUIManager.hpp
        PlayerColor.hpp
        PlayerColor.cpp
        Constants.hpp
        TextureManager.cpp
        TextureManager.hpp
        VictoryState.hpp
        ChildBoard.hpp
        MoveScore.cpp
        MoveScore.hpp
        BoardPos.cpp
        ThreadPool.hpp
        ThreadData.hpp
        Options.hpp)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(gomoku ${SOURCE_FILES})
target_link_libraries(gomoku ${SHARK_LIBRARIES})
