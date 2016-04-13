cmake_minimum_required(VERSION 3.4)
project(gomoku)

set(Shark_DIR ~/.brew/Cellar/shark/3.0.0/lib/cmake/Shark)

# Find the Shark libraries and includes
# set Shark_DIR to the proper location of Shark
find_package(Shark REQUIRED)
include(${SHARK_USE_FILE})

include_directories("~/.brew/include")
link_directories("~/.brew/lib")
link_libraries("sfml-system")
link_libraries("sfml-graphics")
link_libraries("sfml-window")
link_libraries("boost_serialization")
link_libraries("shark")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -ofast ")

set(SOURCE_FILES
        main.cpp
        GUI.cpp
        GUI.hpp
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
        GUIManager.cpp
        GUIManager.hpp
        PlayerColor.hpp
        PlayerColor.cpp
        Constants.hpp
        TextureManager.cpp
        TextureManager.hpp
        VictoryState.hpp
        ChildBoard.cpp
        ChildBoard.hpp
        BoardStore.cpp
        BoardStore.hpp Trainer.cpp Trainer.hpp NetworkManager.cpp NetworkManager.hpp)
add_executable(gomoku ${SOURCE_FILES})
target_link_libraries(gomoku ${SHARK_LIBRARIES})