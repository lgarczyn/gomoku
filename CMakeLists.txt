cmake_minimum_required(VERSION 2.4)
project(gomoku)

#set(Shark_DIR ~/.brew/Cellar/shark/3.0.0/lib/cmake/Shark)

# Find the Shark libraries and includes
# set Shark_DIR to the proper location of Shark
# find_package(Shark REQUIRED)
# include(${SHARK_USE_FILE})

include_directories("~/.brew/include")
link_directories("~/.brew/lib")
link_libraries("sfml-system")
link_libraries("sfml-graphics")
link_libraries("sfml-window")
link_libraries("boost_serialization")
#link_libraries("shark")


set(CMAKE_CXX_FLAGS " -std=c++11 -Ofast -ffast-math ${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
        main.cpp
        GUI.cpp
        Game.cpp
        MTDF.cpp
        GUIManager.cpp
        PlayerColor.cpp
        TextureManager.cpp
        MoveScore.cpp
        BoardPos.cpp)
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_executable(gomoku ${SOURCE_FILES})
#target_link_libraries(gomoku ${SHARK_LIBRARIES})
