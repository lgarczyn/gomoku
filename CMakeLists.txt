cmake_minimum_required(VERSION 3.4)
project(gomoku)

#include_directories("/nfs/2014/e/edelangh/.brew/include")
include_directories("/nfs/2014/l/lgarczyn/.brew/include")
#link_directories("/nfs/2014/e/edelangh/.brew/lib")
link_directories("/nfs/2014/l/lgarczyn/.brew/lib")
link_libraries("sfml-system")
link_libraries("sfml-graphics")
link_libraries("sfml-window")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ofast")

set(SOURCE_FILES
        main.cpp
        Board.cpp
        Board.hpp
        Analyzer.cpp
        Analyzer.hpp
        IAnalyzer.hpp
        AnalyzerBrainDead.cpp
        AnalyzerBrainDead.hpp
        Game.cpp
        Game.hpp
        MTDF.cpp
        MTDF.hpp
        BoardPos.cpp
        BoardPos.hpp
        BoardSquare.hpp
        GuiManager.cpp
        GuiManager.hpp
        PlayerColor.hpp PlayerColor.cpp Constants.hpp TextureManager.cpp TextureManager.hpp VictoryState.hpp)
add_executable(gomoku ${SOURCE_FILES})