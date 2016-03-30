cmake_minimum_required(VERSION 3.4)
project(gomoku)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Board.cpp Board.hpp Analyzer.cpp Analyzer.hpp IAnalyzer.hpp AnalyzerBrainDead.cpp AnalyzerBrainDead.hpp Game.cpp Game.hpp MTDF.cpp MTDF.hpp IHeuristics.hpp Heuristics.hpp)
add_executable(gomoku ${SOURCE_FILES})